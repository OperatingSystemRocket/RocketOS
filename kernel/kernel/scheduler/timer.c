#include "timer.h"


static volatile uint64_t _timer_ticks = 0;
static volatile uint64_t _timer_seconds = 0;
static volatile uint64_t _timer_milliseconds = 0;
static uint64_t _timer_frequency = 0;

static uint64_t (*_counter_fun)(void) = NULL;
static uint64_t _counter_frequency = 0;

static size_t pit_counter_var = 0;


uint64_t pit_counter(void) {
    return pit_counter_var;
}

//TODO: replace __attribute__((interrupt)) as it is garbage
__attribute__((interrupt)) static void timer_handler(struct interrupt_frame *const frame) {
    ++pit_counter_var;

    kprintf("timer_handler()\n");

    timer_tick();

    kprintf("after timer_tick()\n");

    pic_send_eoi(1);
}

bool pit_counter_install(void) {
    uint64_t divisor = 1193180 / PIT_FREQUENCY;

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)divisor);
    outb(0x40, (uint8_t)(divisor >> 8));

    // Indicate the timer frequency
    timer_set_timer_frequency(PIT_FREQUENCY);

    pic_irq_enable(0);
    idt_register_handler(32, (uint32_t)timer_handler);

    // Let the timer know about the counter
    timer_counter_fun(pit_counter);
    timer_set_counter_frequency(PIT_FREQUENCY);

    return true;
}

void timer_install(void) {
    pit_counter_install();
}

void scheduler_tick(void); // can't include scheduler.h because of circular include

void timer_tick(void) {
    // Simply let the scheduler know about the tick
    scheduler_tick();
    kprintf("timer_tick end\n");
}

uint64_t timer_seconds(void) {
    return timer_counter() / timer_get_counter_frequency();
}

uint64_t timer_milliseconds(void) {
    return timer_counter() / (timer_get_counter_frequency() / 1000);
}

uint64_t timer_get_timer_frequency(void) {
    return _timer_frequency;
}

void timer_set_timer_frequency(uint64_t freq) {
    uint64_t old_frequency = _timer_frequency;

    _timer_frequency = freq;

    scheduler_frequency_updated(old_frequency, _timer_frequency);
}

uint64_t timer_counter(void) {
    return _counter_fun();
}

uint64_t timer_get_counter_frequency(void) {
    return _counter_frequency;
}

void timer_set_counter_frequency(uint64_t freq) {
    _counter_frequency = freq;
}

void timer_counter_fun(uint64_t (*fun)(void)) {
    _counter_fun = fun;
}

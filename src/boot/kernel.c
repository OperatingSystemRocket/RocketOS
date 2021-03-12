#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "time.h"
#include "serial_driver.h"
#include "kassert.h"


void kernel_main(void) {
    terminal_initialize();

    pic_init();
    isr_install();

    //enable_time();
    enable_keyboard();
    terminal_start();

    kassert_void(serial_init()); //fails if serial is faulty

    serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");



    for(volatile uint32_t i = 0u; ; ++i);
}

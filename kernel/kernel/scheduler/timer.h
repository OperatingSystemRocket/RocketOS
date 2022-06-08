#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <interrupts.h>
#include <hardware_io.h>


#define PIT_FREQUENCY 1000

/*!
 * \brief Install the system timer
 */
void timer_install(void);

/*!
 * \brief Returns a up-counter in seconds
 */
uint64_t timer_seconds(void);

/*!
 * \brief Returns a up-counter in milliseconds
 */
uint64_t timer_milliseconds(void);

/*!
 * \brief Let the timer know of a new tick
 */
void timer_tick(void);

/*!
 * \brief Return the frequency in Hz of the current timer system.
 */
uint64_t timer_get_timer_frequency(void);

/*!
 * \brief Sets the frequency in Hz of the current timer system.
 */
void timer_set_timer_frequency(uint64_t freq);

/*!
 * \brief Returns a up-counter based on the counter frequency
 */
uint64_t timer_counter(void);

/*!
 * \brief Return the frequency in Hz of the current counter system.
 */
uint64_t timer_get_counter_frequency(void);

/*!
 * \brief Sets the frequency in Hz of the current counter system.
 */
void timer_set_counter_frequency(uint64_t freq);

/*!
 * \brief Sets the function to use to get the counter value;
 */
void timer_counter_fun(uint64_t (*fun)(void));

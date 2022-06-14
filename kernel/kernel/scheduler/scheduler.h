#pragma once


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <vector.h>
#include <string.h>
#include <optional.h>
#include <int_lock.h>

#include <gdt.h>
#include <buddy_memory_allocator.h>
#include <osi_memory_allocator.h>
#include <paging.h>
#include <interrupt_types.h>
#include <physical_pointer.h>

#include <tar.h>

#include <kstdio.h>
#include <kstdlib.h>
#include <kassert.h>
#include <kstring.h>

#include <default_vga_driver.h>

#include <process.h>
#include <timer.h>


#define MAX_PROCESS 128

/*!
 * \brief Return the id of the current process
 */
pid_t scheduler_get_pid(void);

/*!
 * \brief Return the process with the given ID
 */
struct process_t* scheduler_get_process(pid_t pid);

/*!
 * \brief Returns the state of the process with the given ID
 */
enum process_state scheduler_get_process_state(pid_t pid);

/*!
 * \brief Block the given process and immediately reschedule it
 */
void scheduler_block_process(pid_t pid);

/*!
 * \brief Unblock a blocked process
 */
void scheduler_unblock_process(pid_t pid);

/*!
 * \brief Unblock a process if it is a blocked
 */
void scheduler_unblock_process_hint(pid_t pid);

/*!
 * \brief Init the scheduler
 */
void scheduler_init(void);

/*!
 * \brief Start the scheduler and starts the first process
 */
void scheduler_start(void); //__attribute__((noreturn));

/*!
 * \brief Indicates if the scheduler is started or not
 */
bool scheduler_is_started(void);

/*!
 * \brief Execute the given file in a new process
 */
GENERATE_OPTIONAL(pid_t)
struct OPTIONAL_NAME(pid_t) scheduler_exec(struct string file, const vector_type(struct string) params);

/*!
 * \brief Kill the current process
 */
void scheduler_kill_current_process(void); //__attribute__((noreturn));

/*!
 * \brief Wait for the given process to terminate
 */
void scheduler_await_termination(pid_t pid);

/*!
 * \brief Allocate more memory for the process
 * \param inc The amount of memory to add
 */
void scheduler_sbrk(size_t inc);

/*!
 * \brief Let the scheduler know of a timer tick
 */
void scheduler_tick(void);

/*!
 * \brief Let another process run.
 *
 * This may change the state of the current process state
 */
void scheduler_yield(void);

/*!
 * \brief Reschedule to another process, if the current process is not running
 *
 * This will not change the state of the process!
 */
void scheduler_reschedule(void);

/*!
 * \brief Indicates a fault in the current process
 */
void scheduler_fault(void);

/*!
 * \brief Make the current process sleep for the given amount of milliseconds
 * \param time The number of milliseconds to wait
 */
void scheduler_sleep_ms(size_t time);

/*!
 * \brief Make the given process sleep for the given amount of milliseconds
 * \param time The number of milliseconds to wait
 */
void scheduler_proc_sleep_ms(pid_t pid, size_t time);

/*!
 * \brief Block the given process, but do not reschedule
 */
void scheduler_block_process_light(pid_t pid);

/*!
 * \brief Block the given process, with a possible timeout, but do not reschedule
 */
void scheduler_block_process_timeout_light(pid_t pid, size_t ms);

/*!
 * \brief Creat a kernel process
 * \param name The name of the process
 * \param user_stack Pointer to the user stack
 * \param kernel_stack Pointer to the kernel stack
 * \param fun The function to execute
 */
struct process_t* scheduler_create_kernel_task(const char* name, char* user_stack, char* kernel_stack, void (*fun)(void));

/*!
 * \brief Creat a kernel process with some data
 * \param name The name of the process
 * \param user_stack Pointer to the user stack
 * \param kernel_stack Pointer to the kernel stack
 * \param fun The function to execute
 * \param data The data to pass to the function
 */
struct process_t* scheduler_create_kernel_task_args(const char* name, char* user_stack, char* kernel_stack, void (*fun)(void*), void* data);

/*!
 * \brief Queue a created system process
 */
void scheduler_queue_system_process(pid_t pid);

/*!
 * \brief Queue an initilization task that will be run after the
 * scheduler is started
 *
 * This must be used for drivers that needs scheduling to be started
 * or for drivers depending on others drivers asynchronously
 * started.
 */
void scheduler_queue_async_init_task(void (*fun)(void));

/*!
 * \brief Lets the scheduler know that the timer frequency has been updated
 */
void scheduler_frequency_updated(uint64_t old_frequency, uint64_t new_frequency);


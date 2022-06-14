#pragma once


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <vector.h>
#include <string.h>
#include <interrupt_types.h>



#define MAX_PRIORITY 4
#define MIN_PRIORITY 1
#define PRIORITY_LEVELS ((MAX_PRIORITY - MIN_PRIORITY) + 1)
#define DEFAULT_PRIORITY 3

typedef size_t pid_t; ///< A process id

#define INVALID_PID (1024 * 1024 * 1024) //I'm pretty sure we won't violate this limit

enum process_state {
    PROCESS_EMPTY = 0, ///< Not a process
    PROCESS_NEW = 1,  ///< A newly created process
    PROCESS_READY = 2, ///< A process ready to run
    PROCESS_RUNNING = 3, ///< A process currently executing
    PROCESS_BLOCKED = 4, ///< A blocked process
    PROCESS_SLEEPING= 5, ///< A sleeping process
    PROCESS_WAITING = 6, ///< A waiting process (for a child)
    PROCESS_KILLED = 7, ///< A killed process
    PROCESS_BLOCKED_TIMEOUT = 8 ///< A blocked, with timeout, process
};

/*!
 * \brief A physical segment of memory used by the process
 */
struct segment_t {
    size_t physical; ///< The physical start
    size_t size; ///< The size of allocated memory
};

struct wait_node {
    size_t pid;
    struct wait_node* next;
};

struct process_t {
    pid_t pid;  ///< The process id
    pid_t ppid; ///< The parent's process id

    bool system; ///< Indicates if the process is a system process

    size_t priority; ///< The priority of the process

    size_t physical_cr3; ///< The physical address of the CR3
    size_t paging_size; ///< The  size of the paging structure

    size_t physical_user_stack; ///< The physical address of the user stack
    size_t physical_kernel_stack; ///< The physical address of the kernel stack
    size_t virtual_kernel_stack; ///< The virtual address of the kernel stack

    size_t kernel_esp; ///< The kernel stack pointer

    size_t brk_start; ///< The start of the brk section
    size_t brk_end; ///< The end of the brk section

    // Only for system kernels
    char* user_stack; ///< Pointer to the user stack
    char* kernel_stack; ///< Pointer to the kernel stack

    volatile struct syscall_regs* context; ///< A pointer to the context

    struct wait_node wait; ///< The process's wait node

    vector_type(struct segment_t) segments; ///< The physical segments

    struct string name; ///< The name of the process
};

#define program_base 0x8000000000 ///< The virtual address of a program start
#define program_break 0x9000000000 ///< The virtual address of a program break start

#define user_stack_size (2 * PAGE_SIZE) ///< The size of the user stack
#define kernel_stack_size (2 * PAGE_SIZE) ///< The size of the kernel stack

#define user_stack_start (program_base + 0x700000) ///< The virtual address of a program user stack
// TODO: check if it should be `-4` because it's 32 bit
#define user_esp (user_stack_start + (user_stack_size - 8)) ///< The initial program stack pointer

/*!
 * \brief An entry in the Process Control Block
 */
struct process_control_t {
    struct process_t process; ///< The process itself
    enum process_state state; ///< The state of the process
    size_t rounds; ///< The number of rounds remaining
    size_t sleep_timeout; ///< The sleep timeout (in ticks)
};

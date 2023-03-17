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
    PROCESS_BLOCKED_TIMEOUT = 8, ///< A blocked, with timeout, process
    PROCESS_ERROR = 9 /// < used for error return codes involving processes
};


struct process_context {
    uint32_t ebx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t eip;
    uint32_t esp;
};

struct process_t {
    pid_t pid;  ///< The process id
    //pid_t ppid; ///< The parent's process id

    bool system; ///< Indicates if the process is a system process

    size_t priority; ///< The priority of the process

    uint32_t physical_cr3; ///< The physical address of the CR3
    uint32_t virtual_cr3; ///< The virtual address of the CR3 in the kernel section of the address space

    uint32_t physical_stack; ///< The physical address of the kernel stack

    volatile struct process_context context; ///< A pointer to the context

    struct string name; ///< The name of the process
};

struct process_control_t {
    struct process_t process; ///< The process itself
    enum process_state state; ///< The state of the process
    size_t rounds; ///< The number of rounds remaining
    size_t sleep_timeout; ///< The sleep timeout (in ticks)
};

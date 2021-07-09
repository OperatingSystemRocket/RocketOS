#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "paging.h"
#include "gdt.h"
#include "kstdlib.h"
#include "interrupts.h"
#include "time.h"

#include "kassert.h"
#include "kstdio.h"


extern volatile bool should_switch_task;


struct task_context {
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
};


struct process {
    bool previously_loaded; //used to know whether to use `load_task` or `resume_task`
    uint32_t* stack; //convenience pointer that is a duplicate of the base pointer

    int32_t id; //pid

    struct task_context register_states;

    uint32_t* page_directory;

    struct process* next; //circular linked list
};


extern void save_current_task(struct task_context* current_task);
extern void load_task(struct task_context* current_task);
extern void resume_task(struct task_context* current_task);

void create_process(void (*entry_point)(void));

void scheduler_init(void);
void enable_timer(void);

void example_function_task(void);
void foo_function_task(void);


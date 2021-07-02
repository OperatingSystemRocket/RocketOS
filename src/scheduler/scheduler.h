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

#include "kstdio.h"


extern volatile bool should_switch_task;


struct task_context {
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
};


struct process {
    int32_t id; //pid

    struct task_context register_states;

    uint32_t* page_directory;

    struct process* next; //circular linked list
};


extern void save_current_task(struct task_context* current_task);
extern void load_task(struct task_context* current_task);
extern void resume_task(struct task_context* current_task);

void scheduler_init(void);
void enable_timer(void);

void example_function_task(void);


#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kstring.h"
#include "paging.h"
#include "gdt.h"
#include "kstdlib.h"

#include "kstdio.h"


struct task_context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
};


struct process {
    int32_t id; //pid

    struct task_context register_states;

    uint32_t* page_directory;

    struct process* next; //circular linked list
};

extern void context_switch(struct task_context** old, struct task_context* new_context);


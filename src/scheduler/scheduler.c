#include "scheduler.h"


volatile struct process* current_process;
volatile struct process* ready_queue;


/*
 * task.c
 *
 *  Created on: Oct 22, 2018
 *      Author: maanu
 */


#include "stdint.h"
#include "stdbool.h"

#include "PSR.h"
#include "task.h"

static KernelTcb_t  sTask_list[MAX_TASK_NUM];
static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
static uint32_t     sAllocated_tcb_index;
static uint32_t     sCurrent_tcb_index;

static KernelTcb_t* Scheduler_round_robin_algorithm(void);

extern void Save_context();
extern void Restore_context();

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;

    for(uint32_t i = 0 ; i < MAX_TASK_NUM ; i++)
    {
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t* ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc = 0;
        ctx->spsr = PSR_INIT;
    }
}

void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++];

    if (sAllocated_tcb_index > MAX_TASK_NUM)
    {
        return NOT_ENOUGH_TASK_NUM;
    }

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)new_tcb->sp;
    ctx->pc = (uint32_t)startFunc;

    return (sAllocated_tcb_index - 1);
}

uint32_t Kernel_task_get_current_task_id(void)
{
    return sCurrent_tcb_index;
}

void Kernel_task_scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();

    Kernel_task_context_switching();
}

__attribute__ ((naked)) void Kernel_task_context_switching(void)
{
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

static KernelTcb_t* Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}

/*
 * DebugCliTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "stdlib.h"

#include "Kernel.h"

#define MAX_CMD_LEN	16

typedef void (*CmdFunc_t)(char* param);

typedef struct cmdTable
{
	uint8_t		cmdLen;
	char* 		cmd;
	char* 		desc;
	CmdFunc_t	func;
} CmdTable_t;

static void HandleDebugCommand(void);

static void HandleCmdHelp(char* param);

static CmdTable_t sCmdTable[] =
{
		{4, "help",		"show this information", 	HandleCmdHelp},
		{0, NULL,NULL,NULL}
};

void Debug_cli_task(void)
{
    debug_printf("Debug_cli_task....\n");

    while (true)
    {
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn);
        if (handle_event == KernelEventFlag_CmdIn)
        {
        	HandleDebugCommand();
        }
        Kernel_yield();
    }
}

static void HandleDebugCommand(void)
{
	char cmdStr[MAX_CMD_LEN] = {0};
	char ch;
	uint32_t i;
	bool isLenExceed = true;

	for(i = 0 ; i < (MAX_CMD_LEN - 1) ; i++)
	{
		Kernel_recv_msg(KernelMsgQ_DebugCmd, &ch, 1);

		cmdStr[i] = ch;

		if (ch == '\0')
		{
			isLenExceed = false;
			break;
		}
	}

	if (isLenExceed)
	{
		// flush msgQ
		do
		{
			Kernel_recv_msg(KernelMsgQ_DebugCmd, &ch, 1);
		} while (ch != '\0');
	}

	uint32_t param_start_pos = memfind((uint8_t*)cmdStr, ' ', i);

	// split cmd and param
	char* param = NULL;
	if (param_start_pos != (i + 1))
	{
		*(cmdStr + param_start_pos) = '\0';
		param = (cmdStr + param_start_pos + 1);
	}

	CmdTable_t* pCmdTblRow = sCmdTable;
	while(pCmdTblRow->cmdLen != 0)
	{
		if (memncmp((uint8_t*)pCmdTblRow->cmd, (uint8_t*)cmdStr, pCmdTblRow->cmdLen))
		{
			pCmdTblRow->func(param);
			break;
		}
		pCmdTblRow++;
	}
}

static void HandleCmdHelp(char* param)
{
	CmdTable_t* pCmdTblRow = sCmdTable;
	while(pCmdTblRow->cmdLen != 0)
	{
		debug_printf("%s          %s\n", pCmdTblRow->cmd, pCmdTblRow->desc);
		pCmdTblRow++;
	}
}

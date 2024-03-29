/*
 * DebugCliTask.c
 *
 *  Created on: Feb 9, 2019
 *      Author: maanu
 */

#include "stdio.h"
#include "stdlib.h"

#include "usbd_hid.h"

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
static void HandleHid(char* param);
static void HandleReadMem(char* param);
static void StartHid(char* param);
static void StopHid(char* param);

static CmdTable_t sCmdTable[] =
{
		{4, "help",		"show this information", 	HandleCmdHelp},
		{3, "hid",		"send hid keyboard report",	HandleHid},
		{2, "RM",		"Read memory",				HandleReadMem},
		{5, "start",	"Just send report",			StartHid},
		{4, "stop",		"Stop report",				StopHid},
		{0, NULL,NULL,NULL}
};

void Debug_cli_task(void)
{
    DBG_PRINT("Debug_cli_task....\n");

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
		DBG_PRINT("%s          %s\n", pCmdTblRow->cmd, pCmdTblRow->desc);
		pCmdTblRow++;
	}
}

static void HandleHid(char* param)
{
	uint8_t hid_key_report[8] = {0};

	if (param != NULL)
	{
		hid_key_report[0] = 0x02; // shift
	}
	hid_key_report[2] = 0x11;	// 'n'
	hid_key_report[3] = 0x1b;	// 'x'
	Kernel_send_msg(KernelMsgQ_D2hData, hid_key_report, 8);
	Kernel_send_events(KernelEventFlag_SendD2H);

	Kernel_yield();
	USBD_Delay(200);

	hid_key_report[0] = 0;
	hid_key_report[2] = 0;	// clear
	hid_key_report[3] = 0;	// clear
	Kernel_send_msg(KernelMsgQ_D2hData, hid_key_report, 8);
	Kernel_send_events(KernelEventFlag_SendD2H);
}

static void StartHid(char* param)
{
	uint8_t hid_key_report[8] = {0};
	hid_key_report[2] = 0x11;	// 'n'
	hid_key_report[3] = 0x1b;	// 'x'
	Kernel_send_msg(KernelMsgQ_D2hData, hid_key_report, 8);
	Kernel_send_events(KernelEventFlag_SendD2H);
}

static void StopHid(char* param)
{
	uint8_t hid_key_report[8] = {0};
	hid_key_report[0] = 0;
	hid_key_report[2] = 0;	// clear
	hid_key_report[3] = 0;	// clear
	Kernel_send_msg(KernelMsgQ_D2hData, hid_key_report, 8);
	Kernel_send_events(KernelEventFlag_SendD2H);
}

static void HandleReadMem(char* param)
{
	if (param == NULL)
	{
		DBG_PRINT("No Param\n");
		return;
	}

	uint32_t* addr = (uint32_t*)htou(param, strncnt(param, 8));
	uint32_t data = *addr;

	DBG_PRINT("[%x] %x\n", addr, data);
}

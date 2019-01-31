#include "main.h"

#include "stdbool.h"
#include "Kernel.h"

int get_string_length(const char* s) {
    int len=0;
    while (s[len] != 0) {
        len++;
    }

    return len;
}

int is_equal(const char* one, const char* two) {
    int i;

    if (get_string_length(one) != get_string_length(two)) {
        return -1;
    }

    for (i=0; i<get_string_length(one); i++) {
        if (one[i] != two[i]) {
            return -1;
        }
    }

    return 0;
}

void init_output(void) {
    // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_Settings;

    // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // configure port A1 for driving an LED
    GPIO_Settings.GPIO_Pin = GPIO_Pin_1;
    GPIO_Settings.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_Settings.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_Settings) ;             // initialize port
}

// TODO: check a flag, and call init_output() in not initialized
void turn_on_pa1(void) {
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void turn_off_pa1(void) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void usart_init() {
    GPIO_InitTypeDef GPIO_Settings;
    USART_InitTypeDef USART_Settrings;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_Settings);
    GPIO_Settings.GPIO_Pin = GPIO_Pin_9;
    GPIO_Settings.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_Settings);

    GPIO_Settings.GPIO_Pin = GPIO_Pin_10;
    GPIO_Settings.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_Settings);

    USART_StructInit(&USART_Settrings);
    USART_Settrings.USART_BaudRate = 9600;
    USART_Settrings.USART_WordLength = USART_WordLength_8b;
    USART_Settrings.USART_StopBits = USART_StopBits_1;
    USART_Settrings.USART_Parity = USART_Parity_No;
    USART_Settrings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Settrings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_Settrings);

    USART_Cmd(USART1, ENABLE);

    __enable_irq();
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

// send one byte through USART
void usart_send(const char chr) {
    while(!(USART1->SR & USART_SR_TC));
    USART1->DR = chr;
}

// send a string through USART
void usart_send_string(const char *s) {
    int i=0;
    while (s[i]) {
        usart_send(s[i++]);
    }
}

void usart_send_newline(void) {
    usart_send_string(CRLF);
}

void usart_send_line(const char *s) {
    usart_send_string(s);
    usart_send_string(CRLF);
}

void handle_command(char *command) {
    if (is_equal(command, A1_ON) == 0) {
        turn_on_pa1();
    } else if (is_equal(command, A1_OFF) == 0) {
        turn_off_pa1();
    } else {
        usart_send_line(UNKNOWN_COMMAND);
    }
}

char usart_buf[COMMAND_MAX_LENGTH];
unsigned short usart_buf_length=0;

void USART1_IRQHandler() {
    unsigned char received;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        received = USART_ReceiveData(USART1);

        if (received == CR) {
            usart_buf[usart_buf_length] = 0;
            usart_send_newline();

            handle_command(usart_buf);
            usart_buf_length = 0;
        } else if (received == LF) {
            // ignore
        } else {
            if (usart_buf_length == COMMAND_MAX_LENGTH) {
                usart_send_newline();
                usart_send_line(COMMAND_TOO_LONG);
                usart_buf_length = 0;
                return;

            }

            usart_buf[usart_buf_length++] = received;

            // echo
            usart_send(received);
        }
    }
}

void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();

    taskId = Kernel_task_create(User_task0);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task0 creation fail\n");
    	usart_send_line("Task0 creation fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task1 creation fail\n");
    	usart_send_line("Task1 creation fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if (NOT_ENOUGH_TASK_NUM == taskId)
    {
        //putstr("Task2 creation fail\n");
    	usart_send_line("Task2 creation fail\n");
    }

    Kernel_start();
}

int main(void) {

    // initialize output ports
    init_output();

    // initialize USART
    usart_init();

    usart_send_line("Start Navilos");

    Kernel_init();

    // main loop
    while (1);
}

void User_task0(void)
{
	int a = 1;
	int b = 2;
	int c = 0;

	while (1)
	{
		usart_send_line("Task0 before context switch : ");
		usart_send(0x30 + a); usart_send(' '); usart_send(0x30 + b);
		usart_send_newline();

		c = a + b;

		Kernel_yield();

		usart_send_line("Task0 after context switch : ");
		usart_send(0x30 + c);
		usart_send_newline();

		a++;
		b++;
	}
}

void User_task1(void)
{
	int a = 3;
	int b = 4;
	int c = 0;

	while (1)
	{
		usart_send_line("Task1 before context switch : ");
		usart_send(0x30 + a); usart_send(' '); usart_send(0x30 + b);
		usart_send_newline();

		c = a + b;

		Kernel_yield();

		usart_send_line("Task1 after context switch : ");
		usart_send(0x30 + c);
		usart_send_newline();

		a++;
		b++;
	}
}

void User_task2(void)
{
	int a = 5;
	int b = 6;
	int c = 0;

	while (1)
	{
		usart_send_line("Task2 before context switch : ");
		usart_send(0x30 + a); usart_send(' '); usart_send(0x30 + b);
		usart_send_newline();

		c = a + b;

		Kernel_yield();

		usart_send_line("Task2 after context switch : ");
		usart_send(0x30 + c);
		usart_send_newline();

		a++;
		b++;
	}
}

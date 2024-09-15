#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "game1.h"
#include "game2.h"
#include "game3.h"
#include "game4.h"
#include "infos.h"
#include "menu.h"
#include "OLED1.h"
#include "OLED2.h"
/***************
STM32F103C8T6最小单片机的freertos移植实验
***************/

#define START_TASK_PRIO 1
#define START_STK_SIZE 256
TaskHandle_t StartTask_Handler;

#define menu_TASK_PRIO 2
#define menu_STK_SIZE 50
#define MUSIC0_TASK_PRIO 2
#define MUSIC0_STK_SIZE 50
#define GAME1_TASK_PRIO 2
#define GAME1_STK_SIZE 50
#define GAME2_TASK_PRIO 2
#define GAME2_STK_SIZE 50
#define GAME3_TASK_PRIO 2
#define GAME3_STK_SIZE 50
#define GAME4_TASK_PRIO 2
#define GAME4_STK_SIZE 50
#define INFO_TASK_PRIO 2
#define INFO_STK_SIZE 50
TaskHandle_t menuTask_Handler;
TaskHandle_t MUSIC0Task_Handler;
TaskHandle_t game1_Handler;
TaskHandle_t game2_Handler;
TaskHandle_t game3_Handler;
TaskHandle_t game4_Handler;
TaskHandle_t info_Handler;
void start_task(void *pvParameters);
void menu_task(void *pvParameters);
void music0_task(void *pvParameters);
void geme1_task(void *pvParameters);
void game2_task(void *pvParameters);
void game3_task(void *pvParameters);
void game4_task(void *pvParameters);
void info_task(void *pvParameters);
int menu_select_num;
TaskStatus_t game1_state;
TaskStatus_t game2_state;
TaskStatus_t game3_state;
TaskStatus_t game4_state;
TaskStatus_t info_state;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
	uart_init(115200);
	Key_Init();
	A_OLED_Init();
	Color_LED_Init();
	music_PWM_Init();
	music_PWM_SetCompare4(0);

	xTaskCreate((TaskFunction_t)start_task,
				(const char *)"start_task",
				(uint16_t)START_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)START_TASK_PRIO,
				(TaskHandle_t *)&StartTask_Handler);

	vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	//
	xTaskCreate((TaskFunction_t)menu_task,
				(const char *)"menu_task",
				(uint16_t)menu_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)menu_TASK_PRIO,
				(TaskHandle_t *)&menuTask_Handler);

	//	xTaskCreate((TaskFunction_t)music0_task,
	//				(const char *)"music0_task",
	//				(uint16_t)MUSIC0_STK_SIZE,
	//				(void *)NULL,
	//				(UBaseType_t)MUSIC0_TASK_PRIO,
	//				(TaskHandle_t *)&MUSIC0Task_Handler);

	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

void menu_task(void *pvParameters)
{
	while (1)
	{
		menu_select_num = menu();

		if (menu_select_num == 1)
		{
			vTaskGetInfo(game1_Handler, &game1_state, pdTRUE, eInvalid);
			if (game1_state.eCurrentState == eSuspended)
			{
				A_OLED_Clear();
				vTaskResume(game1_Handler);
			}
			else
			{												 // 使用了 FreeRTOS（Free Real-Time Operating System）中的 xTaskCreate 函数来创建一个新的任务
				xTaskCreate((TaskFunction_t)geme1_task,		 // 指向任务函数的指针
							(const char *)"geme1_task",		 // 任务的名称
							(uint16_t)GAME1_STK_SIZE,		 // 任务栈的大小
							(void *)NULL,					 // 传递给任务函数的参数
							(UBaseType_t)GAME1_TASK_PRIO,	 // 任务的优先级
							(TaskHandle_t *)&game1_Handler); // 一个指向任务句柄的指针
			}
		}
		else if (menu_select_num == 2)
		{
			vTaskGetInfo(game2_Handler, &game2_state, pdTRUE, eInvalid);
			if (game2_state.eCurrentState == eSuspended)
			{
				A_OLED_Clear();
				vTaskResume(game2_Handler);
			}
			else
			{
				xTaskCreate((TaskFunction_t)game2_task,
							(const char *)"game2_task",
							(uint16_t)GAME2_STK_SIZE,
							(void *)NULL,
							(UBaseType_t)GAME2_TASK_PRIO,
							(TaskHandle_t *)&game2_Handler);
			}
		}
		else if (menu_select_num == 3)
		{
			vTaskGetInfo(game3_Handler, &game3_state, pdTRUE, eInvalid);
			if (game3_state.eCurrentState == eSuspended)
			{
				A_OLED_Clear();
				vTaskResume(game3_Handler);
			}
			else
			{
				xTaskCreate((TaskFunction_t)game3_task,
							(const char *)"game3_task",
							(uint16_t)GAME2_STK_SIZE,
							(void *)NULL,
							(UBaseType_t)GAME2_TASK_PRIO,
							(TaskHandle_t *)&game3_Handler);
			}
		}
		else if (menu_select_num == 4)
		{
			vTaskGetInfo(game4_Handler, &game4_state, pdTRUE, eInvalid);
			if (game4_state.eCurrentState == eSuspended)
			{
				A_OLED_Clear();
				vTaskResume(game4_Handler); // 恢复任务
			}
			else
			{
				xTaskCreate((TaskFunction_t)game4_task,
							(const char *)"game4_task",
							(uint16_t)GAME2_STK_SIZE,
							(void *)NULL,
							(UBaseType_t)GAME2_TASK_PRIO,
							(TaskHandle_t *)&game4_Handler);
			}
		}
		else if (menu_select_num == 5)
		{
			vTaskGetInfo(info_Handler, &info_state, pdTRUE, eInvalid);
			if (game4_state.eCurrentState == eSuspended)
			{
				A_OLED_Clear();
				vTaskResume(info_Handler);
			}
			else
			{
				xTaskCreate((TaskFunction_t)info_task,
							(const char *)"info_task",
							(uint16_t)GAME2_STK_SIZE,
							(void *)NULL,
							(UBaseType_t)GAME2_TASK_PRIO,
							(TaskHandle_t *)&info_Handler);
			}
		}
		vTaskSuspend(menuTask_Handler);
	}
}
void music0_task(void *pvParameters)
{

	while (1)
	{
		MUSIC_Analysis();
	}
}
void geme1_task(void *pvParameters)
{
	while (1)
	{
		game1(); // 游戏1主函数
	}
}
void game2_task(void *pvParameters)
{
	while (1)
	{
		game2();
	}
}
void game3_task(void *pvParameters)
{
	while (1)
	{
		game3();
	}
}
void game4_task(void *pvParameters)
{
	while (1)
	{
		game4();
	}
}
void info_task(void *pvParameters)
{
	while (1)
	{
		infos();
	}
}
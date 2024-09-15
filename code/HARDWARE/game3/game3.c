/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * ����STM32��Ƭ���ķ����С��С��ϷFlappy Bird
  * ������ΪSTM32��Ƭ������������STM32F103C8T6��Ƭ���ϲ���ͨ����������STM32��Ƭ��ֻҪ�޸���������Ҳ����ʹ�á�
  * ��ϷЧ������ ��ע ΢�Ź��ںš�Bվ�������Ϳ��ֵ�ƽ̨���˺����ƣ�JL��Ƭ��������¼�Ƶ���Ƶ��Ч����
  * Ҳ����������������Ȥ�Ĵ��룬�������STM32��С������Ϸ������˹������Ϸ�ȣ����붼�Ѿ���Դ������Ȥ�Ŀ��Կ��¡�

		���ϣ�
		STM32F103C8T6��Сϵͳ��
		SSD1306 OLED128*64��ʾ��
		һ������


		IO����˵����

		OLED SCK---PA11
		OLED SDA---PA12
		����----PA7
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "OLED2.h"
#include "Key.h"
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern TaskHandle_t menuTask_Handler;
extern TaskHandle_t game3_Handler;

int x1;
int y1;
int x2;
int y2;
int bs;
int bird_y;
long game3_score;

unsigned char cover[] = {
	0x0,
	0x0,
	0xc,
	0x0,
	0x0,
	0x0,
	0x0,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x0,
	0x80,
	0x80,
	0x80,
	0x88,
	0x80,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xc,
	0x4,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x8,
	0x10,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x0,
	0x0,
	0x0,
	0x0,
	0x4,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x10,
	0x18,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x80,
	0x80,
	0x80,
	0x84,
	0x84,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xfe,
	0xff,
	0xff,
	0xff,
	0xff,
	0xf7,
	0xf7,
	0xf7,
	0xf7,
	0xf7,
	0xe7,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0x80,
	0xc0,
	0xe0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0x0,
	0xe0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xe0,
	0xc0,
	0xc0,
	0x80,
	0x80,
	0x0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xc0,
	0xc0,
	0xc0,
	0x80,
	0x0,
	0x0,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0x0,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xc7,
	0xff,
	0xff,
	0xff,
	0xff,
	0xfe,
	0x0,
	0x0,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0xf8,
	0x0,
	0x80,
	0x80,
	0xc0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0x0,
	0x80,
	0xc0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xf0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xc1,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xc1,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xdf,
	0xc1,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0x3f,
	0x7f,
	0xff,
	0xff,
	0xff,
	0xf0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xcf,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0xfe,
	0xfe,
	0xfe,
	0xfe,
	0xfe,
	0x0,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x3,
	0x3,
	0x1,
	0x1,
	0x1,
	0x1,
	0x0,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xc3,
	0xc3,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x0,
	0x0,
	0x0,
	0x1,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x7,
	0x7,
	0x0,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7,
	0x7,
	0x7,
	0x7,
	0x1,
	0x0,
	0x0,
	0x0,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7f,
	0x7,
	0x7,
	0x7,
	0x3,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7c,
	0x7c,
	0x7f,
	0x7f,
	0x7f,
	0x3f,
	0x3f,
	0x1f,
	0x0,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x3,
	0x1,
	0x0,
	0x0,
	0x0,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x0,
	0x1,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x0,
	0x0,
	0x0,
	0x80,
	0xc0,
	0x80,
	0x0,
	0x0,
	0x1,
	0x3,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x2,
	0x2,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1,
	0x3,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x20,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0x0,
	0x0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf0,
	0xf3,
	0xf3,
	0xcf,
	0xcf,
	0x3f,
	0x3f,
	0x3f,
	0xff,
	0xff,
	0xc0,
	0xc0,
	0x3f,
	0x3f,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xc3,
	0xc3,
	0xfc,
	0xfc,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x3,
	0x3,
	0x3,
	0xf,
	0xf,
	0xcf,
	0xcf,
	0xcf,
	0xcf,
	0xf3,
	0xf3,
	0xfc,
	0xfc,
	0xfc,
	0xff,
	0xff,
	0xcf,
	0xcf,
	0x33,
	0x33,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xcc,
	0xc,
	0xc,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x40,
	0xc0,
	0xc0,
	0xc0,
	0x40,
	0x0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xc0,
	0xc0,
	0x0,
	0x0,
	0xc0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0x80,
	0x40,
	0x40,
	0xc0,
	0x0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0xc0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0xc,
	0x38,
	0x60,
	0x80,
	0x60,
	0x38,
	0xc,
	0x0,
	0xfc,
	0x0,
	0x80,
	0x0,
	0xfc,
	0x84,
	0x84,
	0xfc,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xc,
	0x18,
	0x1f,
	0xf,
	0x0,
	0x0,
	0xf,
	0x8,
	0x8,
	0x8,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xe,
	0x7,
	0x3,
	0xe,
	0xe,
	0x3,
	0x7,
	0xc,
	0x0,
	0xf,
	0x8,
	0x8,
	0x8,
	0x0,
	0x0,
	0xf,
	0x8,
	0x8,
	0xc,
	0x7,
	0x1,
	0x0,
	0x0,
	0x0,
};

unsigned char game_over[] = {
	0xe0,
	0x18,
	0x1c,
	0xc6,
	0x62,
	0x32,
	0x1a,
	0xa,
	0xa,
	0xa,
	0xa,
	0xa,
	0xa,
	0xa,
	0xf2,
	0x2,
	0xfe,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xe0,
	0x20,
	0x10,
	0x90,
	0x90,
	0x90,
	0x90,
	0x90,
	0x90,
	0x90,
	0x10,
	0x30,
	0x20,
	0xc0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xe0,
	0x18,
	0xc,
	0xc6,
	0x22,
	0x12,
	0x12,
	0x12,
	0x12,
	0x12,
	0x12,
	0x12,
	0x22,
	0x66,
	0xc4,
	0x18,
	0xf0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0xc0,
	0x20,
	0x30,
	0x18,
	0x98,
	0x98,
	0x98,
	0x98,
	0x98,
	0x98,
	0x18,
	0x18,
	0x20,
	0xe0,
	0xc0,
	0x0,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0x80,
	0xff,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x1,
	0xf1,
	0x11,
	0x11,
	0x11,
	0x11,
	0xf1,
	0x0,
	0xc1,
	0x30,
	0xc,
	0x4,
	0x4,
	0x84,
	0x84,
	0x84,
	0x4,
	0x4,
	0x4,
	0xfc,
	0x0,
	0xfc,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x4,
	0x18,
	0x60,
	0xc1,
	0x1,
	0x1,
	0xf8,
	0x7,
	0x1,
	0x0,
	0xe0,
	0x10,
	0x10,
	0xe0,
	0x0,
	0xf,
	0x8,
	0xf8,
	0x1,
	0xfe,
	0xfc,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x1,
	0xfe,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0xfc,
	0x4,
	0x4,
	0x4,
	0xfc,
	0x0,
	0x0,
	0xfc,
	0x4,
	0x4,
	0x4,
	0xfc,
	0x0,
	0xf8,
	0xc,
	0xf,
	0x0,
	0x0,
	0xc0,
	0x20,
	0xc0,
	0x0,
	0x1,
	0xf,
	0xf8,
	0x1,
	0x1,
	0xe1,
	0x38,
	0xc,
	0x6,
	0x82,
	0x82,
	0x82,
	0x82,
	0x82,
	0xfe,
	0x0,
	0x0,
	0xff,
	0xff,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x80,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0x7f,
	0x40,
	0x40,
	0x7f,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x1,
	0x1,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x1,
	0x1,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0x3,
	0x84,
	0x46,
	0x43,
	0x40,
	0x40,
	0x40,
	0x7f,
	0x0,
	0xff,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0x7f,
	0x40,
	0x7f,
	0x0,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0x7f,
	0x40,
	0x40,
	0x7f,
	0x0,
	0x80,
	0xc0,
	0x7f,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7,
	0xc4,
	0x27,
	0x20,
	0x20,
	0x20,
	0x3f,
	0x0,
	0x0,
	0xff,
	0x0,
	0x0,
	0x0,
	0xff,
	0x0,
	0xf8,
	0xfc,
	0xc,
	0xc,
	0xc,
	0xc,
	0xf,
	0xf,
	0x18,
	0x30,
	0x63,
	0x44,
	0x48,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x58,
	0x4f,
	0x40,
	0x71,
	0x63,
	0x46,
	0x5c,
	0x58,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x5f,
	0x50,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x40,
	0x5f,
	0x58,
	0x50,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x40,
	0x5f,
	0x58,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x60,
	0x73,
	0x46,
	0x58,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x40,
	0x7f,
	0x3,
	0x3,
	0x3,
	0x3,
	0x3,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xf,
	0x18,
	0x30,
	0x63,
	0x46,
	0x4c,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x4c,
	0x62,
	0x61,
	0x40,
	0x4f,
	0x50,
	0x50,
	0x50,
	0x50,
	0x50,
	0x4c,
	0x46,
	0x63,
	0x30,
	0x18,
	0xc,
	0x18,
	0x20,
	0x43,
	0x46,
	0x4c,
	0x48,
	0x48,
	0x4f,
	0x40,
	0x7e,
	0x3,
	0x3,
	0x3f,
	0x40,
	0x40,
	0x5f,
	0x50,
	0x50,
	0x50,
	0x5f,
	0x40,
	0x7f,
	0x7f,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,

};

unsigned char image[] = {
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xe0,
	0xe0,
	0xf8,
	0x1c,
	0x18,
	0x1c,
	0x1c,
	0x1c,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0xe7,
	0xe7,
	0xe7,
	0x1f,
	0x1f,
	0x17,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x18,
	0x18,
	0xe0,
	0xe0,
	0xe0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0xfc,
	0xfc,
	0xff,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x7,
	0x18,
	0x18,
	0x18,
	0xe0,
	0xe0,
	0x0,
	0x0,
	0x0,
	0x1f,
	0x1f,
	0x1f,
	0xe0,
	0xe0,
	0xc0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1f,
	0x1f,
	0x0,
	0x0,
	0x0,
	0xff,
	0xff,
	0xff,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7,
	0x7,
	0x1f,
	0x38,
	0x38,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0x38,
	0x18,
	0x18,
	0x7,
	0x7,
	0x0,
	0x0,
	0x0,
	0xe0,
	0xe0,
	0xe0,
	0x38,
	0x38,
	0xff,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xe7,
	0xff,
	0x38,
	0x38,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x7,
	0x7,
	0x7,
	0x38,
	0x38,
	0x38,
	0x38,
	0x18,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe0,
	0xe7,
	0xe7,
	0xff,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x38,
	0x1c,
	0x7,
	0x7,
	0x3,
	0x0,
	0x0,

};
unsigned char bird[] = {
	0x18,
	0x64,
	0xa6,
	0x99,
	0x81,
	0xa7,
	0x71,
	0x55,
	0x56,
	0x7c,
	0x10,
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int get_button_status()
{
	if (Get_Key() == r_push_key_value)
		return 1;
	return 0;
}

void draw()
{
	B_OLED_Clear();
	B_OLED_ShowNum(80, 0, game3_score, 4, B_OLED_6X8);
	B_OLED_DrawRectangle(x1, 0, 10, y1, B_OLED_6X8);
	B_OLED_DrawRectangle(x1 - 2, y1 - 6, 14, 6, B_OLED_FILLED);
	B_OLED_DrawRectangle(x1, y1 + 20, 10, 64 - y1, B_OLED_FILLED);
	B_OLED_DrawRectangle(x1 - 2, y1 + 20, 14, 6, B_OLED_FILLED);
	B_OLED_DrawRectangle(x2, 0, 10, y2, B_OLED_FILLED);
	B_OLED_DrawRectangle(x2 - 2, y2 - 6, 14, 6, B_OLED_FILLED);
	B_OLED_DrawRectangle(x2, y2 + 20, 10, 64 - y2, B_OLED_FILLED);
	B_OLED_DrawRectangle(x2 - 2, y2 + 20, 14, 6, B_OLED_FILLED);
	B_OLED_ShowImage(20, bird_y, 11, 8, bird);
	B_OLED_Update();
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int game3(void)
{

	/* USER CODE BEGIN 1 */
	unsigned char is_fail = 0;
	x1 = 128;
	x2 = 192;
	y1 = rand() % 42 + 1;
	y2 = rand() % 42 + 1;

	bs = 32;
	bird_y = 10;
	game3_score = 0;
	/* USER CODE END 1 */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	B_OLED_ShowImage(0, 0, 128, 64, cover);
	B_OLED_Update();
	while (get_button_status() == 0)
	{
		if (get_key_status() == sw1_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskSuspend(game3_Handler);
			B_OLED_ShowImage(0, 0, 128, 64, cover);
			B_OLED_Update();
		}
		else if (get_key_status() == sw2_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskDelete(game3_Handler);
		}
	}
	while (1)
	{
		if (is_fail == 0)
		{

			if (get_button_status() == 1)
			{
				bs = 32;
			}

			x1 = x1 - 2;
			x2 = x2 - 2;
			bs = bs - 5;
			bird_y = bird_y - bs / 10;

			draw();

			if (bird_y < 0)
			{
				bird_y = 0;
			}
			if (bird_y > 64)
			{
				is_fail = 1;
			}

			if (x1 == -10)
			{
				x1 = 128;
				y1 = rand() % 42 + 1;
			}
			if (x2 == -10)
			{
				x2 = 128;
				y2 = rand() % 42 + 1;
			}
			if (x1 >= 20 && x1 <= 20 + 11)
			{
				if (bird_y < y1 || bird_y + 8 > y1 + 20)
				{
					is_fail = 1;
				}
			}
			if (x2 >= 20 && x2 <= 20 + 11)
			{
				if (bird_y < y2 || bird_y + 8 > y2 + 20)
				{
					is_fail = 1;
				}
			}

			if (x1 == 16 || x2 == 16)
			{
				game3_score++;
			}

			vTaskDelay(30);
		}

		if (is_fail == 1)
		{
			vTaskDelay(500);
			B_OLED_Clear();
			B_OLED_ShowImage(0, 0, 128, 32, game_over);
			B_OLED_ShowImage(8, 32, 45, 32, image);
			B_OLED_ShowString(72, 32, "game3_score", B_OLED_8X16);
			B_OLED_ShowNum(72, 48, game3_score, 4, B_OLED_8X16);

			B_OLED_Update();
			vTaskDelay(2000);
			while (get_button_status() == 0)
				;
			is_fail = 0;

			x1 = 128;
			x2 = 192;

			y1 = rand() % 42 + 1;
			y2 = rand() % 42 + 1;

			bs = 32;
			bird_y = 50;
			game3_score = 0;
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (get_key_status() == sw1_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskSuspend(game3_Handler);
		}
		else if (get_key_status() == sw2_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskDelete(game3_Handler);
		}
	}
	/* USER CODE END 3 */
}
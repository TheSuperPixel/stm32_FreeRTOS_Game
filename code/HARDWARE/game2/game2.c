/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * ����51��Ƭ���������սС���?
  * ��ֲ�ԣ�https://www.youtube.com/watch?v=lOz_GuME63E
  * ������ΪSTM32��Ƭ������������STM32F103C8T6��Ƭ���ϲ���ͨ����������STM32��Ƭ��ֻҪ�޸���������Ҳ����ʹ�á�
  * ��ϷЧ������ ��ע ΢�Ź��ںš�Bվ�������Ϳ��ֵ�ƽ̨���˺����ƣ�JL��Ƭ��������¼�Ƶ���Ƶ��Ч����
  * Ҳ����������������Ȥ�Ĵ��룬����Ȥ�Ŀ��Կ��¡�
  * ���ϣ�
  * STM32F103C8T6��Сϵͳ��
  * SSD1306 B_OLED128*64��ʾ��
  * 3������

  * IO����˵����
  * ���ϰ���---PA4
  * ���°���---PC14
  * ��������/�������?---PA2

  * B_OLED SCK---PB12
  * B_OLED SDA---PA15

  * Ч����ʾ��Ƶ���ӣ�https://www.bilibili.com/video/BV1Tz421Y7cH/
	* Powered by JL��Ƭ��
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

#include "stm32f10x.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED2.H"
#include "Key.H"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
extern TaskHandle_t menuTask_Handler;
extern TaskHandle_t game2_Handler;
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
// void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

const unsigned char dioda16[32] =
	{
		0x40,
		0x40,
		0xC2,
		0x6A,
		0xFF,
		0xE5,
		0x0F,
		0x7A,
		0xEA,
		0xF2,
		0x62,
		0x20,
		0xE0,
		0x20,
		0x40,
		0x80,
		0x02,
		0x02,
		0x43,
		0x56,
		0xFF,
		0xA7,
		0xF0,
		0x5E,
		0x57,
		0x4F,
		0x46,
		0x04,
		0x07,
		0x04,
		0x02,
		0x01,
};

int metx = 0;
int mety = 0;
int postoji = 0;
int nep = 8;
int smjer = 0;
int go = 0;
int rx = 95;
int ry = 0;
int rx2 = 95;
int ry2 = 0;
int rx3 = 95;
int ry3 = 0;
int bodovi = 0;

int brzina = 3;
int bkugle = 1;
int najmanja = 600;
int najveca = 1200;
int promjer = 10;

int rx4 = 95;
int ry4 = 0;
int zivoti = 5;
int poc = 0;
int ispaljeno = 0;
int nivo = 1;
int centar = 95;
unsigned char key_num = 0;
unsigned long pocetno = 0;
unsigned long odabrano = 0;
unsigned long trenutno = 0;
unsigned long nivovrije = 0;
int poz = 30;

void tone(int a1, int a2, int a3)
{
}
void ponovo()
{
	metx = 0;
	mety = 0;
	postoji = 0;
	nep = 8;
	smjer = 0;
	go = 0;
	rx = 95;
	ry = 0;
	rx2 = 95;
	ry2 = 0;
	rx3 = 95;
	ry3 = 0;
	bodovi = 0;

	brzina = 3;
	bkugle = 1;
	najmanja = 600;
	najveca = 1200;
	promjer = 12;

	rx4 = 95;
	ry4 = 0;
	zivoti = 5;
	poc = 0;
	ispaljeno = 0;
	nivo = 1;
	pocetno = 0;
	odabrano = 0;
	trenutno = 0;
	nivovrije = 0;
}

int get_key_status()
{
	return Get_Key();
}

int game2_init()
{
	metx = 0;
	mety = 0;
	postoji = 0;
	nep = 8;
	smjer = 0;
	go = 0;
	rx = 95;
	ry = 0;
	rx2 = 95;
	ry2 = 0;
	rx3 = 95;
	ry3 = 0;
	bodovi = 0;

	brzina = 3;
	bkugle = 1;
	najmanja = 600;
	najveca = 1200;
	promjer = 10;

	rx4 = 95;
	ry4 = 0;
	zivoti = 5;
	poc = 0;
	ispaljeno = 0;
	nivo = 1;
	centar = 95;
	key_num = 0;
	pocetno = 0;
	odabrano = 0;
	trenutno = 0;
	nivovrije = 0;
	poz = 30;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int game2(void)
{
	B_OLED_Clear();
	game2_init();
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	int pozicija = 0;
	unsigned long start_tt = 0;
	unsigned long dt = 0;

	// B_OLED_Init();

	while (get_key_status() == 0)
		;

	start_tt = xTaskGetTickCount();
	while (1)
	{
		unsigned long st = xTaskGetTickCount();
		if (go == 0)
		{
			B_OLED_Clear();

			B_OLED_DrawPoint(50, 30);
			B_OLED_DrawPoint(30, 17);
			B_OLED_DrawPoint(60, 18);
			B_OLED_DrawPoint(55, 16);
			B_OLED_DrawPoint(25, 43);
			B_OLED_DrawPoint(100, 43);
			B_OLED_DrawPoint(117, 52);
			B_OLED_DrawPoint(14, 49);
			B_OLED_DrawPoint(24, 24);
			B_OLED_DrawPoint(78, 36);
			B_OLED_DrawPoint(80, 57);
			B_OLED_DrawPoint(107, 11);
			B_OLED_DrawPoint(150, 11);
			B_OLED_DrawPoint(5, 5);
			B_OLED_DrawPoint(8, 7);
			B_OLED_DrawPoint(70, 12);
			B_OLED_DrawPoint(10, 56);
			B_OLED_DrawPoint(70, 25);

			if (poc == 0)
			{
				pocetno = xTaskGetTickCount() - start_tt;
				odabrano = rand() % 800 + 400;
				poc = 1;
			}
			trenutno = xTaskGetTickCount() - start_tt;

			if ((trenutno - nivovrije) > 50000)
			{
				nivovrije = trenutno;
				nivo = nivo + 1;

				brzina = brzina + 1;
				if (nivo % 2 == 0)
				{
					bkugle = bkugle + 1;
					promjer = promjer - 1;
				}
				najmanja = najmanja - 50;
				najveca = najveca - 50;
			}

			if ((odabrano + pocetno) < trenutno)
			{
				poc = 0;
				ispaljeno = ispaljeno + 1;
				if (ispaljeno == 1)
				{
					rx = 95;
					ry = nep;
				}
				if (ispaljeno == 2)
				{
					rx2 = 95;
					ry2 = nep;
				}
				if (ispaljeno == 3)
				{
					rx3 = 95;
					ry3 = nep;
				}

				if (ispaljeno == 4)
				{
					rx4 = 95;
					ry4 = nep;
				}
			}

			if (ispaljeno > 0)
			{
				B_OLED_DrawCircle(rx, ry, 2, 0);
				rx = rx - brzina;
			}

			if (ispaljeno > 1)
			{
				B_OLED_DrawCircle(rx2, ry2, 1, 0);
				rx2 = rx2 - brzina;
			}

			if (ispaljeno > 2)
			{
				B_OLED_DrawCircle(rx3, ry3, 4, 0);
				rx3 = rx3 - brzina;
			}

			if (ispaljeno > 3)
			{
				B_OLED_DrawCircle(rx4, ry4, 2, 0);
				rx4 = rx4 - brzina;
			}

			if (get_key_status() == r_front_key_value && poz >= 2)
			{
				poz = poz - 2;
			}

			if (get_key_status() == r_back_key_value && poz <= 46)
			{
				poz = poz + 2;
			}

			if (get_key_status() == r_push_key_value && postoji == 0)
			{
				postoji = 1;
				metx = 6;
				mety = poz + 8;
				tone(9, 1200, 20);
			}
			if (postoji == 1)
			{
				metx = metx + 8;

				B_OLED_DrawLine(metx, mety, metx + 4, mety);
			}

			B_OLED_ShowImage(4, poz, 16, 16, dioda16); //  Plane
			B_OLED_DrawCircle(centar, nep, promjer, 0);
			B_OLED_DrawCircle(centar + 2, nep + 3, promjer / 3, 1);

			B_OLED_ShowString(33, 57, "score:", B_OLED_6X8);
			B_OLED_ShowNum(68, 57, bodovi, 3, B_OLED_6X8);

			B_OLED_ShowString(33, 0, "lives:", B_OLED_6X8);
			B_OLED_ShowNum(68, 0, zivoti, 1, B_OLED_6X8);

			B_OLED_ShowString(110, 0, "L:", B_OLED_6X8);
			B_OLED_ShowNum(122, 0, nivo, 1, B_OLED_6X8);
			B_OLED_ShowNum(108, 57, trenutno / 1000, 3, B_OLED_6X8);

			B_OLED_Update();

			if (metx > 128)
				postoji = 0;

			if (smjer == 0)
			{
				nep = nep + bkugle;
			}
			else
			{
				nep = nep - bkugle;
			}

			if (nep >= (64 - promjer))
				smjer = 1;
			if (nep <= promjer)
				smjer = 0;

			if (mety >= nep - promjer && mety <= nep + promjer)
				if (metx > (centar - promjer) && metx < (centar + promjer))
				{
					metx = -20;
					tone(9, 500, 20);
					bodovi = bodovi + 1;
					postoji = 0;
				}

			pozicija = poz + 8;
			if (ry >= pozicija - 8 && ry <= pozicija + 8)
				if (rx < 12 && rx > 4)
				{
					rx = 95;
					ry = -50;
					tone(9, 100, 100);
					zivoti = zivoti - 1;
				}

			if (ry2 >= pozicija - 8 && ry2 <= pozicija + 8)
				if (rx2 < 12 && rx2 > 4)
				{
					rx2 = -50;
					ry2 = -50;
					tone(9, 100, 100);
					zivoti = zivoti - 1;
				}

			if (ry3 >= pozicija - 8 && ry3 <= pozicija + 8)
				if (rx3 < 12 && rx3 > 4)
				{
					rx3 = -50;
					ry3 = -50;
					tone(9, 100, 100);
					zivoti = zivoti - 1;
				}

			if (ry4 >= pozicija - 8 && ry4 <= pozicija + 8)
				if (rx4 < 12 && rx4 > 4)
				{
					rx4 = 200;
					ry4 = -50;
					ispaljeno = 0;
					tone(9, 100, 100);
					zivoti = zivoti - 1;
				}

			if (rx4 < 1)
			{
				ispaljeno = 0;
				rx4 = 200;
			}

			if (zivoti == 0)
				go = 1;

			dt = xTaskGetTickCount() - st;
			if (dt < 30)
			{
				vTaskDelay(30 - dt);
			}
		}

		if (go == 1)
		{

			B_OLED_Clear();

			B_OLED_ShowString(7, 10, "Game Over!", B_OLED_8X16);

			B_OLED_ShowString(7, 30, "score:", B_OLED_6X8);
			B_OLED_ShowNum(44, 30, bodovi, 3, B_OLED_6X8);

			B_OLED_ShowString(7, 40, "level:", B_OLED_6X8);
			B_OLED_ShowNum(44, 40, nivo, 1, B_OLED_6X8);
			B_OLED_ShowString(7, 50, "time(s):", B_OLED_6X8);
			B_OLED_ShowNum(60, 50, trenutno / 1000, 3, B_OLED_6X8);

			B_OLED_Update();
			if (zivoti == 0)
			{
				zivoti = 5;
				vTaskDelay(1000);
			}

			if (get_key_status() == r_front_key_value)
			{
				start_tt = xTaskGetTickCount(); // one bug! HAL_GetTick();
				ponovo();
			}
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (get_key_status() == sw1_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskSuspend(game2_Handler);
		}
		else if (get_key_status() == sw2_key_value)
		{
			vTaskResume(menuTask_Handler);
			vTaskDelete(game2_Handler);
		}
	}
	/* USER CODE END 3 */
}

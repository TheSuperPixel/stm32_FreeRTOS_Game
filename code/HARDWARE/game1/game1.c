#include "game1.h"
#include "stm32f10x.h"
#include "stdint.h"
#include "OLED1.H"
#include "Key.H"
#include "Delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "music_pwm.h"
#include "Music.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
extern TaskHandle_t MUSIC0Task_Handler;
extern TaskHandle_t game1_Handler;
extern TaskHandle_t menuTask_Handler;
extern void music0_task(void *pvParameters);
#define START_TASK_PRIO 1
#define MUSIC0_TASK_PRIO 2
#define MUSIC0_STK_SIZE 50
int16_t obstacle_pos = 128; // 障碍物位置
uint16_t Hs;                // 最高分
uint16_t score;             // 当前分数
uint16_t obstacle_ran;      // 随机植物长度
uint16_t height;            // 恐龙跳跃高度
uint8_t Menufalg;           // 菜单标志位
uint8_t MenuSelect;         // 菜单选择位
uint8_t MenuShow;           // 菜单显示标志位
uint8_t reset;              // 重置标志位
uint8_t obstacle_length;    // 植物长度
uint8_t bmpflag;            // 显示图片标志位
uint8_t playflag;           // 游戏、菜单标志位
uint8_t menuclear;          // 菜单清屏
uint8_t ClearF;             // 清屏标志位
uint8_t beingspeed;         // 游戏起始速度
uint8_t mode;               // 菜单模式
uint8_t Key4;               // 计KeyNum=4的按下次数
uint8_t speedmax = 36;      // 速度最大值
uint8_t FMQswitch;          // 蜂鸣器开关
uint8_t LEDswitch = 1;      // 彩灯开关
uint8_t KeyNum;             // 按键值
uint8_t t;                  // 进入次数
uint8_t roll;
uint8_t Speed;        // 游戏速度
uint8_t deadflag;     // 死亡标志位
uint8_t begingtemp;   // 开始速度中间变量
uint8_t hard_val;     // 难度
uint8_t speedtep = 9; // 最大速度中间变量
uint8_t Settlement;   // 结算动画标志位
int led_cnt = 0;
int led_val;
void Buzzer_Init() // 蜂鸣器初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Color_LED_Init() // 彩灯初始化
{
    GPIO_InitTypeDef GPIO_InitStructure1;
    GPIO_InitTypeDef GPIO_InitStructure2;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure1);

    GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure2);
}
void Color_LED(unsigned int color) // 彩灯控制
{
    if (color == 0)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14); // 红色
        GPIO_SetBits(GPIOB, GPIO_Pin_15); // 绿色
        GPIO_SetBits(GPIOA, GPIO_Pin_8);  // 蓝色
    }
    if (color == 1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 红色
        GPIO_SetBits(GPIOB, GPIO_Pin_15);   // 绿色
        GPIO_SetBits(GPIOA, GPIO_Pin_8);    // 蓝色
    }
    if (color == 2)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);   // 红色
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // 绿色
        GPIO_SetBits(GPIOA, GPIO_Pin_8);    // 蓝色
    }
    if (color == 3)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 红色
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // 绿色
        GPIO_SetBits(GPIOA, GPIO_Pin_8);    // 蓝色
    }
    if (color == 4)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);  // 红色
        GPIO_SetBits(GPIOB, GPIO_Pin_15);  // 绿色
        GPIO_ResetBits(GPIOA, GPIO_Pin_8); // 蓝色
    }
    if (color == 5)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 红色
        GPIO_SetBits(GPIOB, GPIO_Pin_15);   // 绿色
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // 蓝色
    }
    if (color == 6)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_14);   // 红色
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // 绿色
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // 蓝色
    }
    if (color == 7)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 红色
        GPIO_ResetBits(GPIOB, GPIO_Pin_15); // 绿色
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // 蓝色
    }
}
void Buzzer(unsigned int SW) // 蜂鸣器控制
{
    if (SW)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_11);
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
    }
}
void bostacle_refresh(void) // 随机障碍
{
    obstacle_pos = obstacle(obstacle_ran, 0);
    if (obstacle_pos + obstacle_length <= 0)
    {
        if (hard_val == 0)
        {
            obstacle_ran = rand() % 4;
        }
        else if (hard_val == 1)
        {
            obstacle_ran = 0;
        }
        else if (hard_val == 2)
        {
            obstacle_ran = 1;
        }
        else if (hard_val == 3)
        {
            obstacle_ran = 2;
        }

        obstacle_pos = obstacle(obstacle_ran, 1);
    }
    switch (obstacle_ran) // 随机三种不同宽度障碍
    {
    case 0:
        obstacle_length = 8;
        break;
    case 1:
        obstacle_length = 16;
        break;
    default:
        obstacle_length = 24;
        break;
    }
}

void Show_other(void) // 显示云、地面、分数
{
    A_OLED_ShowString(1, 14, "SC:", 1, 0);
    A_OLED_ShowString(1, 0, "HS:", 1, 0);
    score++;
    //    if (score % 100 == 0)
    //        Buzzer(1);
    //    else
    //        Buzzer(0);
    A_OLED_ShowDainamic8x128(7, roll);
    A_OLED_ShowCloud();
    A_OLED_ShowNum(1, 17, score, 5, 1);
    A_OLED_ShowNum(1, 4, Hs, 5, 1);
}
void loop_dead(void) // 检测恐龙是否撞到障碍
{
    if ((height <= 16) && ((obstacle_pos >= 16 && obstacle_pos <= 32) || (obstacle_pos + obstacle_length >= 16 && obstacle_pos + obstacle_length <= 32)))
        deadflag = 1;
    if (height <= 0)
        KeyNum = Key();
    if (KeyNum == r_back_key_value || height > 0)
    {
        height = dinojump(0);
    }
    else
        dinorun();
}

const uint8_t close[] =
    {
        3, 2, 1, 1, 1, 1, 2, 3};

void A_OLED_DeadAnimation(unsigned int newbest) // 结算动画页面
{
    uint8_t i, j;
    if (Key() == r_push_key_value)
        reset = 4;
    if (t == 0)
    {
        if (!Settlement)
        {
            for (i = 0; i < 8; i++)
            {
                A_OLED_SetCursor(i, 0);
                for (j = 0; j < 128; j++)
                {
                    A_OLED_Write_Data(0xFF);
                    vTaskDelay(close[i]);
                }
            }
        }
        A_OLED_Clear();
        if (newbest == 0)
        {
            A_OLED_ShowChinese(2, 1, 2, 0);
            A_OLED_ShowChinese(2, 2, 3, 0);
            A_OLED_ShowChinese(2, 3, 4, 0);
            A_OLED_ShowString(2, 7, "...", 3, 0);
        }
        else
        {
            A_OLED_ShowChinese(2, 1, 7, 0);
            A_OLED_ShowChinese(2, 2, 8, 0);
            A_OLED_ShowChinese(2, 3, 9, 0);
            A_OLED_ShowString(2, 7, "!!!", 3, 0);
        }

        A_OLED_ShowString(7, 2, "SC:", 1, 0);
        A_OLED_ShowNum(7, 5, score, 5, 1);
        A_OLED_ShowDino(0, 70);
        t++;
    }
}
void live(void) // 游戏页面
{
    if (LEDswitch)
    {
        Color_LED(2);
    }
    else
    {
        Color_LED(0);
    }
    bostacle_refresh();
    loop_dead();
    Show_other();
    Speed = (score / 30) + beingspeed;
    if (Speed >= speedmax)
        Speed = speedmax;
    vTaskDelay(37 - Speed);
}
void die(void) // 碰撞障碍后
{
    if (LEDswitch)
    {
        Color_LED(1);
    }
    else
    {
        Color_LED(0);
    }
    uint8_t nowbest = 0;
    if (score > Hs)
        nowbest = 1;
    else
        nowbest = 0;
    A_OLED_DeadAnimation(nowbest);
    if (reset == 4)
    {
        deadflag = 0;
        height = 0;
        obstacle(obstacle_ran, 1);
        height = dinojump(1);
        t = 0;
        reset = 0;
        A_OLED_Clear();
        if (nowbest)
            Hs = score;
        score = 0;
    }
}

void Menu1(void) // 彩灯开关
{
    A_OLED_ShowMenuChinese(1, 1, 0, 0);
    A_OLED_ShowMenuChinese(1, 2, 1, 0);
    A_OLED_ShowMenuChinese(1, 3, 2, 0);
    A_OLED_ShowMenuChinese(1, 4, 3, 0);
    if (LEDswitch == 0)
    {
        A_OLED_ShowString(3, 7, "OFF", 3, 0);
    }
    else
    {
        A_OLED_ShowString(3, 7, "ON ", 3, 0);
    }
    if (KeyNum == r_back_key_value)
    {
        LEDswitch++;
        LEDswitch %= 2;
    }
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }
        A_OLED_Clear();
    }
}
void Menu2(void) // 设置初始速度
{
    A_OLED_ShowMenuChinese(1, 1, 4, 0);
    A_OLED_ShowMenuChinese(1, 2, 5, 0);
    A_OLED_ShowMenuChinese(1, 3, 6, 0);
    A_OLED_ShowMenuChinese(1, 4, 7, 0); // 初始速度
    A_OLED_ShowNum(3, 8, begingtemp, 1, 3);
    if (KeyNum == r_front_key_value)
    {
        begingtemp++;
        begingtemp %= 10;
        beingspeed = begingtemp * 3;
    };
    if (KeyNum == r_back_key_value)
    {
        if (begingtemp == 0)
        {
            begingtemp = 9;
        }
        else
        {
            begingtemp--;
        }

        beingspeed = begingtemp * 3;
    };
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }
        A_OLED_Clear();
    }
}
void Menu3(void) // 设置速度上限
{
    A_OLED_ShowMenuChinese(1, 1, 8, 0);
    A_OLED_ShowMenuChinese(1, 2, 9, 0);
    A_OLED_ShowMenuChinese(1, 3, 10, 0);
    A_OLED_ShowMenuChinese(1, 4, 11, 0);
    A_OLED_ShowNum(3, 8, speedtep, 1, 3);
    if (KeyNum == r_front_key_value)
    {
        speedtep++;
        if (speedtep == 9)
        {
            speedtep = 1;
        }
        speedmax = speedtep * 4;
    }
    if (KeyNum == r_back_key_value)
    {
        speedtep--;
        if (speedtep <= 1)
            speedtep = 9;
        speedmax = speedtep * 4;
    }
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }
        A_OLED_Clear();
    }
}
void Menu4(void) // 蜂鸣器开关
{
    A_OLED_ShowMenuChinese(1, 1, 12, 0);
    A_OLED_ShowMenuChinese(1, 2, 13, 0);
    A_OLED_ShowMenuChinese(1, 3, 14, 0);
    A_OLED_ShowMenuChinese(1, 4, 15, 0);
    A_OLED_ShowMenuChinese(1, 5, 16, 0);
    if (FMQswitch == 0)
    {
        A_OLED_ShowString(3, 7, "ON ", 3, 0);
    }
    else
        A_OLED_ShowString(3, 7, "OFF ", 3, 0);
    if (KeyNum == r_back_key_value)
    {
        FMQswitch++;
        FMQswitch %= 2;
    }
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }

        if (FMQswitch == 0)
        {
            if (MUSIC0Task_Handler == NULL)
            {
                xTaskCreate((TaskFunction_t)music0_task,
                            (const char *)"music0_task",
                            (uint16_t)MUSIC0_STK_SIZE,
                            (void *)NULL,
                            (UBaseType_t)MUSIC0_TASK_PRIO,
                            (TaskHandle_t *)&MUSIC0Task_Handler);
                music_PWM_SetCompare4(50);
            }
        }
        else
        {
            if (MUSIC0Task_Handler != NULL)
            {
                vTaskDelete(MUSIC0Task_Handler);
                MUSIC0Task_Handler = NULL;
                music_PWM_SetCompare4(0);
            }
        }
        A_OLED_Clear();
    }
}
void Menu5(void) // 结算动画开关
{
    A_OLED_ShowMenuChinese(1, 1, 17, 0);
    A_OLED_ShowMenuChinese(1, 2, 18, 0);
    A_OLED_ShowMenuChinese(1, 3, 19, 0);
    A_OLED_ShowMenuChinese(1, 4, 20, 0);
    A_OLED_ShowMenuChinese(1, 5, 21, 0);
    A_OLED_ShowMenuChinese(1, 6, 22, 0);
    if (FMQswitch == 0)
    {
        A_OLED_ShowString(3, 7, "ON ", 3, 0);
    }
    else
    {
        A_OLED_ShowString(3, 7, "OFF", 3, 0);
        Settlement = ~Settlement;
    }
    if (KeyNum == r_back_key_value)
    {
        FMQswitch++;
        FMQswitch %= 2;
    }
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }
        A_OLED_Clear();
    }
}
void Menu6(void) // 难度
{
    A_OLED_ShowMenuChinese(1, 1, 23, 0);
    A_OLED_ShowMenuChinese(1, 2, 24, 0); // 难度
    A_OLED_ShowNum(3, 8, hard_val, 1, 3);
    if (KeyNum == r_front_key_value)
    {
        hard_val++;
        if (hard_val == 4)
        {
            hard_val = 0;
        }
    };
    if (KeyNum == r_back_key_value)
    {
        hard_val--;
        if (hard_val > 3)
        {
            hard_val = 3;
        }
    };
    if (KeyNum == r_push_key_value || KeyNum == r_left_key_value)
    {
        Key4++;
        if (Key4 > 1)
        {
            mode = 7;
            Key4 = 0;
        }
        A_OLED_Clear();
    }
}

void Menu(void) // 菜单
{
    KeyNum = Key();

    if (KeyNum == r_left_key_value)
    {
        playflag = 0;
        bmpflag = 0;
    }
    if (menuclear == 0)
    {
        A_OLED_Clear();
        menuclear++;
    }

    if (!MenuShow)
    {
        if (KeyNum == r_back_key_value)
        {
            MenuSelect++;
            MenuSelect %= 6;
            A_OLED_Clear();
        }
        if (KeyNum == r_front_key_value)
        {
            MenuSelect--;
            if (MenuSelect > 5)
            {
                MenuSelect = 5;
            }
            MenuSelect %= 6;
            A_OLED_Clear();
        }
        A_OLED_ShowChinese(1, 4, 0, 0);
        A_OLED_ShowChinese(1, 5, 1, 0);
        if (LEDswitch)
        {
            led_cnt++;
            if (led_cnt > 100)
            {
                led_cnt = 0;
                led_val = rand() % 6;
            }

            Color_LED(led_val + 1);
        }
        else
        {
            Color_LED(0);
        }
        switch (MenuSelect)
        {
        case 0:
            if (MenuSelect == 0)
            {
                A_OLED_ShowString(2, 1, "1.", 3, 1);
                A_OLED_ShowMenuChinese(2, 2, 0, 1);
                A_OLED_ShowMenuChinese(2, 3, 1, 1);
                A_OLED_ShowMenuChinese(2, 4, 2, 1);
                A_OLED_ShowMenuChinese(2, 5, 3, 1); // 彩灯开关
                A_OLED_ShowString(3, 1, "2.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 4, 0);
                A_OLED_ShowMenuChinese(3, 3, 5, 0);
                A_OLED_ShowMenuChinese(3, 4, 6, 0);
                A_OLED_ShowMenuChinese(3, 5, 7, 0); // 初始速度
                A_OLED_ShowString(4, 1, "3.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 8, 0);
                A_OLED_ShowMenuChinese(4, 3, 9, 0);
                A_OLED_ShowMenuChinese(4, 4, 10, 0);
                A_OLED_ShowMenuChinese(4, 5, 11, 0); // 速度上限
            }
            else
            {
                A_OLED_ShowString(2, 1, "1.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 0, 0);
                A_OLED_ShowMenuChinese(2, 3, 1, 0);
                A_OLED_ShowMenuChinese(2, 4, 2, 0);
                A_OLED_ShowMenuChinese(2, 5, 3, 0);
            }
            break; // 返回游戏}
        case 1:
            if (MenuSelect == 1)
            {
                A_OLED_ShowString(2, 1, "1.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 0, 0);
                A_OLED_ShowMenuChinese(2, 3, 1, 0);
                A_OLED_ShowMenuChinese(2, 4, 2, 0);
                A_OLED_ShowMenuChinese(2, 5, 3, 0); // 彩灯开关
                A_OLED_ShowString(3, 1, "2.", 3, 1);
                A_OLED_ShowMenuChinese(3, 2, 4, 1);
                A_OLED_ShowMenuChinese(3, 3, 5, 1);
                A_OLED_ShowMenuChinese(3, 4, 6, 1);
                A_OLED_ShowMenuChinese(3, 5, 7, 1); // 初始速度
                A_OLED_ShowString(4, 1, "3.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 8, 0);
                A_OLED_ShowMenuChinese(4, 3, 9, 0);
                A_OLED_ShowMenuChinese(4, 4, 10, 0);
                A_OLED_ShowMenuChinese(4, 5, 11, 0); // 速度上限
            }
            else
            {
                A_OLED_ShowString(3, 1, "2.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 4, 0);
                A_OLED_ShowMenuChinese(3, 3, 5, 0);
                A_OLED_ShowMenuChinese(3, 4, 6, 0);
                A_OLED_ShowMenuChinese(3, 5, 7, 0);
            }
            break; // 返回游戏}
        case 2:
            if (MenuSelect == 2)
            {
                A_OLED_ShowString(2, 1, "1.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 0, 0);
                A_OLED_ShowMenuChinese(2, 3, 1, 0);
                A_OLED_ShowMenuChinese(2, 4, 2, 0);
                A_OLED_ShowMenuChinese(2, 5, 3, 0); // 彩灯开关
                A_OLED_ShowString(3, 1, "2.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 4, 0);
                A_OLED_ShowMenuChinese(3, 3, 5, 0);
                A_OLED_ShowMenuChinese(3, 4, 6, 0);
                A_OLED_ShowMenuChinese(3, 5, 7, 0); // 初始速度
                A_OLED_ShowString(4, 1, "3.", 3, 1);
                A_OLED_ShowMenuChinese(4, 2, 8, 1);
                A_OLED_ShowMenuChinese(4, 3, 9, 1);
                A_OLED_ShowMenuChinese(4, 4, 10, 1);
                A_OLED_ShowMenuChinese(4, 5, 11, 1); // 速度上限
            }
            else
            {
                A_OLED_ShowString(4, 1, "3.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 8, 0);
                A_OLED_ShowMenuChinese(4, 3, 9, 0);
                A_OLED_ShowMenuChinese(4, 4, 10, 0);
                A_OLED_ShowMenuChinese(4, 5, 11, 0);
            };
            break; // 返回游戏}
        case 3:
            if (MenuSelect == 3)
            {
                A_OLED_ShowString(2, 1, "4.", 3, 1);
                A_OLED_ShowMenuChinese(2, 2, 12, 1);
                A_OLED_ShowMenuChinese(2, 3, 13, 1);
                A_OLED_ShowMenuChinese(2, 4, 14, 1);
                A_OLED_ShowMenuChinese(2, 5, 15, 1);
                A_OLED_ShowMenuChinese(2, 6, 16, 1); // 蜂鸣器开关
                A_OLED_ShowString(3, 1, "5.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 17, 0);
                A_OLED_ShowMenuChinese(3, 3, 18, 0);
                A_OLED_ShowMenuChinese(3, 4, 19, 0);
                A_OLED_ShowMenuChinese(3, 5, 20, 0);
                A_OLED_ShowMenuChinese(3, 6, 21, 0);
                A_OLED_ShowMenuChinese(3, 7, 22, 0); // 结算动画开关
                A_OLED_ShowString(4, 1, "6.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 23, 0);
                A_OLED_ShowMenuChinese(4, 3, 24, 0); // 难度
            }
            else
            {
                A_OLED_ShowString(2, 1, "4.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 12, 0);
                A_OLED_ShowMenuChinese(2, 3, 13, 0);
                A_OLED_ShowMenuChinese(2, 2, 14, 0);
                A_OLED_ShowMenuChinese(2, 3, 15, 1);
                A_OLED_ShowMenuChinese(2, 3, 16, 0);
            }
            break;
        case 4:
            if (MenuSelect == 4)
            {
                A_OLED_ShowString(2, 1, "4.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 12, 0);
                A_OLED_ShowMenuChinese(2, 3, 13, 0);
                A_OLED_ShowMenuChinese(2, 4, 14, 0);
                A_OLED_ShowMenuChinese(2, 5, 15, 0);
                A_OLED_ShowMenuChinese(2, 6, 16, 0); // 蜂鸣器开关
                A_OLED_ShowString(3, 1, "5.", 3, 1);
                A_OLED_ShowMenuChinese(3, 2, 17, 1);
                A_OLED_ShowMenuChinese(3, 3, 18, 1);
                A_OLED_ShowMenuChinese(3, 4, 19, 1);
                A_OLED_ShowMenuChinese(3, 5, 20, 1);
                A_OLED_ShowMenuChinese(3, 6, 21, 1);
                A_OLED_ShowMenuChinese(3, 7, 22, 1); // 结算动画开关
                A_OLED_ShowString(4, 1, "6.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 23, 0);
                A_OLED_ShowMenuChinese(4, 3, 24, 0); // 难度
            }
            else
            {
                A_OLED_ShowString(3, 1, "5.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 17, 0);
                A_OLED_ShowMenuChinese(3, 3, 18, 0);
                A_OLED_ShowMenuChinese(3, 4, 19, 0);
                A_OLED_ShowMenuChinese(3, 5, 20, 0);
                A_OLED_ShowMenuChinese(3, 6, 21, 0);
                A_OLED_ShowMenuChinese(3, 7, 22, 0);
            }
            break; // 结算动画开关
        case 5:
            if (MenuSelect == 5)
            {
                A_OLED_ShowString(2, 1, "4.", 3, 0);
                A_OLED_ShowMenuChinese(2, 2, 12, 0);
                A_OLED_ShowMenuChinese(2, 3, 13, 0);
                A_OLED_ShowMenuChinese(2, 4, 14, 0);
                A_OLED_ShowMenuChinese(2, 5, 15, 0);
                A_OLED_ShowMenuChinese(2, 6, 16, 0); // 蜂鸣器开关
                A_OLED_ShowString(3, 1, "5.", 3, 0);
                A_OLED_ShowMenuChinese(3, 2, 17, 0);
                A_OLED_ShowMenuChinese(3, 3, 18, 0);
                A_OLED_ShowMenuChinese(3, 4, 19, 0);
                A_OLED_ShowMenuChinese(3, 5, 20, 0);
                A_OLED_ShowMenuChinese(3, 6, 21, 0);
                A_OLED_ShowMenuChinese(3, 7, 22, 0); // 结算动画开关
                A_OLED_ShowString(4, 1, "6.", 3, 1);
                A_OLED_ShowMenuChinese(4, 2, 23, 1);
                A_OLED_ShowMenuChinese(4, 3, 24, 1); // 难度
            }
            else
            {
                A_OLED_ShowString(4, 1, "6.", 3, 0);
                A_OLED_ShowMenuChinese(4, 2, 23, 0);
                A_OLED_ShowMenuChinese(4, 3, 24, 0);
            }
            break; // 难度
        }
    }
    if (KeyNum == r_push_key_value)
    {
        if (ClearF == 0)
        {
            ClearF++;
            A_OLED_Clear();
        }
        switch (MenuSelect)
        {
        case 0:
            mode = 1;
            break;
        case 1:
            mode = 2;
            break;
        case 2:
            mode = 3;
            break;
        case 3:
            mode = 4;
            break;
        case 4:
            mode = 5;
            break;
        case 5:
            mode = 6;
            break;
        }
    }
    switch (mode)
    {
    case 1:
        Menu1();
        MenuShow = 1;
        break;
    case 2:
        Menu2();
        MenuShow = 1;
        break;
    case 3:
        Menu3();
        MenuShow = 1;
        break;
    case 4:
        Menu4();
        MenuShow = 1;
        break;
    case 5:
        Menu5();
        MenuShow = 1;
        break;
    case 6:
        Menu6();
        MenuShow = 1;
        break;
    case 7:
        MenuShow = 0;
        break;
    default:
        break;
    }
}
void init_data()
{
    obstacle_pos = 128;  // 障碍物位置
    Hs = 0;              // 最高分
    score = 0;           // 当前分数
    obstacle_ran = 0;    // 随机植物长度
    height = 0;          // 恐龙跳跃高度
    Menufalg = 0;        // 菜单标志位
    MenuSelect = 0;      // 菜单选择位
    MenuShow = 0;        // 菜单显示标志位
    reset = 0;           // 重置标志位
    obstacle_length = 0; // 植物长度
    bmpflag = 0;         // 显示图片标志位
    playflag = 0;        // 游戏、菜单标志位
    menuclear = 0;       // 菜单清屏
    ClearF = 0;          // 清屏标志位
    beingspeed = 0;      // 游戏起始速度
    mode = 0;            // 菜单模式
    Key4 = 0;            // 计KeyNum=4的按下次数
    speedmax = 36;       // 速度最大值
    FMQswitch = 0;       // 蜂鸣器开关
    LEDswitch = 1;       // 彩灯开关
    KeyNum = 0;          // 按键值
    t = 0;               // 进入次数
    roll = 0;
    Speed = 0;      // 游戏速度
    deadflag = 0;   // 死亡标志位
    begingtemp = 0; // 开始速度中间变量
    hard_val = 0;   // 难度
    speedtep = 9;   // 最大速度中间变量
    Settlement = 0; // 结算动画标志位
}
int game1(void)
{
    A_OLED_Clear();
    //    Buzzer_Init();
    init_data();
    // vTaskDelay(10);
    // KeyNum = 0;
    // vTaskDelay(1000);
    // end:
    while (1)
    {
        if (playflag == 0)
        {
            if (bmpflag == 0)
            {
                if (LEDswitch)
                {
                    Color_LED(4);
                }
                else
                {
                    Color_LED(0);
                }

                A_OLED_ShowBMP();
                bmpflag++;
            }
            KeyNum = Key();
            if (KeyNum == r_push_key_value)
            {
                playflag = 1;
                A_OLED_Clear();
            }
            if (KeyNum == r_left_key_value)
            {
                playflag = 2;
                A_OLED_Clear();
            }
        }
        else if (playflag == 1)
        {
            if (!deadflag)
            {
                live();
            }
            else
            {
                die();
                // playflag = 0;
                // bmpflag = 0;
                // goto end;
            }
        }
        else if (playflag == 2)
        {
            Menu();
        }
        if (KeyNum == sw1_key_value)
        {
            vTaskResume(menuTask_Handler); // 恢复菜单任务线程
            vTaskSuspend(game1_Handler);   // 暂停游戏任务线程
        }
        else if (KeyNum == sw2_key_value)
        {
            vTaskResume(menuTask_Handler); // 恢复菜单任务线程
            vTaskDelete(game1_Handler);    // 删除游戏任务线程
        }
    }
}

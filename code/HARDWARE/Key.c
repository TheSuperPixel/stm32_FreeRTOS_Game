#include "stm32f10x.h" // Device header
#include "key.h"
uint8_t Key_KeyNumber;
//**定时器扫描按键**//

void GPIO_KeyInit(void) // 初始化GPIO
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitTypeDef GPIO_InitStructure3;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = key_r_back_pin | key_r_push_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(key_r_back_and_push_port, &GPIO_InitStructure);

	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure2.GPIO_Pin = key_l_left_pin | key_l_right_pin | key_l_front_pin | key_l_back_pin | key_l_push_pin | key_r_left_pin | key_r_right_pin | key_r_front_pin;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(key_l_all_AND_right_left_right_front_port, &GPIO_InitStructure2);

	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure3.GPIO_Pin = sw1_pin | sw2_pin;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(sw_port, &GPIO_InitStructure3);
}

void Timer_Init(void) // 初始化定时器
{
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_InternalClockConfig(TIM2);

	TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period = 300 - 1;
	TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TimeBaseInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

uint8_t Key(void) // 获取按键值
{
	uint8_t Temp;
	Temp = Key_KeyNumber;
	Key_KeyNumber = 0;
	return Temp;
}

uint8_t Get_Key(void) // 获取电平变化
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(key_r_back_and_push_port, key_r_back_pin) == 0)
	{
		KeyNum = r_back_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_r_left_pin) == 0)
	{
		KeyNum = r_left_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_r_right_pin) == 0)
	{
		KeyNum = r_right_key_value;
	}
	if (GPIO_ReadInputDataBit(key_r_back_and_push_port, key_r_push_pin) == 0)
	{
		KeyNum = r_push_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_r_front_pin) == 0)
	{
		KeyNum = r_front_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_l_back_pin) == 0)
	{
		KeyNum = l_back_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_l_left_pin) == 0)
	{
		KeyNum = l_left_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_l_right_pin) == 0)
	{
		KeyNum = l_right_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_l_push_pin) == 0)
	{
		KeyNum = l_push_key_value;
	}
	if (GPIO_ReadInputDataBit(key_l_all_AND_right_left_right_front_port, key_l_front_pin) == 0)
	{
		KeyNum = l_front_key_value;
	}
	if (GPIO_ReadInputDataBit(sw_port, sw1_pin) == 0)
	{
		KeyNum = sw1_key_value;
	}
	if (GPIO_ReadInputDataBit(sw_port, sw2_pin) == 0)
	{
		KeyNum = sw2_key_value;
	}
	return KeyNum;
}

void Loop_Key(void) // 扫描按键
{
	static uint8_t LastKey = 0, NowKey = 0;
	LastKey = NowKey;
	NowKey = Get_Key();
	if (LastKey == r_front_key_value && NowKey == 0)
	{
		Key_KeyNumber = r_front_key_value;
	}
	if (LastKey == r_back_key_value && NowKey == 0)
	{
		Key_KeyNumber = r_back_key_value;
	}
	if (LastKey == r_left_key_value && NowKey == 0)
	{
		Key_KeyNumber = r_left_key_value;
	}
	if (LastKey == r_right_key_value && NowKey == 0)
	{
		Key_KeyNumber = r_right_key_value;
	}
	if (LastKey == r_push_key_value && NowKey == 0)
	{
		Key_KeyNumber = r_push_key_value;
	}
	if (LastKey == l_front_key_value && NowKey == 0)
	{
		Key_KeyNumber = l_front_key_value;
	}
	if (LastKey == l_back_key_value && NowKey == 0)
	{
		Key_KeyNumber = l_back_key_value;
	}
	if (LastKey == l_left_key_value && NowKey == 0)
	{
		Key_KeyNumber = l_left_key_value;
	}
	if (LastKey == l_right_key_value && NowKey == 0)
	{
		Key_KeyNumber = l_right_key_value;
	}
	if (LastKey == l_push_key_value && NowKey == 0)
	{
		Key_KeyNumber = l_push_key_value;
	}
	if (LastKey == sw1_key_value && NowKey == 0)
	{
		Key_KeyNumber = sw1_key_value;
	}
	if (LastKey == sw2_key_value && NowKey == 0)
	{
		Key_KeyNumber = sw2_key_value;
	}
}

void Key_Init(void) // 按键初始化
{
	GPIO_KeyInit();
	Timer_Init();
}

void TIM2_IRQHandler(void) // 定时器扫描（30ms）
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == 1)
	{
		Loop_Key();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

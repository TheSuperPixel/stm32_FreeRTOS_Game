#include "stm32f10x.h" // Device header
#include "music_pwm.h"
#include "Delay.h" 
void music_PWM_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM1);

    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 71-1;    // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71-1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // CCR
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void music_PWM_SetCompare4(unsigned int Compare)
{
    TIM_SetCompare4(TIM1, Compare);
}
void music_PWM_SetPSC(unsigned int freq)
{
		unsigned int psc=1000000 / freq - 1; 
	
    TIM_Cmd(TIM1,DISABLE);
		vTaskDelay(1);
		
		TIM_PrescalerConfig(TIM1,psc-1,TIM_PSCReloadMode_Immediate);
		TIM_Cmd(TIM1,ENABLE);
}

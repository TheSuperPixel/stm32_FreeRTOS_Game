#ifndef _Key__H
#define _Key__H

#define key_l_all_AND_right_left_right_front_port GPIOA
#define key_l_left_pin GPIO_Pin_0
#define key_l_right_pin GPIO_Pin_1
#define key_l_front_pin GPIO_Pin_2
#define key_l_back_pin GPIO_Pin_3
#define key_l_push_pin GPIO_Pin_4

#define key_r_left_pin GPIO_Pin_5
#define key_r_right_pin GPIO_Pin_6
#define key_r_front_pin GPIO_Pin_7
#define key_r_back_and_push_port GPIOB
#define key_r_back_pin GPIO_Pin_0
#define key_r_push_pin GPIO_Pin_1

#define sw_port GPIOB
#define sw1_pin GPIO_Pin_12
#define sw2_pin GPIO_Pin_13

#define l_front_key_value 1
#define l_back_key_value 2
#define l_left_key_value 3
#define l_right_key_value 4
#define l_push_key_value 5
#define r_front_key_value 6
#define r_back_key_value 7
#define r_left_key_value 8
#define r_right_key_value 9
#define r_push_key_value 10
#define sw1_key_value 11
#define sw2_key_value 12
void GPIO_KeyInit(void);
void Timer_Init(void);
uint8_t Key(void);
uint8_t Get_Key(void);
void Loop_Key(void);
void Key_Init(void);
void TIM2_IRQHandler(void);

#endif

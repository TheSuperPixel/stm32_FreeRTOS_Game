/*
 * OLED.h
 *
 *  Created on: 2024年5月31日
 *      Author: hejunlin
 */

#ifndef INC_OLED2_H_
#define INC_OLED2_H_

#include <stdint.h>
#include "OLED_Data2.h"
#include "stm32f10x.h"

/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define B_OLED_8X16				8
#define B_OLED_6X8				6

/*IsFilled参数数值*/
#define B_OLED_UNFILLED			0
#define B_OLED_FILLED				1

#define scl_Pin GPIO_Pin_8
#define scl_GPIO_Port GPIOB
#define sda_Pin GPIO_Pin_9
#define sda_GPIO_Port GPIOB
/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void B_OLED_Init(void);

/*更新函数*/
void B_OLED_Update(void);
void B_OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void B_OLED_Clear(void);
void B_OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void B_OLED_Reverse(void);
void B_OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void B_OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void B_OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void B_OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void B_OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void B_OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void B_OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void B_OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void B_OLED_ShowChinese(int16_t X, int16_t Y, char *Chinese);
void B_OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void B_OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void B_OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t B_OLED_GetPoint(int16_t X, int16_t Y);
void B_OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void B_OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void B_OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void B_OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void B_OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void B_OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif /* INC_OLED_H_ */

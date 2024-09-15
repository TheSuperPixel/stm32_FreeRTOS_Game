#ifndef _OLED1__H
#define _OLED1__H
#include "stdint.h"
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

void A_OLED_GPIO_Init(void);
void A_OLED_I2C_Start(void);
void A_OLED_I2C_Stop(void);
void A_OLED_I2C_SendByte(uint8_t Byte);
void A_OLED_Write_Command(uint8_t Command);
void A_OLED_Write_Data(uint8_t Data);
void A_OLED_SetCursor(uint8_t y,uint8_t x);
void A_OLED_Clear(void);
void A_OLED_Fill(void);
void A_OLED_Init(void);
void A_OLED_OPEN(void);
void A_OLED_OFF(void);
void A_OLED_Char6x8(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void A_OLED_Char6x12(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void A_OLED_Char8x16(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void A_OLED_Char12x24(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void A_OLED_6x8String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void A_OLED_6x12String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void A_OLED_8x16String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void A_OLED_12x24String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void A_OLED_ShowString(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t size,uint8_t mode);
void A_OLED_ShowChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode);
void A_OLED_ShowDainamic8x128(uint8_t Line,uint8_t n);
uint32_t A_OLED_Pow(uint32_t X, uint32_t Y);
void A_OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length,uint8_t size);
void A_OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void A_OLED_ShowCloud(void);
void dinorun(void);
int dinojump(char reset);
void clear_jump(void);
int obstacle(uint8_t ver,uint8_t reset);
void A_OLED_ShowDino(uint8_t Line,uint8_t Column);
void A_OLED_ShowBMP(void);
void A_OLED_ShowMenuChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode);
#endif

/*
 * OLED_Data.h
 *
 *  Created on: 2024年5月31日
 *      Author: hejunlin
 */

#ifndef INC_OLED_DATA2_H_
#define INC_OLED_DATA2_H_

#include <stdint.h>

/*中文字符字节宽度*/
#define B_OLED_CHN_CHAR_WIDTH			2		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct
{
	char Index[B_OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
	uint8_t Data[32];						//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t B_OLED_F8x16[][16];
extern const uint8_t B_OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t B_OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...

#endif /* INC_OLED_DATA_H_ */

/*
 * OLED.c
 *
 *  Created on: 2024年5月31日
 *      Author: hejunlin
 */

#include "OLED2.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"
#include "Delay.h"
/**
  * 数据存储格式：
  * 纵向8点，高位在下，先从左到右，再从上到下
  * 每一个Bit对应一个像素点
  *
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  *
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  *
  *       0             X轴           127
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  *
  */


/*全局变量*********************/

/**
  * OLED显存数组
  * 所有的显示函数，都只是对此显存数组进行读写
  * 随后调用OLED_Update函数或OLED_UpdateArea函数
  * 才会将显存数组的数据发送到OLED硬件，进行显示
  */
uint8_t B_OLED_DisplayBuf[8][128] = {
	0x00,0x00,0x7E,0x00,0x40,0x00,0x3C,0x42,0x42,0x3C,0x00,0x00,0x00,0x00,0x00,0x80,
	0xC0,0xC0,0x60,0x20,0x30,0x10,0x18,0x18,0x0C,0x7F,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x7F,0x08,0x18,0x18,0x10,0x30,0x30,0x60,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xE0,
	0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0x00,0x30,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
	0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0xE0,0xF0,0xF0,0xF0,0xF0,
	0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
	0xF0,0xF0,0xF0,0xE0,0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0x1C,0x0E,0x03,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0E,0x1C,0xF0,
	0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x3F,
	0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xC7,0x87,0x00,0x00,0x07,0x07,0x07,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x0F,0x07,0x07,0x07,0x00,0x80,0xFC,0xFF,0xFF,0xFF,0x7F,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xC7,0xE7,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFE,0xFF,0xF8,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,
	0xFF,0xFC,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,
	0x1C,0x1C,0x1F,0x1F,0x1F,0x0F,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x01,0x1F,0x1F,
	0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x18,0x1F,0x1F,0x1F,0x1F,0x1E,0x1E,0x0F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1C,0x00,0x07,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
	0x07,0x0F,0x1F,0x3F,0x3F,0x1E,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x78,0xFE,0x07,0x01,0x00,0x01,0x03,0x07,0xC6,0xF4,0xF4,
	0xE4,0xE4,0xEC,0xE8,0xE8,0xE8,0xC8,0xC8,0xC8,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,
	0xD8,0xD8,0xC8,0xE8,0xE8,0xE8,0xE8,0xE8,0xEC,0xE4,0xE4,0xE4,0xF6,0x46,0x03,0x03,
	0x01,0x00,0x01,0x83,0xFE,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x8F,0x00,0x00,0xCF,0xFF,0xFF,0xFF,0xFF,0xFF,0x8F,0x00,
	0x00,0xCF,0xFF,0xFF,0xFF,0x1F,0x0F,0x00,0x80,0xC0,0xC0,0xE0,0xE0,0x60,0x60,0x60,
	0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x00,0x00,0x00,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x80,
	0xC0,0xC0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0xFE,0x1C,0x00,0x00,0x08,0x11,0x27,0x43,
	0x83,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0x0F,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x83,0x43,0x27,0x10,0x08,0x00,
	0x00,0x3C,0xFE,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x1F,0xFF,0xFF,0xFF,0xFC,0xFE,0x7F,0x1F,0x07,0x1F,0x7F,0xFF,0xFF,0xFC,
	0xFE,0xFF,0x1F,0x03,0x00,0x00,0x00,0x00,0x38,0x7C,0xFE,0xFE,0xFE,0xEE,0xC6,0x46,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xE7,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xC0,0xF0,0x3C,0x07,0x33,0x78,0x70,0x68,0x10,0x20,0x20,0x40,
	0x41,0x32,0x08,0x40,0x40,0x20,0x30,0x10,0x18,0x18,0x0C,0x0C,0x0F,0x0F,0x0F,0x0C,
	0x1C,0x18,0x18,0x30,0x20,0x20,0x40,0x40,0x1C,0x23,0x41,0x40,0x20,0x30,0x10,0x28,
	0x70,0x78,0x37,0x0F,0x3C,0xF0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0x0F,0x38,0x30,0x60,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xE0,0xE0,0xE0,0xC0,
	0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xC0,0xC0,0x60,0x30,0x1C,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0xF8,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x78,0xF8,0x80,0x00,0x00,
	0x00,0x80,0xF0,0x78,0xF8,0xF8,0x00,0x00,0x00,0xC0,0xF0,0x70,0x38,0x18,0x18,0x08,
	0x08,0x00,0x00,0x00,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0C,0x18,0x30,0x30,
	0x60,0x66,0x4F,0x4F,0x5F,0x5F,0x4F,0x4F,0x63,0x60,0x30,0x1C,0x0C,0x05,0x04,0x1C,
	0x30,0x60,0x67,0x4F,0x4F,0x4F,0x5F,0x4F,0x4F,0x66,0x60,0x30,0x30,0x18,0x0C,0x07,
	0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x30,0x30,0x38,0x1F,0x0F,0x07,0x00,0x00,0x3F,0x3F,0x3F,0x30,0x30,
	0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x00,0x01,0x1F,0x3E,0x3C,
	0x3E,0x0F,0x01,0x00,0x3F,0x3F,0x00,0x00,0x00,0x07,0x1F,0x1C,0x38,0x30,0x30,0x30,
	0x30,0x00,0x00,0x00,0x0F,0x1F,0x3F,0x30,0x30,0x30,0x30,0x38,0x0F,0x00,0x00,0x00,
};

/*********************全局变量*/


/*引脚配置*********************/

/**
  * 函    数：B_OLED写SCL高低电平
  * 参    数：要写入SCL的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SCL时，此函数会被调用
  *           用户需要根据参数传入的值，将SCL置为高电平或者低电平
  *           当参数传入0时，置SCL为低电平，当参数传入1时，置SCL为高电平
  */
void B_OLED_W_SCL(uint8_t BitValue)
{
	/*根据BitValue的值，将SCL置高电平或者低电平*/
	//GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);
	GPIO_WriteBit(scl_GPIO_Port, scl_Pin, BitValue);
	//HAL_Delay(2);

	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
}

/**
  * 函    数：B_OLED写SDA高低电平
  * 参    数：要写入SDA的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写SDA时，此函数会被调用
  *           用户需要根据参数传入的值，将SDA置为高电平或者低电平
  *           当参数传入0时，置SDA为低电平，当参数传入1时，置SDA为高电平
  */
void B_OLED_W_SDA(uint8_t BitValue)
{
	/*根据BitValue的值，将SDA置高电平或者低电平*/
	//GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);
	GPIO_WriteBit(sda_GPIO_Port, sda_Pin, BitValue);
	//HAL_Delay(2);
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
}

/**
  * 函    数：B_OLED引脚初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：当上层函数需要初始化时，此函数会被调用
  *           用户需要将SCL和SDA引脚初始化为开漏模式，并释放引脚
  */
void B_OLED_GPIO_Init(void)
{


	/*在初始化前，加入适量延时，待B_OLED供电稳定*/
	Delay_us(500);
	/*将SCL和SDA引脚初始化为开漏模式*/
	/*
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	/*释放SCL和SDA*/
	B_OLED_W_SCL(1);
	B_OLED_W_SDA(1);
}

/*********************引脚配置*/


/*通信协议*********************/

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void B_OLED_I2C_Start(void)
{
	B_OLED_W_SDA(1);		//释放SDA，确保SDA为高电平
	B_OLED_W_SCL(1);		//释放SCL，确保SCL为高电平
	B_OLED_W_SDA(0);		//在SCL高电平期间，拉低SDA，产生起始信号
	B_OLED_W_SCL(0);		//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void B_OLED_I2C_Stop(void)
{
	B_OLED_W_SDA(0);		//拉低SDA，确保SDA为低电平
	B_OLED_W_SCL(1);		//释放SCL，使SCL呈现高电平
	B_OLED_W_SDA(1);		//在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void B_OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;

	/*循环8次，主机依次发送数据的每一位*/
	for (i = 0; i < 8; i++)
	{
		/*使用掩码的方式取出Byte的指定一位数据并写入到SDA线*/
		/*两个!的作用是，让所有非零的值变为1*/
		B_OLED_W_SDA(!!(Byte & (0x80 >> i)));
		B_OLED_W_SCL(1);	//释放SCL，从机在SCL高电平期间读取SDA
		B_OLED_W_SCL(0);	//拉低SCL，主机开始发送下一位数据
	}

	B_OLED_W_SCL(1);		//额外的一个时钟，不处理应答信号
	B_OLED_W_SCL(0);
}

/**
  * 函    数：B_OLED写命令
  * 参    数：Command 要写入的命令值，范围：0x00~0xFF
  * 返 回 值：无
  */
void B_OLED_WriteCommand(uint8_t Command)
{
	B_OLED_I2C_Start();				//I2C起始
	B_OLED_I2C_SendByte(0x78);		//发送B_OLED的I2C从机地址
	B_OLED_I2C_SendByte(0x00);		//控制字节，给0x00，表示即将写命令
	B_OLED_I2C_SendByte(Command);		//写入指定的命令
	B_OLED_I2C_Stop();				//I2C终止
}

/**
  * 函    数：B_OLED写数据
  * 参    数：Data 要写入数据的起始地址
  * 参    数：Count 要写入数据的数量
  * 返 回 值：无
  */
void B_OLED_WriteData(uint8_t *Data, uint8_t Count)
{
	uint8_t i;

	B_OLED_I2C_Start();				//I2C起始
	B_OLED_I2C_SendByte(0x78);		//发送B_OLED的I2C从机地址
	B_OLED_I2C_SendByte(0x40);		//控制字节，给0x40，表示即将写数据
	/*循环Count次，进行连续的数据写入*/
	for (i = 0; i < Count; i ++)
	{
		B_OLED_I2C_SendByte(Data[i]);	//依次发送Data的每一个数据
	}
	B_OLED_I2C_Stop();				//I2C终止
}

/*********************通信协议*/


/*硬件配置*********************/

/**
  * 函    数：B_OLED初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用前，需要调用此初始化函数
  */
void B_OLED_Init(void)
{
	B_OLED_GPIO_Init();			//先调用底层的端口初始化

	/*写入一系列的命令，对B_OLED进行初始化配置*/
	B_OLED_WriteCommand(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启

	B_OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	B_OLED_WriteCommand(0x80);	//0x00~0xFF

	B_OLED_WriteCommand(0xA8);	//设置多路复用率
	B_OLED_WriteCommand(0x3F);	//0x0E~0x3F

	B_OLED_WriteCommand(0xD3);	//设置显示偏移
	B_OLED_WriteCommand(0x00);	//0x00~0x7F

	B_OLED_WriteCommand(0x40);	//设置显示开始行，0x40~0x7F

	B_OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置

	B_OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置

	B_OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	B_OLED_WriteCommand(0x12);

	B_OLED_WriteCommand(0x81);	//设置对比度
	B_OLED_WriteCommand(0xCF);	//0x00~0xFF

	B_OLED_WriteCommand(0xD9);	//设置预充电周期
	B_OLED_WriteCommand(0xF1);

	B_OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	B_OLED_WriteCommand(0x30);

	B_OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	B_OLED_WriteCommand(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色

	B_OLED_WriteCommand(0x8D);	//设置充电泵
	B_OLED_WriteCommand(0x14);

	B_OLED_WriteCommand(0xAF);	//开启显示

	//B_OLED_Clear();				//清空显存数组
	B_OLED_Update();				//更新显示，清屏，防止初始化后未显示内容时花屏
}

/**
  * 函    数：B_OLED设置显示光标位置
  * 参    数：Page 指定光标所在的页，范围：0~7
  * 参    数：X 指定光标所在的X轴坐标，范围：0~127
  * 返 回 值：无
  * 说    明：B_OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
  */
void B_OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*如果使用此程序驱动1.3寸的B_OLED显示屏，则需要解除此注释*/
	/*因为1.3寸的B_OLED驱动芯片（SH1106）有132列*/
	/*屏幕的起始列接在了第2列，而不是第0列*/
	/*所以需要将X加2，才能正常显示*/
//	X += 2;

	/*通过指令设置页地址和列地址*/
	B_OLED_WriteCommand(0xB0 | Page);					//设置页位置
	B_OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	B_OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/*********************硬件配置*/


/*工具函数*********************/

/*工具函数仅供内部部分函数使用*/

/**
  * 函    数：次方函数
  * 参    数：X 底数
  * 参    数：Y 指数
  * 返 回 值：等于X的Y次方
  */
uint32_t B_OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//结果默认为1
	while (Y --)			//累乘Y次
	{
		Result *= X;		//每次把X累乘到结果上
	}
	return Result;
}

/**
  * 函    数：判断指定点是否在指定多边形内部
  * 参    数：nvert 多边形的顶点数
  * 参    数：vertx verty 包含多边形顶点的x和y坐标的数组
  * 参    数：testx testy 测试点的X和y坐标
  * 返 回 值：指定点是否在指定多边形内部，1：在内部，0：不在内部
  */
uint8_t B_OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i, j, c = 0;

	/*此算法由W. Randolph Franklin提出*/
	/*参考链接：https://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
  * 函    数：判断指定点是否在指定角度内部
  * 参    数：X Y 指定点的坐标
  * 参    数：StartAngle EndAngle 起始角度和终止角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * 返 回 值：指定点是否在指定角度内部，1：在内部，0：不在内部
  */
uint8_t B_OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//计算指定点的弧度，并转换为角度表示
	if (StartAngle < EndAngle)	//起始角度小于终止角度的情况
	{
		/*如果指定角度在起始终止角度之间，则判定指定点在指定角度*/
		if (PointAngle >= StartAngle && PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	else			//起始角度大于于终止角度的情况
	{
		/*如果指定角度大于起始角度或者小于终止角度，则判定指定点在指定角度*/
		if (PointAngle >= StartAngle || PointAngle <= EndAngle)
		{
			return 1;
		}
	}
	return 0;		//不满足以上条件，则判断判定指定点不在指定角度
}

/*********************工具函数*/


/*功能函数*********************/

/**
  * 函    数：将B_OLED显存数组更新到B_OLED屏幕
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有的显示函数，都只是对B_OLED显存数组进行读写
  *           随后调用B_OLED_Update函数或B_OLED_UpdateArea函数
  *           才会将显存数组的数据发送到B_OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_Update(void)
{
	uint8_t j;
	/*遍历每一页*/
	for (j = 0; j < 8; j ++)
	{
		/*设置光标位置为每一页的第一列*/
		B_OLED_SetCursor(j, 0);
		/*连续写入128个数据，将显存数组的数据写入到B_OLED硬件*/
		B_OLED_WriteData(B_OLED_DisplayBuf[j], 128);
	}
}

/**
  * 函    数：将B_OLED显存数组部分更新到B_OLED屏幕
  * 参    数：X 指定区域左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定区域左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Width 指定区域的宽度，范围：0~128
  * 参    数：Height 指定区域的高度，范围：0~64
  * 返 回 值：无
  * 说    明：此函数会至少更新参数指定的区域
  *           如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
  * 说    明：所有的显示函数，都只是对B_OLED显存数组进行读写
  *           随后调用B_OLED_Update函数或B_OLED_UpdateArea函数
  *           才会将显存数组的数据发送到B_OLED硬件，进行显示
  *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t j;
	int16_t Page, Page1;

	/*负数坐标在计算页地址时需要加一个偏移*/
	/*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
	Page = Y / 8;
	Page1 = (Y + Height - 1) / 8 + 1;
	if (Y < 0)
	{
		Page -= 1;
		Page1 -= 1;
	}

	/*遍历指定区域涉及的相关页*/
	for (j = Page; j < Page1; j ++)
	{
		if (X >= 0 && X <= 127 && j >= 0 && j <= 7)		//超出屏幕的内容不显示
		{
			/*设置光标位置为相关页的指定列*/
			B_OLED_SetCursor(j, X);
			/*连续写入Width个数据，将显存数组的数据写入到B_OLED硬件*/
			B_OLED_WriteData(&B_OLED_DisplayBuf[j][X], Width);
		}
	}
}

/**
  * 函    数：将B_OLED显存数组全部清零
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_Clear(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//遍历8页
	{
		for (i = 0; i < 128; i ++)			//遍历128列
		{
			B_OLED_DisplayBuf[j][i] = 0x00;	//将显存数组数据全部清零
		}
	}
}

/**
  * 函    数：将B_OLED显存数组部分清零
  * 参    数：X 指定区域左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定区域左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Width 指定区域的宽度，范围：0~128
  * 参    数：Height 指定区域的高度，范围：0~64
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t i, j;

	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			if (i >= 0 && i <= 127 && j >=0 && j <= 63)				//超出屏幕的内容不显示
			{
				B_OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
			}
		}
	}
}

/**
  * 函    数：将B_OLED显存数组全部取反
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_Reverse(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//遍历8页
	{
		for (i = 0; i < 128; i ++)			//遍历128列
		{
			B_OLED_DisplayBuf[j][i] ^= 0xFF;	//将显存数组数据全部取反
		}
	}
}

/**
  * 函    数：将B_OLED显存数组部分取反
  * 参    数：X 指定区域左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定区域左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Width 指定区域的宽度，范围：0~128
  * 参    数：Height 指定区域的高度，范围：0~64
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t i, j;

	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			if (i >= 0 && i <= 127 && j >=0 && j <= 63)			//超出屏幕的内容不显示
			{
				B_OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
			}
		}
	}
}

/**
  * 函    数：B_OLED显示一个字符
  * 参    数：X 指定字符左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定字符左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Char 指定要显示的字符，范围：ASCII码可见字符
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize)
{
	if (FontSize == B_OLED_8X16)		//字体为宽8像素，高16像素
	{
		/*将ASCII字模库B_OLED_F8x16的指定数据以8*16的图像格式显示*/
		B_OLED_ShowImage(X, Y, 8, 16, B_OLED_F8x16[Char - ' ']);
	}
	else if(FontSize == B_OLED_6X8)	//字体为宽6像素，高8像素
	{
		/*将ASCII字模库B_OLED_F6x8的指定数据以6*8的图像格式显示*/
		B_OLED_ShowImage(X, Y, 6, 8, B_OLED_F6x8[Char - ' ']);
	}
}

/**
  * 函    数：B_OLED显示字符串
  * 参    数：X 指定字符串左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定字符串左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：String 指定要显示的字符串，范围：ASCII码可见字符组成的字符串
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)		//遍历字符串的每个字符
	{
		/*调用B_OLED_ShowChar函数，依次显示每个字符*/
		B_OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
	}
}

/**
  * 函    数：B_OLED显示数字（十进制，正整数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：0~4294967295
  * 参    数：Length 指定数字的长度，范围：0~10
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//遍历数字的每一位
	{
		/*调用B_OLED_ShowChar函数，依次显示每个数字*/
		/*Number / B_OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
		/*+ '0' 可将数字转换为字符格式*/
		B_OLED_ShowChar(X + i * FontSize, Y, Number / B_OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

/**
  * 函    数：B_OLED显示有符号数字（十进制，整数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：-2147483648~2147483647
  * 参    数：Length 指定数字的长度，范围：0~10
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	uint32_t Number1;

	if (Number >= 0)						//数字大于等于0
	{
		B_OLED_ShowChar(X, Y, '+', FontSize);	//显示+号
		Number1 = Number;					//Number1直接等于Number
	}
	else									//数字小于0
	{
		B_OLED_ShowChar(X, Y, '-', FontSize);	//显示-号
		Number1 = -Number;					//Number1等于Number取负
	}

	for (i = 0; i < Length; i++)			//遍历数字的每一位
	{
		/*调用B_OLED_ShowChar函数，依次显示每个数字*/
		/*Number1 / B_OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
		/*+ '0' 可将数字转换为字符格式*/
		B_OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / B_OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

/**
  * 函    数：B_OLED显示十六进制数字（十六进制，正整数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
  * 参    数：Length 指定数字的长度，范围：0~8
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)		//遍历数字的每一位
	{
		/*以十六进制提取数字的每一位*/
		SingleNumber = Number / B_OLED_Pow(16, Length - i - 1) % 16;

		if (SingleNumber < 10)			//单个数字小于10
		{
			/*调用B_OLED_ShowChar函数，显示此数字*/
			/*+ '0' 可将数字转换为字符格式*/
			B_OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
		}
		else							//单个数字大于10
		{
			/*调用B_OLED_ShowChar函数，显示此数字*/
			/*+ 'A' 可将数字转换为从A开始的十六进制字符*/
			B_OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

/**
  * 函    数：B_OLED显示二进制数字（二进制，正整数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：0x00000000~0xFFFFFFFF
  * 参    数：Length 指定数字的长度，范围：0~16
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//遍历数字的每一位
	{
		/*调用B_OLED_ShowChar函数，依次显示每个数字*/
		/*Number / B_OLED_Pow(2, Length - i - 1) % 2 可以二进制提取数字的每一位*/
		/*+ '0' 可将数字转换为字符格式*/
		B_OLED_ShowChar(X + i * FontSize, Y, Number / B_OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
	}
}

/**
  * 函    数：B_OLED显示浮点数字（十进制，小数）
  * 参    数：X 指定数字左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定数字左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Number 指定要显示的数字，范围：-4294967295.0~4294967295.0
  * 参    数：IntLength 指定数字的整数位长度，范围：0~10
  * 参    数：FraLength 指定数字的小数位长度，范围：0~9，小数进行四舍五入显示
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	uint32_t PowNum, IntNum, FraNum;

	if (Number >= 0)						//数字大于等于0
	{
		B_OLED_ShowChar(X, Y, '+', FontSize);	//显示+号
	}
	else									//数字小于0
	{
		B_OLED_ShowChar(X, Y, '-', FontSize);	//显示-号
		Number = -Number;					//Number取负
	}

	/*提取整数部分和小数部分*/
	IntNum = Number;						//直接赋值给整型变量，提取整数
	Number -= IntNum;						//将Number的整数减掉，防止之后将小数乘到整数时因数过大造成错误
	PowNum = B_OLED_Pow(10, FraLength);		//根据指定小数的位数，确定乘数
	FraNum = round(Number * PowNum);		//将小数乘到整数，同时四舍五入，避免显示误差
	IntNum += FraNum / PowNum;				//若四舍五入造成了进位，则需要再加给整数

	/*显示整数部分*/
	B_OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);

	/*显示小数点*/
	B_OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);

	/*显示小数部分*/
	B_OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);
}

/**
  * 函    数：B_OLED显示汉字串
  * 参    数：X 指定汉字串左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定汉字串左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Chinese 指定要显示的汉字串，范围：必须全部为汉字或者全角字符，不要加入任何半角字符
  *           显示的汉字需要在B_OLED_Data.c里的B_OLED_CF16x16数组定义
  *           未找到指定汉字时，会显示默认图形（一个方框，内部一个问号）
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowChinese(int16_t X, int16_t Y, char *Chinese)
{
	uint8_t pChinese = 0;
	uint8_t pIndex;
	uint8_t i;
	char SingleChinese[B_OLED_CHN_CHAR_WIDTH + 1] = {0};

	for (i = 0; Chinese[i] != '\0'; i ++)		//遍历汉字串
	{
		SingleChinese[pChinese] = Chinese[i];	//提取汉字串数据到单个汉字数组
		pChinese ++;							//计次自增

		/*当提取次数到达B_OLED_CHN_CHAR_WIDTH时，即代表提取到了一个完整的汉字*/
		if (pChinese >= B_OLED_CHN_CHAR_WIDTH)
		{
			pChinese = 0;		//计次归零

			/*遍历整个汉字字模库，寻找匹配的汉字*/
			/*如果找到最后一个汉字（定义为空字符串），则表示汉字未在字模库定义，停止寻找*/
			for (pIndex = 0; strcmp(B_OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				/*找到匹配的汉字*/
				if (strcmp(B_OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
				{
					break;		//跳出循环，此时pIndex的值为指定汉字的索引
				}
			}

			/*将汉字字模库B_OLED_CF16x16的指定数据以16*16的图像格式显示*/
			B_OLED_ShowImage(X + ((i + 1) / B_OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, B_OLED_CF16x16[pIndex].Data);
		}
	}
}

/**
  * 函    数：B_OLED显示图像
  * 参    数：X 指定图像左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定图像左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Width 指定图像的宽度，范围：0~128
  * 参    数：Height 指定图像的高度，范围：0~64
  * 参    数：Image 指定要显示的图像
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i = 0, j = 0;
	int16_t Page, Shift;

	/*将图像所在区域清空*/
	B_OLED_ClearArea(X, Y, Width, Height);

	/*遍历指定图像涉及的相关页*/
	/*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*遍历指定图像涉及的相关列*/
		for (i = 0; i < Width; i ++)
		{
			if (X + i >= 0 && X + i <= 127)		//超出屏幕的内容不显示
			{
				/*负数坐标在计算页地址和移位时需要加一个偏移*/
				Page = Y / 8;
				Shift = Y % 8;
				if (Y < 0)
				{
					Page -= 1;
					Shift += 8;
				}

				if (Page + j >= 0 && Page + j <= 7)		//超出屏幕的内容不显示
				{
					/*显示图像在当前页的内容*/
					B_OLED_DisplayBuf[Page + j][X + i] |= Image[j * Width + i] << (Shift);
				}

				if (Page + j + 1 >= 0 && Page + j + 1 <= 7)		//超出屏幕的内容不显示
				{
					/*显示图像在下一页的内容*/
					B_OLED_DisplayBuf[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
				}
			}
		}
	}
}

/**
  * 函    数：B_OLED使用printf函数打印格式化字符串
  * 参    数：X 指定格式化字符串左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定格式化字符串左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：FontSize 指定字体大小
  *           范围：B_OLED_8X16		宽8像素，高16像素
  *                 B_OLED_6X8		宽6像素，高8像素
  * 参    数：format 指定要显示的格式化字符串，范围：ASCII码可见字符组成的字符串
  * 参    数：... 格式化字符串参数列表
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...)
{
	char String[256];						//定义字符数组
	va_list arg;							//定义可变参数列表数据类型的变量arg
	va_start(arg, format);					//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);			//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);							//结束变量arg
	B_OLED_ShowString(X, Y, String, FontSize);//B_OLED显示字符数组（字符串）
}

/**
  * 函    数：B_OLED在指定位置画一个点
  * 参    数：X 指定点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawPoint(int16_t X, int16_t Y)
{
	if (X >= 0 && X <= 127 && Y >=0 && Y <= 63)		//超出屏幕的内容不显示
	{
		/*将显存数组指定位置的一个Bit数据置1*/
		B_OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
	}
}

/**
  * 函    数：B_OLED获取指定位置点的值
  * 参    数：X 指定点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 返 回 值：指定位置点是否处于点亮状态，1：点亮，0：熄灭
  */
uint8_t B_OLED_GetPoint(int16_t X, int16_t Y)
{
	if (X >= 0 && X <= 127 && Y >=0 && Y <= 63)		//超出屏幕的内容不读取
	{
		/*判断指定位置的数据*/
		if (B_OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8))
		{
			return 1;	//为1，返回1
		}
	}

	return 0;		//否则，返回0
}

/**
  * 函    数：B_OLED画线
  * 参    数：X0 指定一个端点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y0 指定一个端点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：X1 指定另一个端点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y1 指定另一个端点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;

	if (y0 == y1)		//横线单独处理
	{
		/*0号点X坐标大于1号点X坐标，则交换两点X坐标*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}

		/*遍历X坐标*/
		for (x = x0; x <= x1; x ++)
		{
			B_OLED_DrawPoint(x, y0);	//依次画点
		}
	}
	else if (x0 == x1)	//竖线单独处理
	{
		/*0号点Y坐标大于1号点Y坐标，则交换两点Y坐标*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}

		/*遍历Y坐标*/
		for (y = y0; y <= y1; y ++)
		{
			B_OLED_DrawPoint(x0, y);	//依次画点
		}
	}
	else				//斜线
	{
		/*使用Bresenham算法画直线，可以避免耗时的浮点运算，效率更高*/
		/*参考文档：https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*参考教程：https://www.bilibili.com/video/BV1364y1d7Lo*/

		if (x0 > x1)	//0号点X坐标大于1号点X坐标
		{
			/*交换两点坐标*/
			/*交换后不影响画线，但是画线方向由第一、二、三、四象限变为第一、四象限*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}

		if (y0 > y1)	//0号点Y坐标大于1号点Y坐标
		{
			/*将Y坐标取负*/
			/*取负后影响画线，但是画线方向由第一、四象限变为第一象限*/
			y0 = -y0;
			y1 = -y1;

			/*置标志位yflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
			yflag = 1;
		}

		if (y1 - y0 > x1 - x0)	//画线斜率大于1
		{
			/*将X坐标与Y坐标互换*/
			/*互换后影响画线，但是画线方向由第一象限0~90度范围变为第一象限0~45度范围*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;

			/*置标志位xyflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
			xyflag = 1;
		}

		/*以下为Bresenham算法画直线*/
		/*算法要求，画线方向必须为第一象限0~45度范围*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;

		/*画起始点，同时判断标志位，将坐标换回来*/
		if (yflag && xyflag){B_OLED_DrawPoint(y, -x);}
		else if (yflag)		{B_OLED_DrawPoint(x, -y);}
		else if (xyflag)	{B_OLED_DrawPoint(y, x);}
		else				{B_OLED_DrawPoint(x, y);}

		while (x < x1)		//遍历X轴的每个点
		{
			x ++;
			if (d < 0)		//下一个点在当前点东方
			{
				d += incrE;
			}
			else			//下一个点在当前点东北方
			{
				y ++;
				d += incrNE;
			}

			/*画每一个点，同时判断标志位，将坐标换回来*/
			if (yflag && xyflag){B_OLED_DrawPoint(y, -x);}
			else if (yflag)		{B_OLED_DrawPoint(x, -y);}
			else if (xyflag)	{B_OLED_DrawPoint(y, x);}
			else				{B_OLED_DrawPoint(x, y);}
		}
	}
}

/**
  * 函    数：B_OLED矩形
  * 参    数：X 指定矩形左上角的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定矩形左上角的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Width 指定矩形的宽度，范围：0~128
  * 参    数：Height 指定矩形的高度，范围：0~64
  * 参    数：IsFilled 指定矩形是否填充
  *           范围：B_OLED_UNFILLED		不填充
  *                 B_OLED_FILLED			填充
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	int16_t i, j;
	if (!IsFilled)		//指定矩形不填充
	{
		/*遍历上下X坐标，画矩形上下两条线*/
		for (i = X; i < X + Width; i ++)
		{
			B_OLED_DrawPoint(i, Y);
			B_OLED_DrawPoint(i, Y + Height - 1);
		}
		/*遍历左右Y坐标，画矩形左右两条线*/
		for (i = Y; i < Y + Height; i ++)
		{
			B_OLED_DrawPoint(X, i);
			B_OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//指定矩形填充
	{
		/*遍历X坐标*/
		for (i = X; i < X + Width; i ++)
		{
			/*遍历Y坐标*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*在指定区域画点，填充满矩形*/
				B_OLED_DrawPoint(i, j);
			}
		}
	}
}

/**
  * 函    数：B_OLED三角形
  * 参    数：X0 指定第一个端点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y0 指定第一个端点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：X1 指定第二个端点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y1 指定第二个端点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：X2 指定第三个端点的横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y2 指定第三个端点的纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：IsFilled 指定三角形是否填充
  *           范围：B_OLED_UNFILLED		不填充
  *                 B_OLED_FILLED			填充
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled)
{
	int16_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	int16_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};

	if (!IsFilled)			//指定三角形不填充
	{
		/*调用画线函数，将三个点用直线连接*/
		B_OLED_DrawLine(X0, Y0, X1, Y1);
		B_OLED_DrawLine(X0, Y0, X2, Y2);
		B_OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//指定三角形填充
	{
		/*找到三个点最小的X、Y坐标*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}

		/*找到三个点最大的X、Y坐标*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}

		/*最小最大坐标之间的矩形为可能需要填充的区域*/
		/*遍历此区域中所有的点*/
		/*遍历X坐标*/
		for (i = minx; i <= maxx; i ++)
		{
			/*遍历Y坐标*/
			for (j = miny; j <= maxy; j ++)
			{
				/*调用B_OLED_pnpoly，判断指定点是否在指定三角形之中*/
				/*如果在，则画点，如果不在，则不做处理*/
				if (B_OLED_pnpoly(3, vx, vy, i, j)) {B_OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
  * 函    数：B_OLED画圆
  * 参    数：X 指定圆的圆心横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定圆的圆心纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Radius 指定圆的半径，范围：0~255
  * 参    数：IsFilled 指定圆是否填充
  *           范围：B_OLED_UNFILLED		不填充
  *                 B_OLED_FILLED			填充
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;

	/*使用Bresenham算法画圆，可以避免耗时的浮点运算，效率更高*/
	/*参考文档：https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
	/*参考教程：https://www.bilibili.com/video/BV1VM4y1u7wJ*/

	d = 1 - Radius;
	x = 0;
	y = Radius;

	/*画每个八分之一圆弧的起始点*/
	B_OLED_DrawPoint(X + x, Y + y);
	B_OLED_DrawPoint(X - x, Y - y);
	B_OLED_DrawPoint(X + y, Y + x);
	B_OLED_DrawPoint(X - y, Y - x);

	if (IsFilled)		//指定圆填充
	{
		/*遍历起始点Y坐标*/
		for (j = -y; j < y; j ++)
		{
			/*在指定区域画点，填充部分圆*/
			B_OLED_DrawPoint(X, Y + j);
		}
	}

	while (x < y)		//遍历X轴的每个点
	{
		x ++;
		if (d < 0)		//下一个点在当前点东方
		{
			d += 2 * x + 1;
		}
		else			//下一个点在当前点东南方
		{
			y --;
			d += 2 * (x - y) + 1;
		}

		/*画每个八分之一圆弧的点*/
		B_OLED_DrawPoint(X + x, Y + y);
		B_OLED_DrawPoint(X + y, Y + x);
		B_OLED_DrawPoint(X - x, Y - y);
		B_OLED_DrawPoint(X - y, Y - x);
		B_OLED_DrawPoint(X + x, Y - y);
		B_OLED_DrawPoint(X + y, Y - x);
		B_OLED_DrawPoint(X - x, Y + y);
		B_OLED_DrawPoint(X - y, Y + x);

		if (IsFilled)	//指定圆填充
		{
			/*遍历中间部分*/
			for (j = -y; j < y; j ++)
			{
				/*在指定区域画点，填充部分圆*/
				B_OLED_DrawPoint(X + x, Y + j);
				B_OLED_DrawPoint(X - x, Y + j);
			}

			/*遍历两侧部分*/
			for (j = -x; j < x; j ++)
			{
				/*在指定区域画点，填充部分圆*/
				B_OLED_DrawPoint(X - y, Y + j);
				B_OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}

/**
  * 函    数：B_OLED画椭圆
  * 参    数：X 指定椭圆的圆心横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定椭圆的圆心纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：A 指定椭圆的横向半轴长度，范围：0~255
  * 参    数：B 指定椭圆的纵向半轴长度，范围：0~255
  * 参    数：IsFilled 指定椭圆是否填充
  *           范围：B_OLED_UNFILLED		不填充
  *                 B_OLED_FILLED			填充
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;

	/*使用Bresenham算法画椭圆，可以避免部分耗时的浮点运算，效率更高*/
	/*参考链接：https://blog.csdn.net/myf_666/article/details/128167392*/

	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);

	if (IsFilled)	//指定椭圆填充
	{
		/*遍历起始点Y坐标*/
		for (j = -y; j < y; j ++)
		{
			/*在指定区域画点，填充部分椭圆*/
			B_OLED_DrawPoint(X, Y + j);
			B_OLED_DrawPoint(X, Y + j);
		}
	}

	/*画椭圆弧的起始点*/
	B_OLED_DrawPoint(X + x, Y + y);
	B_OLED_DrawPoint(X - x, Y - y);
	B_OLED_DrawPoint(X - x, Y + y);
	B_OLED_DrawPoint(X + x, Y - y);

	/*画椭圆中间部分*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//下一个点在当前点东方
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//下一个点在当前点东南方
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;

		if (IsFilled)	//指定椭圆填充
		{
			/*遍历中间部分*/
			for (j = -y; j < y; j ++)
			{
				/*在指定区域画点，填充部分椭圆*/
				B_OLED_DrawPoint(X + x, Y + j);
				B_OLED_DrawPoint(X - x, Y + j);
			}
		}

		/*画椭圆中间部分圆弧*/
		B_OLED_DrawPoint(X + x, Y + y);
		B_OLED_DrawPoint(X - x, Y - y);
		B_OLED_DrawPoint(X - x, Y + y);
		B_OLED_DrawPoint(X + x, Y - y);
	}

	/*画椭圆两侧部分*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;

	while (y > 0)
	{
		if (d2 <= 0)		//下一个点在当前点东方
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;

		}
		else				//下一个点在当前点东南方
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;

		if (IsFilled)	//指定椭圆填充
		{
			/*遍历两侧部分*/
			for (j = -y; j < y; j ++)
			{
				/*在指定区域画点，填充部分椭圆*/
				B_OLED_DrawPoint(X + x, Y + j);
				B_OLED_DrawPoint(X - x, Y + j);
			}
		}

		/*画椭圆两侧部分圆弧*/
		B_OLED_DrawPoint(X + x, Y + y);
		B_OLED_DrawPoint(X - x, Y - y);
		B_OLED_DrawPoint(X - x, Y + y);
		B_OLED_DrawPoint(X + x, Y - y);
	}
}

/**
  * 函    数：B_OLED画圆弧
  * 参    数：X 指定圆弧的圆心横坐标，范围：-32768~32767，屏幕区域：0~127
  * 参    数：Y 指定圆弧的圆心纵坐标，范围：-32768~32767，屏幕区域：0~63
  * 参    数：Radius 指定圆弧的半径，范围：0~255
  * 参    数：StartAngle 指定圆弧的起始角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * 参    数：EndAngle 指定圆弧的终止角度，范围：-180~180
  *           水平向右为0度，水平向左为180度或-180度，下方为正数，上方为负数，顺时针旋转
  * 参    数：IsFilled 指定圆弧是否填充，填充后为扇形
  *           范围：B_OLED_UNFILLED		不填充
  *                 B_OLED_FILLED			填充
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void B_OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;

	/*此函数借用Bresenham算法画圆的方法*/

	d = 1 - Radius;
	x = 0;
	y = Radius;

	/*在画圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
	if (B_OLED_IsInAngle(x, y, StartAngle, EndAngle))	{B_OLED_DrawPoint(X + x, Y + y);}
	if (B_OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - x, Y - y);}
	if (B_OLED_IsInAngle(y, x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + y, Y + x);}
	if (B_OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - y, Y - x);}

	if (IsFilled)	//指定圆弧填充
	{
		/*遍历起始点Y坐标*/
		for (j = -y; j < y; j ++)
		{
			/*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
			if (B_OLED_IsInAngle(0, j, StartAngle, EndAngle)) {B_OLED_DrawPoint(X, Y + j);}
		}
	}

	while (x < y)		//遍历X轴的每个点
	{
		x ++;
		if (d < 0)		//下一个点在当前点东方
		{
			d += 2 * x + 1;
		}
		else			//下一个点在当前点东南方
		{
			y --;
			d += 2 * (x - y) + 1;
		}

		/*在画圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
		if (B_OLED_IsInAngle(x, y, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + x, Y + y);}
		if (B_OLED_IsInAngle(y, x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + y, Y + x);}
		if (B_OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - x, Y - y);}
		if (B_OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - y, Y - x);}
		if (B_OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + x, Y - y);}
		if (B_OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + y, Y - x);}
		if (B_OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - x, Y + y);}
		if (B_OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - y, Y + x);}

		if (IsFilled)	//指定圆弧填充
		{
			/*遍历中间部分*/
			for (j = -y; j < y; j ++)
			{
				/*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
				if (B_OLED_IsInAngle(x, j, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + x, Y + j);}
				if (B_OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - x, Y + j);}
			}

			/*遍历两侧部分*/
			for (j = -x; j < x; j ++)
			{
				/*在填充圆的每个点时，判断指定点是否在指定角度内，在，则画点，不在，则不做处理*/
				if (B_OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {B_OLED_DrawPoint(X - y, Y + j);}
				if (B_OLED_IsInAngle(y, j, StartAngle, EndAngle)) {B_OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}

/*********************功能函数*/




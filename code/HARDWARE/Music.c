/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Music.c
  * @brief          : Music program body
  * @author         : 千樊
  *	@time			: 2022-01-15
  *
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Music.h"
#include "music_pwm.h"
#include "stdint.h"
#include "Delay.h" 
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PT */
	uint16_t Tone_Index[8][3]={
		{0  ,0  ,0   },
		{262,523,1046},
		{294,587,1175},
		{330,659,1318},
		{349,698,1397},
		{392,784,1568},
		{440,880,1760},
		{494,988,1976}		
	};
//	
//中速代表乐曲速度术语里的Moderato，或称中板，每分钟在88到104拍。
//中速 每分钟100拍 一拍600ms
//两只老虎简谱，没有细调
	uint16_t Music_Two_Tigers[][3]={
		{0,0,570},
		{1,Alto,One_Beat},
		{2,Alto,One_Beat},
		{3,Alto,One_Beat},
		{1,Alto,One_Beat},
		
		{0,Alto,24},
		
		{1,Alto,One_Beat},
		{2,Alto,One_Beat},
		{3,Alto,One_Beat},
		{1,Alto,One_Beat},
		
		//{0,Alto,3},
		
		{3,Alto,One_Beat},
		{4,Alto,One_Beat},
		{5,Alto,One_Beat},
		{0,Alto,One_Beat},
		
		
		{3,Alto,One_Beat},
		{4,Alto,One_Beat},
		{5,Alto,One_Beat},
		{0,Alto,One_Beat},
		
		
		{5,Alto,One_TWO_Beat},
		{6,Alto,One_TWO_Beat},
		{5,Alto,One_TWO_Beat},
		{4,Alto,One_TWO_Beat},
		{3,Alto,One_Beat},
		{1,Alto,One_Beat},
		
		//{0,Alto,3},
		
		{5,Alto,One_TWO_Beat},
		{6,Alto,One_TWO_Beat},
		{5,Alto,One_TWO_Beat},
		{4,Alto,One_TWO_Beat},
		{3,Alto,One_Beat},
		{1,Alto,One_Beat},
		
		{0,Alto,24},
		
		{1,Alto,One_Beat},
		{5,Bass,One_Beat},
		{1,Alto,One_Beat},
		{0,Alto,One_Beat},
		
		
		{1,Alto,One_Beat},
		{5,Bass,One_Beat},
		{1,Alto,One_Beat},
		{0,Alto,One_Beat},
		

	};

//曲信息
//中速 每分钟65拍 一拍920ms
uint16_t Music_Lone_Brave[][3]={
	{0,0,920},
	
//	#define Bass		0
//	#define	Alto		1
//	#define	Teble		2
	
//	#define	One_Beat				1
//	#define	One_TWO_Beat		2
//	#define	One_FOUR_Beat		4
	
	//第一小节
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	
	//第二小节	
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	
	//第三小节	
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	
	//第四小节	
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	
	//第五小节	
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{7,Bass,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat}  ,
	
	//第六小节	
	{3,Alto,One_Beat}	   ,{3,Alto,One_Beat}  	   ,{0,Alto,One_Beat}      ,{0,Alto,One_FOUR_Beat} ,
	{1,Alto,One_FOUR_Beat} ,{2,Alto,One_FOUR_Beat} ,{1,Alto,One_FOUR_Beat} ,
	
	//第七小节
	{3,Alto,One_Beat}      ,{3,Alto,One_Beat}	   ,{0,Alto,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat} ,
	{2,Alto,One_FOUR_Beat} ,{1,Alto,One_FOUR_Beat} ,{2,Alto,One_FOUR_Beat} ,{3,Alto,One_FOUR_Beat} ,
	
	//第八小节
	{6,Bass,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat} ,{6,Bass,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat} ,
	{6,Bass,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat} ,{2,Alto,One_TWO_Beat}  ,{1,Alto,One_TWO_Beat}  ,
	
	//第九小节
	{7,Bass,One_TWO_Beat}  ,{7,Bass,One_FOUR_Beat},{0,Alto,One_TWO_Beat}	 ,{0,Alto,One_FOUR_Beat} ,

	//第十小节
	{3,Alto,One_Beat}		   ,{3,Alto,One_Beat}		  ,{0,Alto,One_Beat}       ,{0,Alto,One_FOUR_Beat} ,
	{1,Alto,One_FOUR_Beat} ,{2,Alto,One_FOUR_Beat},{1,Alto,One_FOUR_Beat}  ,
	
	//第十一小节
	{3,Alto,One_Beat}      ,{3,Alto,One_Beat}			,{0,Alto,One_TWO_Beat}	 ,{1,Alto,One_FOUR_Beat} ,
	{2,Alto,One_FOUR_Beat} ,{1,Alto,One_FOUR_Beat},{2,Alto,One_FOUR_Beat}  ,{3,Alto,One_FOUR_Beat} ,
	
	//第十二小节
	{6,Bass,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat},{6,Bass,One_TWO_Beat}	 ,{1,Alto,One_FOUR_Beat} ,
	{6,Bass,One_TWO_Beat}  ,{1,Alto,One_FOUR_Beat},{3,Alto,One_TWO_Beat}   ,{2,Alto,One_TWO_Beat}  ,
		
	//第十三小节
	{7,Bass,One_TWO_Beat}  ,{7,Bass,One_FOUR_Beat},{0,Alto,One_TWO_Beat}	 ,{0,Alto,One_FOUR_Beat} ,
	
	//第十四小节
	{6,Bass,One_FOUR_Beat} ,{1,Alto,One_FOUR_Beat},{6,Alto,One_TWO_Beat}	 ,{6,Alto,One_FOUR_Beat} ,
	{0,Alto,20 /*小衔接*/} ,{6,Alto,One_FOUR_Beat},{6,Alto,One_FOUR_Beat}  ,{5,Alto,One_FOUR_Beat} ,
	{6,Alto,One_TWO_Beat}  ,{6,Alto,One_FOUR_Beat},{5,Alto,One_FOUR_Beat}  ,{6,Alto,One_FOUR_Beat} ,
	{5,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat},{5,Alto,One_FOUR_Beat}  ,
	
	//第十五小节
	{3,Alto,One_FOUR_Beat} ,{3,Alto,One_TWO_Beat} ,{3,Alto,One_Beat}	     ,{0,Alto,20 /*小衔接*/} ,
	{0,Alto,One_Beat}      ,{0,Alto,One_TWO_Beat} ,{6,Bass,One_FOUR_Beat}  ,{1,Alto,One_FOUR_Beat} ,
	
	//第十六小节
	{6,Alto,One_TWO_Beat}  ,{6,Alto,One_FOUR_Beat},{0,Alto,20 /*小衔接*/}  ,{6,Alto,One_FOUR_Beat} ,
	{5,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat},{5,Alto,One_FOUR_Beat}  ,{7,Alto,One_TWO_Beat}  ,
	{7,Alto,One_FOUR_Beat} ,{0,Alto,20 /*小衔接*/},{7,Alto,One_FOUR_Beat}  ,{6,Alto,One_FOUR_Beat} ,
	{7,Alto,One_TWO_Beat}  ,
	
	//第十七小节
	{7,Alto,One_FOUR_Beat} ,{6,Alto,One_TWO_Beat} ,{3,Alto,One_FOUR_Beat}  ,{3,Alto,One_TWO_Beat}  ,
	{3,Alto,One_TWO_Beat}  ,{0,Alto,One_FOUR_Beat},{3,Alto,One_FOUR_Beat}  ,{5,Alto,One_FOUR_Beat} ,
	{3,Alto,One_FOUR_Beat} ,

	//第十八小节
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{2,Alto,One_TWO_Beat}   ,{3,Alto,One_FOUR_Beat} ,
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{5,Alto,One_FOUR_Beat}  ,{3,Alto,One_FOUR_Beat} ,
	{5,Alto,One_FOUR_Beat} ,{3,Alto,One_FOUR_Beat},
	
	//第十九小节
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{2,Alto,One_TWO_Beat}   ,{3,Alto,One_FOUR_Beat} ,
	{2,Alto,One_Beat}      ,{0,Alto,One_TWO_Beat} ,{1,Alto,One_FOUR_Beat}  ,{2,Alto,One_FOUR_Beat} ,
	
	//第二十小节
	{3,Alto,One_TWO_Beat}  ,{6,Bass,One_TWO_Beat} ,{1,Alto,One_TWO_Beat}   ,{3,Alto,One_TWO_Beat}  ,
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{2,Alto,One_FOUR_Beat}  ,{1,Alto,One_FOUR_Beat} ,
	{1,Alto,One_TWO_Beat}  ,
	
	//第二十一小节
	{6,Bass,One_Beat}      ,{6,Bass,One_Beat} 	  ,{0,Alto,One_Beat}       ,{0,Alto,One_TWO_Beat}  ,
	{6,Alto,One_FOUR_Beat} ,{7,Alto,One_FOUR_Beat},
	
	//第二十二小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十三小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{6,Alto,One_FOUR_Beat} ,
	{7,Alto,One_FOUR_Beat} ,
	
	//第二十四小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十五小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,
	
	//第二十六小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,
	
	//第二十七小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{5,Teble,One_TWO_Beat} ,
	
	//第二十八小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	
	//第二十九小节
	{1,Teble,One_TWO_Beat}  ,{0,Alto,One_FOUR_Beat} ,{0,Alto,One_TWO_Beat}  ,{0,Alto,One_TWO_Beat}  ,
	{5,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{5,Teble,One_FOUR_Beat},
	
	//第三十小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	
	//第三十一小节
	{1,Teble,One_TWO_Beat} ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,
//	
//	//第三十二小节
//	{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,
//	
//	//第三十三小节
//	{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,
	
	//第三十四小节
	{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_TWO_Beat}  ,
	{6,Teble,One_FOUR_Beat},{5,Alto,One_FOUR_Beat} ,
	
	//第三十五小节
	{6,Alto,One_TWO_Beat}  ,{5,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,
	{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,{6,Alto,One_TWO_Beat}  ,{0,Alto,184 /*小衔接*/},
	{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,
	{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,
	
	//第三十六小节
//	{3,Alto,One_FOUR_Beat} ,{3,Alto,One_TWO_Beat}  ,{3,Alto,One_Beat}       ,{0,Alto,One_Beat}     ,
//	{0,Alto,One_TWO_Beat}  ,{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat}  ,
	{3,Alto,One_TWO_Beat}  ,{3,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,
	{0,Alto,One_TWO_Beat}  ,{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,
	
	//第三十七小节
	{6,Alto,One_TWO_Beat}  ,{5,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,
	{6,Alto,One_FOUR_Beat} ,{5,Alto,One_FOUR_Beat} ,{7,Alto,One_TWO_Beat}  ,{0,Alto,184 /*小衔接*/} ,
	{7,Alto,One_FOUR_Beat} ,{0,Alto,184 /*小衔接*/} ,{7,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat} ,
	{7,Alto,One_FOUR_Beat} ,{6,Alto,One_FOUR_Beat} ,

	//第三十八小节
//	{3,Alto,One_FOUR_Beat} ,{3,Alto,One_TWO_Beat}  ,{3,Alto,One_Beat}  ,{0,Alto,One_Beat}  ,
//	{0,Alto,One_FOUR_Beat},{3,Alto,One_FOUR_Beat}  ,{5,Alto,One_FOUR_Beat} ,{3,Alto,One_FOUR_Beat} ,
	{3,Alto,One_TWO_Beat}  ,{3,Alto,One_Beat}  ,{0,Alto,One_Beat}  ,{0,Alto,One_FOUR_Beat},
	{3,Alto,One_FOUR_Beat}  ,{5,Alto,One_FOUR_Beat} ,{3,Alto,One_FOUR_Beat} ,
	
	
	//第三十九小节
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{2,Alto,One_TWO_Beat}   ,{3,Alto,One_FOUR_Beat} ,
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{5,Alto,One_FOUR_Beat}  ,{3,Alto,One_FOUR_Beat} ,
	{5,Alto,One_FOUR_Beat} ,{3,Alto,One_FOUR_Beat},
	
	//第四十小节
	{2,Alto,One_TWO_Beat}  ,{3,Alto,One_FOUR_Beat},{2,Alto,One_TWO_Beat}   ,{3,Alto,One_FOUR_Beat} ,
	{2,Alto,One_Beat}      ,{0,Alto,One_TWO_Beat} ,{1,Alto,One_FOUR_Beat}  ,{2,Alto,One_FOUR_Beat} ,
	
	
	//第四十一小节
	{3,Alto,One_TWO_Beat}  ,{6,Alto,One_TWO_Beat} ,{1,Teble,One_TWO_Beat}   ,{3,Teble,One_TWO_Beat}  ,
	{2,Teble,One_TWO_Beat}  ,{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat}  ,{1,Teble,One_FOUR_Beat} ,
	{1,Teble,One_TWO_Beat}  ,
	
	//第四十二小节
	{6,Alto,One_Beat}  ,{0,Alto,One_Beat} ,{0,Alto,One_Beat}   ,{0,Alto,One_TWO_Beat}  ,
	{6,Alto,One_FOUR_Beat}  ,{7,Alto,One_FOUR_Beat},
	
	
	//开始第一遍循环
	//第二十二小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十三小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{6,Alto,One_FOUR_Beat} ,
	{7,Alto,One_FOUR_Beat} ,
	
	//第二十四小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十五小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,
	
	//第二十六小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,
	
	//第二十七小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{5,Teble,One_TWO_Beat} ,
	
	//第二十八小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	
	//第二十九小节
	{1,Teble,One_TWO_Beat}  ,{0,Alto,One_FOUR_Beat} ,{0,Alto,One_TWO_Beat}  ,{0,Alto,One_TWO_Beat}  ,
	{5,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{5,Teble,One_FOUR_Beat},
	
	//第三十小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	
	//第一遍循环结束
	
	//第四十三小节
	{6,Alto,One_TWO_Beat}  ,{6,Alto,One_TWO_Beat} ,{1,Alto,One_TWO_Beat}  ,{3,Alto,One_TWO_Beat}  ,
	{7,Alto,One_Beat},{0,Alto,184 /*小衔接*/},{7,Alto,One_TWO_Beat},{0,Alto,184 /*小衔接*/},
	{7,Alto,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{7,Alto,One_FOUR_Beat},
	
	//第四十四小节
	//{6,Alto,One_FOUR_Beat}  ,{6,Alto,One_TWO_Beat} ,{6,Alto,One_Beat}  ,{0,Alto,One_Beat}  ,
	//{0,Alto,One_Beat},{0,Alto,One_Beat},
	{6,Alto,One_TWO_Beat} ,{6,Alto,One_TWO_Beat} ,{6,Alto,One_TWO_Beat} ,{0,Alto,One_Beat}  ,
	{0,Alto,One_Beat},{0,Alto,One_Beat},
	
	//第四十五小节
	{6,Alto,One_TWO_Beat}  ,{6,Alto,One_TWO_Beat} ,{1,Alto,One_TWO_Beat}  ,{3,Alto,One_TWO_Beat}  ,
	{7,Alto,One_Beat},{0,Alto,184 /*小衔接*/},{7,Alto,One_TWO_Beat},{0,Alto,184 /*小衔接*/},
	{7,Alto,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{7,Alto,One_FOUR_Beat},
	
	//第四十六小节
	{7,Alto,One_FOUR_Beat},{6,Alto,One_TWO_Beat},{6,Alto,One_Beat} ,{0,Alto,One_Beat},
	{0,Alto,One_TWO_Beat},{6,Alto,One_FOUR_Beat} ,{7,Alto,One_FOUR_Beat},
	
	
	//第二次循环
	//第二十二小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十三小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{6,Alto,One_FOUR_Beat} ,
	{7,Alto,One_FOUR_Beat} ,
	
	//第二十四小节
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{7,Alto,One_FOUR_Beat} ,{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{7,Alto,One_FOUR_Beat} ,
	{1,Teble,One_FOUR_Beat},{0,Alto,306 /*小衔接*/},{1,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},
	{1,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	
	//第二十五小节
	{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{0,Alto,306 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,{3,Teble,One_TWO_Beat} ,{5,Teble,One_TWO_Beat} ,
	
	//第二十六小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,
	
	//第二十七小节
	{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},{3,Teble,One_TWO_Beat} ,{5,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{5,Teble,One_FOUR_Beat},{6,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{5,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{5,Teble,One_TWO_Beat} ,
	
	//第二十八小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	
	//第二十九小节
	{1,Teble,One_TWO_Beat}  ,{0,Alto,One_FOUR_Beat} ,{0,Alto,One_TWO_Beat}  ,{0,Alto,One_TWO_Beat}  ,
	{5,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{5,Teble,One_FOUR_Beat},
	
	//第三十小节
	//{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,
	//{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{3,Teble,One_FOUR_Beat},{2,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},
	//{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},
	{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},
	{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},{3,Teble,One_FOUR_Beat},
	{2,Teble,One_TWO_Beat} ,{0,Alto,184 /*小衔接*/},{2,Teble,One_TWO_Beat} ,{1,Teble,One_FOUR_Beat},
	{0,Alto,184 /*小衔接*/},{1,Teble,One_FOUR_Beat},

	//第二次循环结束
	
	//第四十七小节
	{1,Teble,One_TWO_Beat} ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,{0,Alto,One_Beat}      ,

	{0,Alto,One_Beat} ,
};


/* USER CODE END PT */
/* Function definition -------------------------------------------------------*/
/* USER CODE BEGIN FD */
/**
  * @Function name  MUSIC_Begin
  * @Introduce  	开始播放音乐						
  * @Return 		NULL
  */
void MUSIC_Analysis(void){
		uint16_t i;
		//切换数组即可更改音乐
		uint16_t MusicBeatNum = ((((sizeof(Music_Lone_Brave))/2)/3)-1);
		
		uint16_t MusicSpeed = Music_Lone_Brave[0][2];
		for(i = 1;i<=MusicBeatNum;i++){
			music_PWM_SetPSC(Tone_Index[Music_Lone_Brave[i][0]][Music_Lone_Brave[i][1]]);
			vTaskDelay(MusicSpeed/Music_Lone_Brave[i][2]);
		}
	}
	
/* USER CODE END FD */
/************************ (C) COPYRIGHT 「千樊」 *****END OF FILE****/

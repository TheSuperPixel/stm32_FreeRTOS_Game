/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body

    ����STM32�Ķ���˹������Ϸ

    1. ��Ƭ���ͺţ�STM32F103C8T6
    2. ��ʾ���ͺţ�0.96��OLED��SSD1306
    3. IO���ӹ�ϵ
    OLED SCL  ----  PB12
    OLED SDA  ----  PB13
    left      ----  PA0
    right     ----  PA1
    down      ----  PA2
    f         ----  PA3  (���ܰ�������ʼ��Ϸ������ת)

    4. ��ע Bվ�����������֡�΢�Ź��ں� ��JL��Ƭ���������Բ鿴����˹�������ʾЧ����
    5. ��ע Bվ�����������֡�΢�Ź��ں� ��JL��Ƭ��������ȡ������Ȥ�Ŀ�Դ��Ƭ�����롣

  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled12864_4.h"
#include "Key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "spritebank4.h"
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
// ===================================================================================
// Global Variables
// ===================================================================================
unsigned char Grid_TTRIS[12][3] = {{0}};
unsigned char MEM_TTTRIS[16] = {0, 2, 0, 4, 3, 7, 6, 9, 9, 12, 11, 15, 14, 17, 17, 19};
unsigned char Level_TTRIS;
unsigned int Scores_TTRIS;
unsigned int Nb_of_line_F_TTRIS;
unsigned char Level_Speed_ADJ_TTRIS;
unsigned char Nb_of_line_TTRIS[3];
unsigned char RND_VAR_TTRIS;
unsigned char LONG_PRESS_X_TTRIS;
unsigned char DOWN_DESACTIVE_TTRIS;
unsigned char DROP_SPEED_TTRIS;
unsigned char SPEED_x_trig_TTRIS;
unsigned char DROP_TRIG_TTRIS;
int xx_TTRIS, yy_TTRIS;
unsigned char Piece_Mat2_TTRIS[5][5];
unsigned char Ripple_filter_TTRIS;
unsigned char PIECEs_TTRIS;
unsigned char PIECEs_TTRIS_PREVIEW;
unsigned char PIECEs_rot_TTRIS;
unsigned char DROP_BREAK_TTRIS;
int OU_SUIS_JE_X_TTRIS;
int OU_SUIS_JE_Y_TTRIS;
unsigned char OU_SUIS_JE_X_ENGAGED_TTRIS;
unsigned char OU_SUIS_JE_Y_ENGAGED_TTRIS;
int DEPLACEMENT_XX_TTRIS;
int DEPLACEMENT_YY_TTRIS;

#define JOY_OLED_data_start(y) \
  {                            \
    OLED_SetPos(0, y);         \
    OLED_data_start();         \
  }
#define JOY_OLED_send(b) \
  {                      \
    IIC_WriteByte(b);    \
    IIC_WaitAck();       \
  }
#define JOY_OLED_end IIC_Stop

extern TaskHandle_t menuTask_Handler;
extern TaskHandle_t game4_Handler;
// ===================================================================================
// Function Prototypes
// ===================================================================================
void reset_Score_TTRIS(void);
unsigned char PSEUDO_RND_TTRIS(void);
void SND_TTRIS(unsigned char Snd_TTRIS);
void INTRO_MANIFEST_TTRIS(void);
void END_DROP_TTRIS(void);
void SETUP_NEW_PREVIEW_PIECE_TTRIS(unsigned char *Rot_TTRIS);
void CONTROLE_TTRIS(unsigned char *Rot_TTRIS);
void Game_Play_TTRIS(void);
unsigned char End_Play_TTRIS(void);
void DELETE_LINE_TTRIS(void);
unsigned char Calcul_of_Score_TTRIS(unsigned char Tmp_TTRIS);
void FLASH_LINE_TTRIS(unsigned char *PASS_LINE);
void PAINT_LINE_TTRIS(unsigned char VISIBLE, unsigned char *PASS_LINE);
void Clean_Grid_TTRIS(unsigned char *PASS_LINE);
unsigned char CHECK_if_Rot_Ok_TTRIS(unsigned char *Rot_TTRIS);
unsigned char Check_collision_x_TTRIS(int x_Axe);
unsigned char Check_collision_y_TTRIS(int y_Axe);
void Move_Piece_TTRIS(void);
void Ou_suis_Je_TTRIS(int xx_, int yy_);
void Select_Piece_TTRIS(unsigned char Piece_);
void rotate_Matrix_TTRIS(unsigned char ROT);
unsigned char Scan_Piece_Matrix_TTRIS(int x_Mat, int y_Mat);
unsigned char GRID_STAT_TTRIS(int X_SCAN, int Y_SCAN);
unsigned char CHANGE_GRID_STAT_TTRIS(int X_SCAN, int Y_SCAN, unsigned char VALUE);
unsigned char blitzSprite_TTRIS(int xPos, int yPos, unsigned char xPASS, unsigned char yPASS, unsigned char FRAME, const unsigned char *SPRITES);
unsigned char H_grid_Scan_TTRIS(unsigned char xPASS);
unsigned char Recupe_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char NEXT_BLOCK_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char RECUPE_BACKGROUND_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char DropPiece_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char SplitSpriteDecalageY_TTRIS(unsigned char decalage, unsigned char Input, unsigned char UPorDOWN);
unsigned char RecupeLineY_TTRIS(unsigned char Valeur);
unsigned char RecupeDecalageY_TTRIS(unsigned char Valeur);
void Tiny_Flip_TTRIS(unsigned char HR_TTRIS);
void Flip_intro_TTRIS(unsigned char *TIMER1);
unsigned char intro_TTRIS(unsigned char xPASS, unsigned char yPASS, unsigned char *TIMER1);
unsigned char Recupe_Start_TTRIS(unsigned char xPASS, unsigned char yPASS, unsigned char *TIMER1);
unsigned char recupe_Chateau_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char recupe_SCORES_TTRIS(unsigned char xPASS, unsigned char yPASS);
void Convert_Nb_of_line_TTRIS(void);
unsigned char recupe_Nb_of_line_TTRIS(unsigned char xPASS, unsigned char yPASS);
unsigned char recupe_LEVEL_TTRIS(unsigned char xPASS, unsigned char yPASS);
void INIT_ALL_VAR_TTRIS(void);
void recupe_HIGHSCORE_TTRIS(void);
void Reset_Value_TTRIS(void);
void save_HIGHSCORE_TTRIS(void);
void Check_NEW_RECORD(void);
unsigned char checksum(unsigned char Byte_);

void JOY_DLY_ms(int ms)
{
  vTaskDelay(ms);
}

unsigned char JOY_act_pressed()
{
  if (Get_Key() == r_push_key_value)
    return 1;
  return 0;
}

unsigned char JOY_left_pressed()
{
  if (Get_Key() == r_left_key_value)
  {
    vTaskDelay(5);
    if (Get_Key() == r_left_key_value)
      return 1;
  }

  return 0;
}

unsigned char JOY_right_pressed()
{
  if (Get_Key() == r_right_key_value)
  {
    vTaskDelay(5);
    if (Get_Key() == r_right_key_value)
      return 1;
  }

  return 0;
}

unsigned char JOY_act_released()
{

  if (Get_Key() == r_push_key_value)
    return 0;
  return 1;
}

unsigned char JOY_down_pressed()
{
  if (Get_Key() == r_back_key_value)
    return 1;
  return 0;
}

// Additional Defines
#define abs(n) ((n >= 0) ? (n) : (-(n)))

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int game4(void)
{

  /* USER CODE BEGIN 2 */
  Reset_Value_TTRIS();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    unsigned char Rot_TTRIS = 0;
    unsigned char SKIP_FRAME = 0;
    INIT_ALL_VAR_TTRIS();
    Game_Play_TTRIS();
    Ou_suis_Je_TTRIS(xx_TTRIS, yy_TTRIS);

    INTRO_MANIFEST_TTRIS();

    SETUP_NEW_PREVIEW_PIECE_TTRIS(&Rot_TTRIS);
    Tiny_Flip_TTRIS(128);
    JOY_DLY_ms(1000);
    xx_TTRIS = 55;
    yy_TTRIS = 5;

    while (1)
    {
      CONTROLE_TTRIS(&Rot_TTRIS);
      if (DROP_BREAK_TTRIS == 6)
      {
        END_DROP_TTRIS();
        if (End_Play_TTRIS())
        {
          Tiny_Flip_TTRIS(128);
          SND_TTRIS(3);
          JOY_DLY_ms(2000);
          Check_NEW_RECORD();
          break;
        }
        yy_TTRIS = 2;
        xx_TTRIS = 55;
        PIECEs_TTRIS = PIECEs_TTRIS_PREVIEW;
        SETUP_NEW_PREVIEW_PIECE_TTRIS(&Rot_TTRIS);
        DOWN_DESACTIVE_TTRIS = 1;
        Tiny_Flip_TTRIS(128);
        Game_Play_TTRIS();
        Tiny_Flip_TTRIS(128);
      }

      if ((JOY_act_pressed()) && (Ripple_filter_TTRIS == 0))
      {
        PSEUDO_RND_TTRIS();
        Ripple_filter_TTRIS = 1;
      }

      Move_Piece_TTRIS();
      if (SKIP_FRAME == 6)
      {
        Tiny_Flip_TTRIS(82);
        SKIP_FRAME = 0;
      }
      else
      {
        SKIP_FRAME++;
      }
      if (get_key_status() == sw1_key_value)
      {
        vTaskResume(menuTask_Handler);
        vTaskSuspend(game4_Handler);
        Tiny_Flip_TTRIS(128);
        Game_Play_TTRIS();
        Tiny_Flip_TTRIS(128);
      }
      else if (get_key_status() == sw2_key_value)
      {
        vTaskResume(menuTask_Handler);
        vTaskDelete(game4_Handler);
        Tiny_Flip_TTRIS(128);
        Game_Play_TTRIS();
        Tiny_Flip_TTRIS(128);
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
// ===================================================================================
// Functions
// ===================================================================================
void reset_Score_TTRIS(void)
{
  unsigned char x = 0;
  for (x = 0; x < 3; x++)
  {
    Nb_of_line_TTRIS[x] = 0;
  }
  Level_TTRIS = 0;
  Scores_TTRIS = 0;
  Nb_of_line_F_TTRIS = 0;
}

unsigned char PSEUDO_RND_TTRIS(void)
{
  RND_VAR_TTRIS = (RND_VAR_TTRIS < 6) ? RND_VAR_TTRIS + 1 : 0;
  return RND_VAR_TTRIS;
}

void SND_TTRIS(unsigned char Snd_TTRIS)
{
}

void INTRO_MANIFEST_TTRIS(void)
{
  unsigned char TIMER_1 = 0;
  recupe_HIGHSCORE_TTRIS();
  Convert_Nb_of_line_TTRIS();
  Flip_intro_TTRIS(&TIMER_1);
  while (1)
  {
    PIECEs_TTRIS = PSEUDO_RND_TTRIS();
    if (JOY_act_pressed())
    {
      reset_Score_TTRIS();
      break;
    }
    JOY_DLY_ms(33);
    TIMER_1 = (TIMER_1 < 7) ? TIMER_1 + 1 : 0;
    Flip_intro_TTRIS(&TIMER_1);
    if (get_key_status() == sw1_key_value)
    {
      vTaskResume(menuTask_Handler);
      vTaskSuspend(game4_Handler);
      Flip_intro_TTRIS(&TIMER_1);
    }
    else if (get_key_status() == sw2_key_value)
    {
      vTaskResume(menuTask_Handler);
      vTaskDelete(game4_Handler);
    }
  }
  SND_TTRIS(4);
}

void END_DROP_TTRIS(void)
{
  unsigned char x, y;
  DROP_BREAK_TTRIS = 0;
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      if (Piece_Mat2_TTRIS[x][y] == 1)
      {
        CHANGE_GRID_STAT_TTRIS(OU_SUIS_JE_X_TTRIS + (x), OU_SUIS_JE_Y_TTRIS + (y), 1);
      }
    }
  }
  Scores_TTRIS = (OU_SUIS_JE_Y_TTRIS < 9) ? Scores_TTRIS + 2 : Scores_TTRIS + 1;
  yy_TTRIS = 0;
  xx_TTRIS = 0;
  DELETE_LINE_TTRIS();
  Convert_Nb_of_line_TTRIS();
}

void SETUP_NEW_PREVIEW_PIECE_TTRIS(unsigned char *Rot_TTRIS)
{
  PIECEs_TTRIS_PREVIEW = PSEUDO_RND_TTRIS();
  Select_Piece_TTRIS(PIECEs_TTRIS);
  *Rot_TTRIS = 0;
  rotate_Matrix_TTRIS(*Rot_TTRIS);
}

void CONTROLE_TTRIS(unsigned char *Rot_TTRIS)
{
  if ((OU_SUIS_JE_X_ENGAGED_TTRIS == 0))
  {
    if (SPEED_x_trig_TTRIS == 0)
    {
      if (JOY_right_pressed())
      {
        if (LONG_PRESS_X_TTRIS == 0)
        {
          SND_TTRIS(1);
        }
        if ((LONG_PRESS_X_TTRIS == 0) || (LONG_PRESS_X_TTRIS == 20))
        {
          DEPLACEMENT_XX_TTRIS = 1;
          SPEED_x_trig_TTRIS = 2;
        }
        if (LONG_PRESS_X_TTRIS < 20)
        {
          LONG_PRESS_X_TTRIS++;
        }
      }
      if (JOY_left_pressed())
      {
        if (LONG_PRESS_X_TTRIS == 0)
        {
          SND_TTRIS(1);
        }
        if ((LONG_PRESS_X_TTRIS == 0) || (LONG_PRESS_X_TTRIS == 20))
        {
          DEPLACEMENT_XX_TTRIS = -1;
          SPEED_x_trig_TTRIS = 2;
        }
        if (LONG_PRESS_X_TTRIS < 20)
        {
          LONG_PRESS_X_TTRIS++;
        }
      }
    }
    else
    {
      SPEED_x_trig_TTRIS = (SPEED_x_trig_TTRIS > 0) ? SPEED_x_trig_TTRIS - 1 : 0;
    }
  }
  if ((JOY_right_pressed() == 0) && (JOY_left_pressed() == 0))
  {
    LONG_PRESS_X_TTRIS = 0;
    PSEUDO_RND_TTRIS();
  }

  if (JOY_act_released())
  {
    if ((OU_SUIS_JE_X_ENGAGED_TTRIS == 0) && (OU_SUIS_JE_Y_ENGAGED_TTRIS == 0))
    {
      Ripple_filter_TTRIS = 0;
    }
  }
  if ((Ripple_filter_TTRIS == 1))
  {
    CHECK_if_Rot_Ok_TTRIS(Rot_TTRIS);
    Ripple_filter_TTRIS = 2;
  }
  if (OU_SUIS_JE_Y_ENGAGED_TTRIS == 0)
  {
    DROP_TRIG_TTRIS--;
    if (DROP_TRIG_TTRIS == 0)
    {
      DEPLACEMENT_YY_TTRIS = 1;
      DROP_TRIG_TTRIS = Level_Speed_ADJ_TTRIS;
    }
  }
  if (DROP_SPEED_TTRIS > 0)
  {
    DROP_SPEED_TTRIS--;
  }
  else
  {
    DROP_SPEED_TTRIS = Level_Speed_ADJ_TTRIS;
  }
  if (JOY_down_pressed())
  {

    // ajouter cest 2 ligne
    if (OU_SUIS_JE_X_ENGAGED_TTRIS == 0)
    {
      DEPLACEMENT_XX_TTRIS = 0;
      LONG_PRESS_X_TTRIS = 1;
    } // ajouter cest 2 ligne

    PSEUDO_RND_TTRIS();
    if (DOWN_DESACTIVE_TTRIS == 0)
    {
      DEPLACEMENT_YY_TTRIS = 1;
      DROP_SPEED_TTRIS = 0;
    }
  }
  else
  {
    DOWN_DESACTIVE_TTRIS = 0;
  }
}

void Game_Play_TTRIS(void)
{
  unsigned char LEVEL_TMP = (Nb_of_line_F_TTRIS / 20);
  if (Level_TTRIS != LEVEL_TMP)
  {
    Level_TTRIS = LEVEL_TMP;
    SND_TTRIS(2);
  }
  if (Level_TTRIS < 21)
  {
    Level_Speed_ADJ_TTRIS = map(Level_TTRIS, 0, 20, 11, 1);
  }
}

unsigned char End_Play_TTRIS(void)
{
  unsigned char t = 0;
  for (t = 0; t < 12; t++)
  {
    if (GRID_STAT_TTRIS(t, 1) == 1)
    {
      return 1;
    }
  }
  return 0;
}

void DELETE_LINE_TTRIS(void)
{
  unsigned char LOOP, SCAN_Line, ADDs_Block, OK_DELETE = 0;
  unsigned char LINE_MEM[19] = {0};
  unsigned char Nb_of_Line_temp = 0;
  for (LOOP = 0; LOOP < 19; LOOP++)
  {
    ADDs_Block = 1;
    for (SCAN_Line = 0; SCAN_Line < 12; SCAN_Line++)
    {
      if (GRID_STAT_TTRIS(SCAN_Line, LOOP) == 0)
      {
        ADDs_Block = 0;
      }
    }
    if (ADDs_Block)
    {
      LINE_MEM[LOOP] = 1;
      OK_DELETE = 1;
    }
  }
  if (OK_DELETE == 1)
  {
    FLASH_LINE_TTRIS(&LINE_MEM[0]);
    Clean_Grid_TTRIS(&LINE_MEM[0]);
  }
  for (LOOP = 0; LOOP < 19; LOOP++)
  {
    if (LINE_MEM[LOOP] == 1)
    {
      Nb_of_Line_temp++;
    }
  }
  Nb_of_line_F_TTRIS = Nb_of_line_F_TTRIS + Nb_of_Line_temp;
  Scores_TTRIS = (Scores_TTRIS + Calcul_of_Score_TTRIS(Nb_of_Line_temp));
}

unsigned char Calcul_of_Score_TTRIS(unsigned char Tmp_TTRIS)
{
  switch (Tmp_TTRIS)
  {
  case 0:
    return 0;
    break;
  case 1:
    return 2;
    break;
  case 2:
    return 5;
    break;
  case 3:
    return 8;
    break;
  case 4:
    return 12;
    break;
  default:
    return 0;
    break;
  }
}

void FLASH_LINE_TTRIS(unsigned char *PASS_LINE)
{
  unsigned char LOOP;
  for (LOOP = 0; LOOP < 5; LOOP++)
  {
    PAINT_LINE_TTRIS(1, &PASS_LINE[0]);
    Tiny_Flip_TTRIS(82);

    PAINT_LINE_TTRIS(0, &PASS_LINE[0]);
    Tiny_Flip_TTRIS(82);
  }
  SND_TTRIS(5);
}

void PAINT_LINE_TTRIS(unsigned char VISIBLE, unsigned char *PASS_LINE)
{
  unsigned char LOOP, SCAN_Line;
  for (LOOP = 0; LOOP < 19; LOOP++)
  {
    if (PASS_LINE[LOOP] == 1)
    {
      for (SCAN_Line = 0; SCAN_Line < 12; SCAN_Line++)
      {
        CHANGE_GRID_STAT_TTRIS(SCAN_Line, LOOP, VISIBLE);
      }
    }
  }
}

void Clean_Grid_TTRIS(unsigned char *PASS_LINE)
{
  unsigned char GRID_2 = 18, GRID_1 = 18;
  unsigned char x;
  while (1)
  {
    if (PASS_LINE[GRID_1] == 1)
    {
      GRID_2 = GRID_1;
      GRID_2 = (GRID_2 > 0) ? GRID_2 - 1 : 0;
      break;
    }
    GRID_1 = (GRID_1 > 0) ? GRID_1 - 1 : 0;
  }
  while (1)
  {
    while (1)
    {
      if (PASS_LINE[GRID_2] == 1)
      {
        GRID_2 = (GRID_2 > 0) ? GRID_2 - 1 : 0;
      }
      else
      {
        break;
      }
    }
    for (x = 0; x < 12; x++)
    {
      CHANGE_GRID_STAT_TTRIS(x, GRID_1, (GRID_2 > 0) ? GRID_STAT_TTRIS(x, GRID_2) : 0);
    }
    GRID_1 = (GRID_1 > 0) ? GRID_1 - 1 : 0;
    GRID_2 = (GRID_2 > 0) ? GRID_2 - 1 : 0;
    if (GRID_1 == 0)
    {
      break;
    }
  }
}

unsigned char CHECK_if_Rot_Ok_TTRIS(unsigned char *Rot_TTRIS)
{
  unsigned char Mem_rot = *Rot_TTRIS;
  Ou_suis_Je_TTRIS(xx_TTRIS, yy_TTRIS);
  *Rot_TTRIS = (*Rot_TTRIS < PIECEs_rot_TTRIS) ? *Rot_TTRIS + 1 : 0;
  rotate_Matrix_TTRIS(*Rot_TTRIS);

  if ((Check_collision_x_TTRIS(OU_SUIS_JE_X_ENGAGED_TTRIS) || (Check_collision_y_TTRIS(OU_SUIS_JE_Y_ENGAGED_TTRIS))) != 0)
  {
    *Rot_TTRIS = Mem_rot;
    rotate_Matrix_TTRIS(*Rot_TTRIS);
    return 1;
  }
  SND_TTRIS(0);
  return 0;
}

unsigned char Check_collision_x_TTRIS(int x_Axe)
{
  unsigned char x, y;
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      if (Piece_Mat2_TTRIS[x][y] == 1)
      {
        if (GRID_STAT_TTRIS((x + OU_SUIS_JE_X_TTRIS) + x_Axe, y + OU_SUIS_JE_Y_TTRIS))
        {
          return 1;
        }
      }
    }
  }
  return 0;
}

unsigned char Check_collision_y_TTRIS(int y_Axe)
{
  unsigned char x, y;
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      if (Piece_Mat2_TTRIS[x][y] == 1)
      {
        if (GRID_STAT_TTRIS(x + OU_SUIS_JE_X_TTRIS, (y + OU_SUIS_JE_Y_TTRIS) + y_Axe))
        {
          return 1;
        }
      }
    }
  }
  return 0;
}

void Move_Piece_TTRIS(void)
{
  Ou_suis_Je_TTRIS(xx_TTRIS, yy_TTRIS);
  if (OU_SUIS_JE_X_ENGAGED_TTRIS == 0)
  {
    if (Check_collision_x_TTRIS(DEPLACEMENT_XX_TTRIS))
    {
      DEPLACEMENT_XX_TTRIS = 0;
    }
  }
  if (DEPLACEMENT_XX_TTRIS == 1)
  {
    xx_TTRIS++;
  }
  if (DEPLACEMENT_XX_TTRIS == -1)
  {
    xx_TTRIS--;
  }
  Ou_suis_Je_TTRIS(xx_TTRIS, yy_TTRIS);
  if (OU_SUIS_JE_X_ENGAGED_TTRIS == 0)
  {
    DEPLACEMENT_XX_TTRIS = 0;
  }

  if (Check_collision_y_TTRIS(DEPLACEMENT_YY_TTRIS))
  {
    DEPLACEMENT_YY_TTRIS = 0;
    LONG_PRESS_X_TTRIS = 0;
    Ripple_filter_TTRIS = 0;
    DROP_BREAK_TTRIS = 6;
    Tiny_Flip_TTRIS(82); // add line for refresh screen at drop
  }
  else
  {
    DROP_BREAK_TTRIS = 0;
  }
  if (DROP_SPEED_TTRIS == 0)
  {
    if (DEPLACEMENT_YY_TTRIS == -1)
    {
      yy_TTRIS--;
    }
    if (DEPLACEMENT_YY_TTRIS == 1)
    {
      yy_TTRIS++;
    }
  }
  Ou_suis_Je_TTRIS(xx_TTRIS, yy_TTRIS);
  if (OU_SUIS_JE_Y_ENGAGED_TTRIS == 0)
  {
    DEPLACEMENT_YY_TTRIS = 0;
  }
}

void Ou_suis_Je_TTRIS(int xx_, int yy_)
{
  int xx_t, yy_t;
  xx_t = (((xx_) + 9) - 46);
  yy_t = (((yy_) + 9) - 5);
  OU_SUIS_JE_X_TTRIS = ((xx_t / 3) - 3);
  if ((xx_t % 3) != 0)
  {
    OU_SUIS_JE_X_ENGAGED_TTRIS = 1;
  }
  else
  {
    OU_SUIS_JE_X_ENGAGED_TTRIS = 0;
  }
  OU_SUIS_JE_Y_TTRIS = ((yy_t / 3) - 3);
  if ((yy_t) != ((OU_SUIS_JE_Y_TTRIS + 3) * 3))
  {
    OU_SUIS_JE_Y_ENGAGED_TTRIS = 1;
  }
  else
  {
    OU_SUIS_JE_Y_ENGAGED_TTRIS = 0;
  }
}

void Select_Piece_TTRIS(unsigned char Piece_)
{
  PIECEs_TTRIS = Piece_;
  switch (Piece_)
  {
  case 0:
    PIECEs_rot_TTRIS = 3;
    break;
  case 1:
    PIECEs_rot_TTRIS = 0;
    break;
  case 2:
  case 3:
  case 4:
    PIECEs_rot_TTRIS = 1;
    break;
  case 5:
  case 6:
    PIECEs_rot_TTRIS = 3;
    break;
  default:
    PIECEs_rot_TTRIS = 0;
    break;
  }
}

void rotate_Matrix_TTRIS(unsigned char ROT)
{
  unsigned char a_ = 0, b_ = 0, y, x;
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      switch (ROT)
      {
      case 0:
        a_ = x, b_ = y;
        break;
      case 1:
        a_ = 4 - y, b_ = x;
        break;
      case 2:
        a_ = 4 - x, b_ = 4 - y;
        break;
      case 3:
        a_ = y, b_ = 4 - x;
        break;
      default:
        break;
      }
      Piece_Mat2_TTRIS[a_][b_] = Scan_Piece_Matrix_TTRIS(x, y + (PIECEs_TTRIS * 5));
    }
  }
}

unsigned char Scan_Piece_Matrix_TTRIS(int x_Mat, int y_Mat)
{
  // unsigned char Result = (0b10000000 >> x_Mat) & (Pieces_TTRIS1[y_Mat]);
  unsigned char Result;
  Result = (0x80 >> x_Mat) & (Pieces_TTRIS1[y_Mat]);
  if (Result)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  return 0;
}

unsigned char GRID_STAT_TTRIS(int X_SCAN, int Y_SCAN)
{
  unsigned char Y_VAR_SELECT, Y_VAR_DECALAGE, COMP_BYTE_DECALAGE;
  if (Y_SCAN < 0)
    return 0;
  if ((X_SCAN < 0) || (X_SCAN > 11))
  {
    return 1;
  }
  if (Y_SCAN > 18)
  {
    return 1;
  }
  Y_VAR_SELECT = Y_SCAN >> 3;
  Y_VAR_DECALAGE = RecupeDecalageY_TTRIS(Y_SCAN);
  COMP_BYTE_DECALAGE = (0x80 >> Y_VAR_DECALAGE);
  // COMP_BYTE_DECALAGE = (0b10000000 >> Y_VAR_DECALAGE);
  if ((COMP_BYTE_DECALAGE & Grid_TTRIS[X_SCAN][Y_VAR_SELECT]) == 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
  return 0;
}

unsigned char CHANGE_GRID_STAT_TTRIS(int X_SCAN, int Y_SCAN, unsigned char VALUE)
{

  unsigned char Y_VAR_SELECT, Y_VAR_DECALAGE, COMP_BYTE_DECALAGE;
  if ((X_SCAN < 0) || (X_SCAN > 11))
    return 0;
  if ((Y_SCAN < 0) || (Y_SCAN > 18))
    return 0;
  Y_VAR_SELECT = Y_SCAN >> 3;
  Y_VAR_DECALAGE = RecupeDecalageY_TTRIS(Y_SCAN);
  COMP_BYTE_DECALAGE = (0x80 >> Y_VAR_DECALAGE);
  // COMP_BYTE_DECALAGE = (0b10000000 >> Y_VAR_DECALAGE);
  if (VALUE)
  {
    Grid_TTRIS[X_SCAN][Y_VAR_SELECT] = COMP_BYTE_DECALAGE | Grid_TTRIS[X_SCAN][Y_VAR_SELECT];
  }
  else
  {
    Grid_TTRIS[X_SCAN][Y_VAR_SELECT] = (0xff - COMP_BYTE_DECALAGE) & Grid_TTRIS[X_SCAN][Y_VAR_SELECT];
  }
  return 0;
}

unsigned char blitzSprite_TTRIS(int xPos, int yPos, unsigned char xPASS, unsigned char yPASS, unsigned char FRAME, const unsigned char *SPRITES)
{
  unsigned char OUTBYTE, WSPRITE, HSPRITE, Wmax, RECUPELINEY, SPRITEyLINE, SPRITEyDECALAGE, ScanA, ScanB, OUTBYTE2;
  unsigned int PICBYTE;
  WSPRITE = ((SPRITES[0]));
  HSPRITE = ((SPRITES[1]));
  Wmax = ((HSPRITE * WSPRITE) + 1);
  PICBYTE = FRAME * (Wmax - 1);
  RECUPELINEY = RecupeLineY_TTRIS(yPos);
  if ((xPASS > ((xPos + (WSPRITE - 1)))) || (xPASS < xPos) || ((RECUPELINEY > yPASS) || ((RECUPELINEY + (HSPRITE)) < yPASS)))
  {
    return 0x00;
  }
  SPRITEyLINE = (yPASS - (RECUPELINEY));
  SPRITEyDECALAGE = (RecupeDecalageY_TTRIS(yPos));
  ScanA = (((xPASS - xPos) + (SPRITEyLINE * WSPRITE)) + 2);
  ScanB = (((xPASS - xPos) + ((SPRITEyLINE - 1) * WSPRITE)) + 2);
  if (ScanA > Wmax)
  {
    OUTBYTE = 0x00;
  }
  else
  {
    OUTBYTE = SplitSpriteDecalageY_TTRIS(SPRITEyDECALAGE, (SPRITES[ScanA + (PICBYTE)]), 1);
  }
  if ((SPRITEyLINE > 0))
  {
    OUTBYTE2 = SplitSpriteDecalageY_TTRIS(SPRITEyDECALAGE, (SPRITES[ScanB + (PICBYTE)]), 0);
    if (ScanB > Wmax)
    {
      return OUTBYTE;
    }
    else
    {
      return OUTBYTE | OUTBYTE2;
    }
  }
  else
  {
    return OUTBYTE;
  }
}

unsigned char H_grid_Scan_TTRIS(unsigned char xPASS)
{
  return (H_Grid_TTTRIS[xPASS - 46]);
}

unsigned char Recupe_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  unsigned char BYTE_TTRIS = 0;
  unsigned char x = 0, y;
  for (y = MEM_TTTRIS[(yPASS << 1)]; y < MEM_TTTRIS[(yPASS << 1) + 1]; y++)
  {
    if ((xPASS > 45) && (xPASS < 82))
    {
      x = H_grid_Scan_TTRIS(xPASS);
    }
    else
    {
      return (RECUPE_BACKGROUND_TTRIS(xPASS, yPASS) |
              NEXT_BLOCK_TTRIS(xPASS, yPASS) |
              recupe_Nb_of_line_TTRIS(xPASS, yPASS) |
              recupe_SCORES_TTRIS(xPASS, yPASS) |
              recupe_LEVEL_TTRIS(xPASS, yPASS));
    }
    if (GRID_STAT_TTRIS(x, y) == 1)
    {
      BYTE_TTRIS = BYTE_TTRIS | blitzSprite_TTRIS(46 + (x * 3), 5 + (y * 3), xPASS, yPASS, 0, tinyblock_TTTRIS);
    }
  }
  return RECUPE_BACKGROUND_TTRIS(xPASS, yPASS) | BYTE_TTRIS | DropPiece_TTRIS(xPASS, yPASS);
}

unsigned char NEXT_BLOCK_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  unsigned char y = 0, x = 0;
  if (xPASS > 89)
  {
    unsigned char Byte_Mem = 0;
    unsigned char x_add = 0;
    unsigned char y_add = 0;
    switch (PIECEs_TTRIS_PREVIEW)
    {
    case 0:
      x_add = 1;
      y_add = 1;
      break;
    case 1:
      y_add = -1;
      break;
    case 2:
      x_add = 1;
      break;
    case 3:
      break;
    case 4:
      x_add = 1;
      y_add = 1;
      break;
    case 5:
      x_add = 1;
      break;
    case 6:
      break;
    default:
      break;
    }

    for (y = 0; y < 5; y++)
    {
      for (x = 0; x < 5; x++)
      {
        if (Scan_Piece_Matrix_TTRIS(x, y + (PIECEs_TTRIS_PREVIEW * 5)) == 1)
        {
          Byte_Mem |= blitzSprite_TTRIS(92 + (x * 2) + x_add, (27 + (y * 2)) - 5 + y_add, xPASS, yPASS, 0, tiny_PREVIEW_block_TTTRIS);
        }
      }
    }
    return Byte_Mem;
  }
  return 0;
}

unsigned char RECUPE_BACKGROUND_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  return (BACKGROUND_TTRIS[xPASS + (yPASS * 128)]);
}

unsigned char DropPiece_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  unsigned char Byte_Mem = 0, y = 0, x = 0;
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      if (Piece_Mat2_TTRIS[x][y] == 1)
      {
        Byte_Mem |= blitzSprite_TTRIS(xx_TTRIS + (x * 3), (yy_TTRIS + (y * 3)) - 5, xPASS, yPASS, 0, tinyblock2_TTTRIS);
      }
    }
  }
  return Byte_Mem;
}

unsigned char SplitSpriteDecalageY_TTRIS(unsigned char decalage, unsigned char Input, unsigned char UPorDOWN)
{
  if (UPorDOWN)
  {
    return Input << decalage;
  }
  return Input >> (8 - decalage);
}

unsigned char RecupeLineY_TTRIS(unsigned char Valeur)
{
  return (Valeur >> 3);
}

unsigned char RecupeDecalageY_TTRIS(unsigned char Valeur)
{
  return (Valeur - ((Valeur >> 3) << 3));
}

void Tiny_Flip_TTRIS(unsigned char HR_TTRIS)
{
  unsigned char y, x;
  for (y = 0; y < 8; y++)
  {
    JOY_OLED_data_start(y);
    for (x = 0; x < HR_TTRIS; x++)
    {
      JOY_OLED_send(Recupe_TTRIS(x, y));
    }
    JOY_OLED_end();
  }
}

void Flip_intro_TTRIS(unsigned char *TIMER1)
{
  unsigned char y, x;
  for (y = 0; y < 8; y++)
  {
    JOY_OLED_data_start(y);
    for (x = 0; x < 128; x++)
    {
      JOY_OLED_send(intro_TTRIS(x, y, TIMER1));
    }
    JOY_OLED_end();
  }
}

unsigned char intro_TTRIS(unsigned char xPASS, unsigned char yPASS, unsigned char *TIMER1)
{
  return (RECUPE_BACKGROUND_TTRIS(xPASS, yPASS) |
          recupe_Chateau_TTRIS(xPASS, yPASS) |
          Recupe_Start_TTRIS(xPASS, yPASS, TIMER1) |
          recupe_SCORES_TTRIS(xPASS, yPASS) |
          recupe_Nb_of_line_TTRIS(xPASS, yPASS) |
          recupe_SCORES_TTRIS(xPASS, yPASS) |
          recupe_LEVEL_TTRIS(xPASS, yPASS));
}

unsigned char Recupe_Start_TTRIS(unsigned char xPASS, unsigned char yPASS, unsigned char *TIMER1)
{
  if (*TIMER1 > 3)
  {
    return blitzSprite_TTRIS(49, 28, xPASS, yPASS, 0, start_button_1_TTRIS) | blitzSprite_TTRIS(49, 36, xPASS, yPASS, 0, start_button_2_TTRIS);
  }
  else
  {
    return 0;
  }
}

unsigned char recupe_Chateau_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  if (xPASS < 46)
    return 0;
  if (xPASS > 81)
    return 0;
  return (chateau_TTRIS[(xPASS - 46) + (yPASS * 36)]);
}

unsigned char recupe_SCORES_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  if (xPASS < 95)
  {
    return 0;
  }
  if (xPASS > 119)
  {
    return 0;
  }
  if (yPASS > 1)
  {
    return 0;
  }
#define M10000 (Scores_TTRIS / 10000)
#define M1000 (((Scores_TTRIS) - (M10000 * 10000)) / 1000)
#define M100 (((Scores_TTRIS) - (M1000 * 1000) - (M10000 * 10000)) / 100)
#define M10 (((Scores_TTRIS) - (M100 * 100) - (M1000 * 1000) - (M10000 * 10000)) / 10)
#define M1 ((Scores_TTRIS) - (M10 * 10) - (M100 * 100) - (M1000 * 1000) - (M10000 * 10000))
  return (blitzSprite_TTRIS(95, 8, xPASS, yPASS, M10000, police_TTRIS) |
          blitzSprite_TTRIS(99, 8, xPASS, yPASS, M1000, police_TTRIS) |
          blitzSprite_TTRIS(103, 8, xPASS, yPASS, M100, police_TTRIS) |
          blitzSprite_TTRIS(107, 8, xPASS, yPASS, M10, police_TTRIS) |
          blitzSprite_TTRIS(111, 8, xPASS, yPASS, M1, police_TTRIS) |
          blitzSprite_TTRIS(115, 8, xPASS, yPASS, 0, police_TTRIS));
}

void Convert_Nb_of_line_TTRIS(void)
{
  Nb_of_line_TTRIS[2] = (Nb_of_line_F_TTRIS / 100);
  Nb_of_line_TTRIS[1] = ((Nb_of_line_F_TTRIS - (Nb_of_line_TTRIS[2] * 100)) / 10);
  Nb_of_line_TTRIS[0] = (Nb_of_line_F_TTRIS - (Nb_of_line_TTRIS[2] * 100) - (Nb_of_line_TTRIS[1] * 10));
}

unsigned char recupe_Nb_of_line_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  if (xPASS < 16)
  {
    return 0;
  }
  if (xPASS > 28)
  {
    return 0;
  }
  if (yPASS > 1)
  {
    return 0;
  }
  return (blitzSprite_TTRIS(16, 8, xPASS, yPASS, Nb_of_line_TTRIS[2], police_TTRIS) |
          blitzSprite_TTRIS(20, 8, xPASS, yPASS, Nb_of_line_TTRIS[1], police_TTRIS) |
          blitzSprite_TTRIS(24, 8, xPASS, yPASS, Nb_of_line_TTRIS[0], police_TTRIS));
}

unsigned char recupe_LEVEL_TTRIS(unsigned char xPASS, unsigned char yPASS)
{
  if (xPASS < 109)
  {
    return 0;
  }
  if (xPASS > 118)
  {
    return 0;
  }
  if (yPASS != 5)
  {
    return 0;
  }
  return (blitzSprite_TTRIS(109, 41, xPASS, yPASS, (Level_TTRIS / 10), police_TTRIS) |
          blitzSprite_TTRIS(114, 41, xPASS, yPASS, (Level_TTRIS % 10), police_TTRIS));
}

void INIT_ALL_VAR_TTRIS(void)
{
  unsigned char y = 0, x = 0;
  for (y = 0; y < 3; y++)
  {
    for (x = 0; x < 12; x++)
    {
      Grid_TTRIS[x][y] = 0;
    }
  }
  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      Piece_Mat2_TTRIS[x][y] = 0;
    }
  }
  LONG_PRESS_X_TTRIS = 0;
  DOWN_DESACTIVE_TTRIS = 0;
  DROP_SPEED_TTRIS = 0;
  SPEED_x_trig_TTRIS = 0;
  DROP_TRIG_TTRIS = 1;
  xx_TTRIS = 0;
  yy_TTRIS = 0;
  Ripple_filter_TTRIS = 0;
  PIECEs_TTRIS = 0;
  PIECEs_TTRIS_PREVIEW = 0;
  PIECEs_rot_TTRIS = 0;
  DROP_BREAK_TTRIS = 0;
  OU_SUIS_JE_X_TTRIS = 0;
  OU_SUIS_JE_Y_TTRIS = 0;
  OU_SUIS_JE_X_ENGAGED_TTRIS = 0;
  OU_SUIS_JE_Y_ENGAGED_TTRIS = 0;
  DEPLACEMENT_XX_TTRIS = 0;
  DEPLACEMENT_YY_TTRIS = 0;
}

#define Number_of_Backup 4

void recupe_HIGHSCORE_TTRIS(void)
{

  Reset_Value_TTRIS();
}

void Reset_Value_TTRIS(void)
{
  Level_TTRIS = 0;
  Nb_of_line_F_TTRIS = 0;
  Scores_TTRIS = 0;
}

void save_HIGHSCORE_TTRIS(void)
{
}

void Check_NEW_RECORD(void)
{
}

unsigned char checksum(unsigned char Byte_)
{
  unsigned char Total_ = 0;
  unsigned char t = 0;
  for (t = 0; t < 8; t++)
  {
    if ((Byte_) & (0x01 << t))
    {
      Total_++;
    }
  }
  return Total_;
}
/* USER CODE END 4 */

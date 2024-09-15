#include "OLED2.H"
#include "Key.H"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
extern TaskHandle_t game1_Handler;
extern TaskHandle_t game2_Handler;
extern TaskHandle_t game3_Handler;
extern TaskHandle_t game4_Handler;
extern TaskHandle_t menuTask_Handler;
extern TaskStatus_t game1_state;
extern TaskStatus_t game2_state;
extern TaskStatus_t game3_state;
extern TaskStatus_t game4_state;
char text[30];
#define text_height 8
#define half 64
int infos(){
    B_OLED_Clear();
    vTaskGetInfo(game1_Handler, &game1_state, pdTRUE, eInvalid);
    vTaskGetInfo(game2_Handler, &game2_state, pdTRUE, eInvalid);
    vTaskGetInfo(game3_Handler, &game3_state, pdTRUE, eInvalid);
    vTaskGetInfo(game4_Handler, &game4_state, pdTRUE, eInvalid);

    B_OLED_ShowString(0, 0, "game1", B_OLED_6X8);
    switch (game1_state.eCurrentState)
    {
    case eRunning:
        B_OLED_ShowString(0, text_height+1, "eRunning", B_OLED_6X8);
        break;
    case eReady:
        B_OLED_ShowString(0, text_height + 1, "eReady", B_OLED_6X8);
        break;
    case eBlocked:
        B_OLED_ShowString(0, text_height + 1, "eBlocked", B_OLED_6X8);
        break;
    case eSuspended:
        B_OLED_ShowString(0, text_height + 1, "eSuspended", B_OLED_6X8);
        break;
    case eDeleted:
        B_OLED_ShowString(0, text_height + 1, "eDeleted", B_OLED_6X8);
        break;
    case eInvalid:
        B_OLED_ShowString(0, text_height + 1, "eInvalid", B_OLED_6X8);
        break;
    default:
        break;
    }
    sprintf(text, "pro:%1d", game1_state.uxBasePriority);
    B_OLED_ShowString(0, 2 * text_height + 1, text, B_OLED_6X8);
    // sprintf(text, "stack:%1d", game1_state.usStackHighWaterMark);
    // B_OLED_ShowString(0, 3 * text_height + 1, text, B_OLED_6X8);

    B_OLED_ShowString(0, 4 * text_height + 1, "game2", B_OLED_6X8);
    switch (game2_state.eCurrentState)
    {
    case eRunning:
        B_OLED_ShowString(0, 5 * text_height + 1, "eRunning", B_OLED_6X8);
        break;
    case eReady:
        B_OLED_ShowString(0, 5 * text_height + 1, "eReady", B_OLED_6X8);
        break;
    case eBlocked:
        B_OLED_ShowString(0, 5 * text_height + 1, "eBlocked", B_OLED_6X8);
        break;
    case eSuspended:
        B_OLED_ShowString(0, 5 * text_height + 1, "eSuspended", B_OLED_6X8);
        break;
    case eDeleted:
        B_OLED_ShowString(0, 5 * text_height + 1, "eDeleted", B_OLED_6X8);
        break;
    case eInvalid:
        B_OLED_ShowString(0, 5 * text_height + 1, "eInvalid", B_OLED_6X8);
        break;
    default:
        break;
    }
    sprintf(text, "pro:%1d", game2_state.uxBasePriority);
    B_OLED_ShowString(0, 6 * text_height + 1, text, B_OLED_6X8);
    // sprintf(text, "stack:%1d", game2_state.usStackHighWaterMark);
    // B_OLED_ShowString(0, 7 * text_height + 1, text, B_OLED_6X8);

    B_OLED_ShowString(half, 0, "game3", B_OLED_6X8);
    switch (game3_state.eCurrentState)
    {
    case eRunning:
        B_OLED_ShowString(half, text_height + 1, "eRunning", B_OLED_6X8);
        break;
    case eReady:
        B_OLED_ShowString(half, text_height + 1, "eReady", B_OLED_6X8);
        break;
    case eBlocked:
        B_OLED_ShowString(half, text_height + 1, "eBlocked", B_OLED_6X8);
        break;
    case eSuspended:
        B_OLED_ShowString(half, text_height + 1, "eSuspended", B_OLED_6X8);
        break;
    case eDeleted:
        B_OLED_ShowString(half, text_height + 1, "eDeleted", B_OLED_6X8);
        break;
    case eInvalid:
        B_OLED_ShowString(half, text_height + 1, "eInvalid", B_OLED_6X8);
        break;
    default:
        break;
    }
    sprintf(text, "pro:%1d", game3_state.uxBasePriority);
    B_OLED_ShowString(half, 2 * text_height + 1, text, B_OLED_6X8);
    // sprintf(text, "stack:%1d", game3_state.usStackHighWaterMark);
    // B_OLED_ShowString(half, 3 * text_height + 1, text, B_OLED_6X8);

    B_OLED_ShowString(half, 4 * text_height + 1, "game4", B_OLED_6X8);
    switch (game4_state.eCurrentState)
    {
    case eRunning:
        B_OLED_ShowString(half, 5 * text_height + 1, "eRunning", B_OLED_6X8);
        break;
    case eReady:
        B_OLED_ShowString(half, 5 * text_height + 1, "eReady", B_OLED_6X8);
        break;
    case eBlocked:
        B_OLED_ShowString(half, 5 * text_height + 1, "eBlocked", B_OLED_6X8);
        break;
    case eSuspended:
        B_OLED_ShowString(half, 5 * text_height + 1, "eSuspended", B_OLED_6X8);
        break;
    case eDeleted:
        B_OLED_ShowString(half, 5 * text_height + 1, "eDeleted", B_OLED_6X8);
        break;
    case eInvalid:
        B_OLED_ShowString(half, 5 * text_height + 1, "eInvalid", B_OLED_6X8);
        break;
    default:
        break;
    }
    sprintf(text, "pro:%1d", game4_state.uxBasePriority);
    B_OLED_ShowString(half, 6 * text_height + 1, text, B_OLED_6X8);
    // sprintf(text, "stack:%1d", game4_state.usStackHighWaterMark);
    // B_OLED_ShowString(half, 7 * text_height + 1, text, B_OLED_6X8);


    B_OLED_Update();
    while (1)
    {
        if (get_key_status() == sw1_key_value)
        {
            vTaskResume(menuTask_Handler);
            vTaskSuspend(game4_Handler);

        }
        else if (get_key_status() == sw2_key_value)
        {
            vTaskResume(menuTask_Handler);
            vTaskDelete(game4_Handler);

        }
    }
    
}
#include "menu.h"
#include "OLED2.h"
#include "Key.h"
#include "FreeRTOS.h"
#include "task.h"
#define distance_y 20
#define distance_x 5
#define distance_y2 5
#define length_y 20
#define length_y2 15
#define length_x 25
#define offset_state_x 8
#define offset_state_y 15
#define offset_state_x2 60
#define offset_state_y2 5
int select = 1;
int menu_key_num = 0;
extern TaskHandle_t game1_Handler;
extern TaskHandle_t game2_Handler;
extern TaskHandle_t game3_Handler;
extern TaskHandle_t game4_Handler;
extern TaskHandle_t menuTask_Handler;
extern TaskHandle_t info_Handler;
extern TaskStatus_t game1_state;
extern TaskStatus_t game2_state;
extern TaskStatus_t game3_state;
extern TaskStatus_t game4_state;
extern TaskStatus_t info_state;
char text_menu[30];
int menu()
{

    while (1)
    {
        vTaskGetInfo(game1_Handler, &game1_state, pdTRUE, eInvalid);
        vTaskGetInfo(game2_Handler, &game2_state, pdTRUE, eInvalid);
        vTaskGetInfo(game3_Handler, &game3_state, pdTRUE, eInvalid);
        vTaskGetInfo(game4_Handler, &game4_state, pdTRUE, eInvalid);
        vTaskGetInfo(info_Handler, &info_state, pdTRUE, eInvalid);
        B_OLED_Clear();
        B_OLED_ShowString(14, 20, "1", B_OLED_8X16);
        B_OLED_ShowString(14 + (length_x + distance_x) * 1, 20, "2", B_OLED_8X16);
        B_OLED_ShowString(14 + (length_x + distance_x) * 2, 20, "3", B_OLED_8X16);
        B_OLED_ShowString(14 + (length_x + distance_x) * 3, 20, "4", B_OLED_8X16);
        B_OLED_ShowString(35, 50, "freeRTOS", B_OLED_6X8);

        B_OLED_DrawRectangle(distance_x, distance_y, length_x, length_y, B_OLED_UNFILLED);
        B_OLED_DrawRectangle(distance_x + length_x + distance_x, distance_y, length_x, length_y, B_OLED_UNFILLED);
        B_OLED_DrawRectangle(distance_x + (length_x + distance_x) * 2, distance_y, length_x, length_y, B_OLED_UNFILLED);
        B_OLED_DrawRectangle(distance_x + (length_x + distance_x) * 3, distance_y, length_x, length_y, B_OLED_UNFILLED);
        B_OLED_DrawRectangle(distance_x, length_y + distance_y2 + distance_y, length_x + (distance_x + length_x) * 3, length_y2, B_OLED_UNFILLED);

        if ((game1_state.eCurrentState == eRunning) || (game1_state.eCurrentState == eSuspended))
        {
            B_OLED_DrawCircle(15 + offset_state_x, 20 + offset_state_y, 2, B_OLED_UNFILLED);
            if (select == 1)
                B_OLED_ShowString(110, 0, "run", B_OLED_6X8);
        }
        if ((game2_state.eCurrentState == eRunning) || (game2_state.eCurrentState == eSuspended))
        {
            B_OLED_DrawCircle(15 + (length_x + distance_x) * 1 + offset_state_x, 20 + offset_state_y, 2, B_OLED_UNFILLED);
            if (select == 2)
                B_OLED_ShowString(110, 0, "run", B_OLED_6X8);
        }
        if ((game3_state.eCurrentState == eRunning) || (game3_state.eCurrentState == eSuspended))
        {
            B_OLED_DrawCircle(15 + (length_x + distance_x) * 2 + offset_state_x, 20 + offset_state_y, 2, B_OLED_UNFILLED);
            if (select == 3)
                B_OLED_ShowString(110, 0, "run", B_OLED_6X8);
        }
        if ((game4_state.eCurrentState == eRunning) || (game4_state.eCurrentState == eSuspended))
        {
            B_OLED_DrawCircle(15 + (length_x + distance_x) * 3 + offset_state_x, 20 + offset_state_y, 2, B_OLED_UNFILLED);
            if (select == 4)
                B_OLED_ShowString(110, 0, "run", B_OLED_6X8);
        }
        if ((info_state.eCurrentState == eRunning) || (info_state.eCurrentState == eSuspended))
        {
            B_OLED_DrawCircle(35 + offset_state_x2, 50 + offset_state_y2, 2, B_OLED_UNFILLED);
            if (select == 5)
                B_OLED_ShowString(110, 0, "run", B_OLED_6X8);
        }
        if (select == 1)
        {
            sprintf(text_menu, "Menu:dragon");
            B_OLED_ReverseArea(distance_x, distance_y, length_x, length_y);
        }
        else if (select == 2)
        {
            sprintf(text_menu, "Menu:war");
            B_OLED_ReverseArea(distance_x + length_x + distance_x, distance_y, length_x, length_y);
        }
        else if (select == 3)
        {
            sprintf(text_menu, "Menu:bird");
            B_OLED_ReverseArea(distance_x + (length_x + distance_x) * 2, distance_y, length_x, length_y);
        }
        else if (select == 4)
        {
            sprintf(text_menu, "Menu:block");
            B_OLED_ReverseArea(distance_x + (length_x + distance_x) * 3, distance_y, length_x, length_y);
        }
        else if (select == 5)
        {
            sprintf(text_menu, "Menu:info");
            B_OLED_ReverseArea(distance_x, length_y + distance_y2 + distance_y, length_x + (distance_x + length_x) * 3, length_y2);
        }
        B_OLED_ShowString(0, 0, text_menu, B_OLED_8X16);

        // B_OLED_ShowNum(0, 0, menu_key_num, 1, B_OLED_6X8);
        menu_key_num = Get_Key();
        if (menu_key_num != 0)
        {
            if ((menu_key_num == r_push_key_value) || (menu_key_num == l_push_key_value))
            {
                return select;
            }
            switch (menu_key_num)
            {
            case r_right_key_value:
                select++;
                break;
            case r_left_key_value:
                select--;
                break;
            case r_back_key_value:
                select += 4;
                break;
            case r_front_key_value:
                select -= 4;
            case l_right_key_value:
                select++;
                break;
            case l_left_key_value:
                select--;
                break;
            case l_back_key_value:
                select += 4;
                break;
            case l_front_key_value:
                select -= 4;
            default:
                break;
            }
            if (select > 5)
                select = 5;
            if (select < 1)
                select = 1;

            while (menu_key_num != 0)
            {
                menu_key_num = Get_Key();
            }
        }

        B_OLED_Update();
    }
}
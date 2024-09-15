#ifndef __GAME1_H
#define __GAME1_H
void Buzzer_Init();    // 蜂鸣器初始化

void Buzzer(unsigned int SW);
void bostacle_refresh(void); // 随机障碍

void Show_other(void); // 显示云、地面、分数
void loop_dead(void); // 检测恐龙是否撞到障碍


void A_OLED_DeadAnimation(unsigned int newbest); 
void live(void); // 游戏页面
void die(void); // 碰撞障碍后
void Menu1(void); // 返回游戏
void Menu2(void); // 设置初始速度
void Menu3(void); // 设置速度上限
void Menu4(void); // 蜂鸣器开关
void Menu5(void);// 结算动画开关
void Menu6(void); // 关机

void Menu(void); // 菜单
int game1(void);
#endif
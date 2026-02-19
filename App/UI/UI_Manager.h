#ifndef __UI_MANAGER_H
#define __UI_MANAGER_H

#include "stm32f10x.h"

/* 页面状态枚举 */
typedef enum {
    PAGE_CLOCK,         // 时钟页面
    PAGE_MENU,          // 菜单页面
    PAGE_STOPWATCH,     // 秒表页面
    PAGE_LED,           // LED控制页面
    PAGE_MPU6050,       // 姿态显示页面
    PAGE_GAME,          // 恐龙游戏页面
    PAGE_EMOJI,         // 表情页面
    PAGE_GRADIENTER,    // 水平仪页面
    PAGE_SETTING,       // 设置页面
    PAGE_STEPS          // 计步器页面
} PageState_t;

void UI_Init(void);
void UI_Refresh(void);
void UI_HandleKey(uint8_t KeyNum);
PageState_t UI_GetCurrentPage(void);
void UI_SwitchPage(PageState_t page); // 新增：允许页面请求切换

#endif

#include "stm32f10x.h"
#include "bsp_oled.h"
#include "bsp_rtc.h"
#include "FreeRTOS.h"
#include "task.h"

// 设置时间状态枚举
typedef enum {
    SET_RETURN = 0,
    SET_YEAR = 1,
    SET_MONTH = 2,
    SET_DAY = 3,
    SET_HOUR = 4,
    SET_MIN = 5,
    SET_SEC = 6
} SetTimeState_t;

static SetTimeState_t currentSetState = SET_RETURN;
static uint8_t editingValue = 0;  // 是否正在编辑数值

/**
  * @brief  显示设置日期界面
  */
static void Show_SetDate_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_Printf(0, 16, OLED_8X16, "年:%4d", MyRTC_Time[0]);
    OLED_Printf(0, 32, OLED_8X16, "月:%2d", MyRTC_Time[1]);
    OLED_Printf(0, 48, OLED_8X16, "日:%2d", MyRTC_Time[2]);
}

/**
  * @brief  显示设置时间界面
  */
static void Show_SetClock_UI(void)
{
    OLED_ShowImage(0, 0, 16, 16, Return);
    OLED_Printf(0, 16, OLED_8X16, "时:%2d", MyRTC_Time[3]);
    OLED_Printf(0, 32, OLED_8X16, "分:%2d", MyRTC_Time[4]);
    OLED_Printf(0, 48, OLED_8X16, "秒:%2d", MyRTC_Time[5]);
}

/**
  * @brief  调整RTC时间值
  * @param  index: 时间索引 (0-5分别代表年月日时分秒)
  * @param  increment: 1=增加, 0=减少
  */
static void Adjust_RTC_Value(uint8_t index, uint8_t increment)
{
    int16_t value = MyRTC_Time[index];
    
    if(increment) {
        value++;
    } else {
        value--;
    }
    
    // 边界处理
    switch(index) {
        case 0: // 年 (2000-2099)
            if(value < 2000) value = 2099;
            if(value > 2099) value = 2000;
            break;
        case 1: // 月 (1-12)
            if(value < 1) value = 12;
            if(value > 12) value = 1;
            break;
        case 2: // 日 (1-31)
            if(value < 1) value = 31;
            if(value > 31) value = 1;
            break;
        case 3: // 时 (0-23)
            if(value < 0) value = 23;
            if(value > 23) value = 0;
            break;
        case 4: // 分 (0-59)
        case 5: // 秒 (0-59)
            if(value < 0) value = 59;
            if(value > 59) value = 0;
            break;
    }
    
    MyRTC_Time[index] = value;
    MyRTC_SetTime();
}

/**
  * @brief  设置时间按键处理
  * @param  KeyNum: 1=上/加, 2=下/减, 3=确定
  * @retval 0=继续设置, 1=退出设置
  */
uint8_t SetTime_HandleKey(uint8_t KeyNum)
{
    if(editingValue) {
        // 正在编辑数值
        if(KeyNum == 1) {
            // 数值减
            Adjust_RTC_Value(currentSetState - 1, 0);
        }
        else if(KeyNum == 2) {
            // 数值加
            Adjust_RTC_Value(currentSetState - 1, 1);
        }
        else if(KeyNum == 3) {
            // 确认修改
            editingValue = 0;
        }
    }
    else {
        // 导航模式
        if(KeyNum == 1) {
            // 上一个选项
            if(currentSetState == SET_RETURN) {
                currentSetState = SET_SEC;
            } else {
                currentSetState--;
            }
        }
        else if(KeyNum == 2) {
            // 下一个选项
            if(currentSetState == SET_SEC) {
                currentSetState = SET_RETURN;
            } else {
                currentSetState++;
            }
        }
        else if(KeyNum == 3) {
            // 进入编辑或返回
            if(currentSetState == SET_RETURN) {
                // 返回上一级
                currentSetState = SET_RETURN;
                return 1;  // 退出设置
            } else {
                // 开始编辑
                editingValue = 1;
            }
        }
    }
    
    return 0;  // 继续运行
}

/**
  * @brief  刷新设置时间界面
  */
void SetTime_RefreshDisplay(void)
{
    OLED_Clear();
    
    // 根据状态显示不同界面
    if(currentSetState <= SET_DAY) {
        Show_SetDate_UI();
    } else {
        Show_SetClock_UI();
    }
    
    // 处理反色显示
    if(editingValue) {
        // 编辑状态下反色数值
        switch(currentSetState) {
            case SET_YEAR:
                OLED_ReverseArea(24, 16, 32, 16);
                break;
            case SET_MONTH:
                OLED_ReverseArea(24, 32, 16, 16);
                break;
            case SET_DAY:
                OLED_ReverseArea(24, 48, 16, 16);
                break;
            case SET_HOUR:
                OLED_ReverseArea(24, 16, 16, 16);
                break;
            case SET_MIN:
                OLED_ReverseArea(24, 32, 16, 16);
                break;
            case SET_SEC:
                OLED_ReverseArea(24, 48, 16, 16);
                break;
            default:
                break;
        }
    } else {
        // 导航状态下反色标签
        switch(currentSetState) {
            case SET_RETURN:
                if(currentSetState <= SET_DAY) {
                    OLED_ReverseArea(0, 0, 16, 16);
                } else {
                    OLED_ReverseArea(0, 0, 16, 16);
                }
                break;
            case SET_YEAR:
                OLED_ReverseArea(0, 16, 16, 16);
                break;
            case SET_MONTH:
                OLED_ReverseArea(0, 32, 16, 16);
                break;
            case SET_DAY:
                OLED_ReverseArea(0, 48, 16, 16);
                break;
            case SET_HOUR:
                OLED_ReverseArea(0, 16, 16, 16);
                break;
            case SET_MIN:
                OLED_ReverseArea(0, 32, 16, 16);
                break;
            case SET_SEC:
                OLED_ReverseArea(0, 48, 16, 16);
                break;
        }
    }
    
    OLED_Update();
}

/**
  * @brief  初始化设置时间模块
  */
void SetTime_Init(void)
{
    currentSetState = SET_RETURN;
    editingValue = 0;
}

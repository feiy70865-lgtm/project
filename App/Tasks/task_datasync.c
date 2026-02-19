#include "task_common.h"
#include "app_config.h"
#include "bsp_rtc.h"
#include "Storage.h"
#include "step_algo.h"
#include "debug_serial.h"

/**
  * @brief  数据同步任务 - 串口通信与数据保存
  * @param  argument: 未使用
  * @retval None
  */
void DataSyncTask(void *argument)
{
    static uint8_t last_day = 0;
    extern int MyRTC_Time[]; // 引用外部变量
    
    while(1)
    {
        /* 1. 检查日期变更，保存步数 */
        MyRTC_ReadTime();
        if (last_day != MyRTC_Time[2] && last_day != 0) // 日期变化
        {
            Storage_SaveDailySteps(MyRTC_Time[0], MyRTC_Time[1], last_day, StepCounter_GetTodaySteps());
            StepCounter_ResetToday();
        }
        last_day = MyRTC_Time[2];
        
        /* 2. 简单的串口命令处理 (示例) */
        /* 每隔5秒发送一次当前步数到蓝牙 */
        Serial_Printf("Time: %02d:%02d:%02d Steps: %d\r\n", 
                      MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5], 
                      StepCounter_GetTodaySteps());
                      
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

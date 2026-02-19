#include "task_common.h"
#include "app_config.h"
#include "UI_Manager.h"
#include "bsp_oled.h"

/* 电源管理变量 */
volatile TickType_t LastActionTime = 0;
volatile uint8_t IsScreenOn = 1;

/**
  * @brief  显示任务 - 负责刷新OLED屏幕
  * @param  argument: 未使用
  * @retval None
  */
void DisplayTask(void *argument)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(DISPLAY_REFRESH_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
    
    while(1)
    {
        /* 检查自动熄屏 */
        if (IsScreenOn && (xTaskGetTickCount() - LastActionTime > pdMS_TO_TICKS(OLED_AUTO_OFF_TIMEOUT * 1000))) {
            IsScreenOn = 0;
            if(xSemaphoreTake(OLEDMutex, portMAX_DELAY) == pdTRUE) {
                OLED_Clear();
                OLED_Update();
                xSemaphoreGive(OLEDMutex);
            }
        }

        if (!IsScreenOn) {
            vTaskDelay(pdMS_TO_TICKS(500)); // 熄屏时降低刷新率
            continue;
        }

        /* 获取OLED互斥信号量 */
        if(xSemaphoreTake(OLEDMutex, portMAX_DELAY) == pdTRUE)
        {
            /* 调用UI管理器刷新界面 */
            UI_Refresh();
            
            OLED_Update();
            xSemaphoreGive(OLEDMutex); // 释放互斥信号量
        }
        
        vTaskDelayUntil(&xLastWakeTime, xFrequency); // 精确延时
    }
}

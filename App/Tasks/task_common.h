#ifndef __TASK_COMMON_H
#define __TASK_COMMON_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* 任务句柄 */
extern TaskHandle_t StartTaskHandle;
extern TaskHandle_t DisplayTaskHandle;
extern TaskHandle_t KeyTaskHandle;
extern TaskHandle_t MPU6050TaskHandle;
extern TaskHandle_t DataSyncTaskHandle;

/* 队列句柄 */
extern QueueHandle_t KeyQueue;

/* 互斥信号量 */
extern SemaphoreHandle_t OLEDMutex;

/* 全局标志位 */
extern uint8_t MPU6050_Available;
extern volatile uint8_t IsScreenOn;
extern volatile TickType_t LastActionTime;

/* 外部接口函数 */
void SendKeyToQueue(uint8_t KeyNum);

#endif

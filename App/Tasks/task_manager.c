#include "task_common.h"
#include "app_config.h"
#include "DataCenter.h"
#include "UI_Manager.h"
#include "debug_serial.h"

/* 任务句柄定义 */
TaskHandle_t StartTaskHandle = NULL;
TaskHandle_t DisplayTaskHandle = NULL;
TaskHandle_t KeyTaskHandle = NULL;
TaskHandle_t MPU6050TaskHandle = NULL;
TaskHandle_t DataSyncTaskHandle = NULL;

/* 队列句柄定义 */
QueueHandle_t KeyQueue = NULL;

/* 互斥信号量定义 */
SemaphoreHandle_t OLEDMutex = NULL;

/* 外部任务函数声明 */
extern void DisplayTask(void *argument);
extern void KeyProcessTask(void *argument);
extern void MPU6050Task(void *argument);
extern void DataSyncTask(void *argument);

/**
  * @brief  开始任务 - 创建其他应用任务
  * @param  argument: 未使用
  * @retval None
  */
void StartTask(void *argument)
{
    taskENTER_CRITICAL(); // 进入临界区
    
    /* 初始化数据中心 */
    DataCenter_Init();

    /* 初始化UI */
    UI_Init();
    
    /* 初始化串口 (蓝牙) */
    Serial_Init();
    
    /* 创建按键消息队列 */
    KeyQueue = xQueueCreate(QUEUE_LENGTH_KEY, QUEUE_ITEM_SIZE_KEY);
    if(KeyQueue == NULL)
    {
        /* 队列创建失败，通常是内存不足 */
        while(1); // 停在此处调试
    }
    
    /* 创建OLED互斥信号量 */
    OLEDMutex = xSemaphoreCreateMutex();
    if(OLEDMutex == NULL)
    {
        /* 信号量创建失败 */
        while(1);
    }
    
    /* 创建显示任务 */
    xTaskCreate((TaskFunction_t)DisplayTask,
                (const char*)"DisplayTask",
                (uint16_t)TASK_STACK_SIZE_DISPLAY,
                (void*)NULL,
                (UBaseType_t)TASK_PRIORITY_DISPLAY,
                (TaskHandle_t*)&DisplayTaskHandle);
    
    /* 创建按键处理任务 */
    xTaskCreate((TaskFunction_t)KeyProcessTask,
                (const char*)"KeyTask",
                (uint16_t)TASK_STACK_SIZE_KEY,
                (void*)NULL,
                (UBaseType_t)TASK_PRIORITY_KEY,
                (TaskHandle_t*)&KeyTaskHandle);
    
    /* 创建MPU6050任务 */
    xTaskCreate((TaskFunction_t)MPU6050Task,
                (const char*)"MPU6050Task",
                (uint16_t)TASK_STACK_SIZE_MPU6050,
                (void*)NULL,
                (UBaseType_t)TASK_PRIORITY_MPU6050,
                (TaskHandle_t*)&MPU6050TaskHandle);
                
    /* 创建数据同步任务 */
    xTaskCreate((TaskFunction_t)DataSyncTask,
                (const char*)"DataSyncTask",
                (uint16_t)TASK_STACK_SIZE_DATASYNC,
                (void*)NULL,
                (UBaseType_t)TASK_PRIORITY_DATASYNC,
                (TaskHandle_t*)&DataSyncTaskHandle);
    
    vTaskDelete(StartTaskHandle); // 删除开始任务
    taskEXIT_CRITICAL(); // 退出临界区
}

/**
  * @brief  空闲任务钩子函数 - 实现低功耗
  * @param  None
  * @retval None
  */
void vApplicationIdleHook(void)
{
    /* 空闲时进入睡眠模式 */
    __WFI(); // 等待中断
}

/**
  * @brief  栈溢出钩子函数
  * @param  xTask: 溢出的任务句柄
  * @param  pcTaskName: 任务名称
  * @retval None
  */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* 发生栈溢出，停机 */
    while(1)
    {
        // 实际应用中可以闪烁LED报警
    }
}

/**
  * @brief  内存分配失败钩子函数
  * @param  None
  * @retval None
  */
void vApplicationMallocFailedHook(void)
{
    /* 内存分配失败 */
    while(1)
    {
        // 实际应用中可以记录日志
    }
}

/* ============================================================================ */
/*                     FreeRTOS静态内存分配回调函数                            */
/* ============================================================================ */

/* 空闲任务控制块和栈 */
static StaticTask_t xIdleTaskTCB;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

/* 定时器任务控制块和栈 */
static StaticTask_t xTimerTaskTCB;
static StackType_t xTimerStack[80];

/**
  * @brief  获取空闲任务内存（静态分配）
  * @param  ppxIdleTaskTCBBuffer: TCB缓冲区指针
  * @param  ppxIdleTaskStackBuffer: 栈缓冲区指针
  * @param  pulIdleTaskStackSize: 栈大小指针
  * @retval None
  */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = xIdleStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
  * @brief  获取定时器任务内存（静态分配）
  * @param  ppxTimerTaskTCBBuffer: TCB缓冲区指针
  * @param  ppxTimerTaskStackBuffer: 栈缓冲区指针
  * @param  pulTimerTaskStackSize: 栈大小指针
  * @retval None
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = xTimerStack;
    *pulTimerTaskStackSize = 80;
}

/**
  ******************************************************************************
  * @file           : freertos_debug.c
  * @brief          : FreeRTOS调试辅助功能
  * @details        : 提供任务状态、堆栈使用、内存监控等调试信息
  ******************************************************************************
  */

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

/* 统计缓冲区大小 */
#define STATS_BUFFER_SIZE 512

/**
  * @brief  获取任务运行时间统计
  * @param  buffer: 输出缓冲区
  * @param  bufferSize: 缓冲区大小
  * @retval None
  * @note   需要在FreeRTOSConfig.h中开启configGENERATE_RUN_TIME_STATS
  */
void Get_TaskRunTimeStats(char *buffer, uint16_t bufferSize)
{
#if (configGENERATE_RUN_TIME_STATS == 1)
    vTaskGetRunTimeStats(buffer);
#else
    snprintf(buffer, bufferSize, "运行时间统计未启用\r\n");
#endif
}

/**
  * @brief  获取任务列表
  * @param  buffer: 输出缓冲区
  * @param  bufferSize: 缓冲区大小
  * @retval None
  * @note   需要在FreeRTOSConfig.h中开启configUSE_TRACE_FACILITY
  */
void Get_TaskList(char *buffer, uint16_t bufferSize)
{
#if (configUSE_TRACE_FACILITY == 1)
    vTaskList(buffer);
#else
    snprintf(buffer, bufferSize, "任务列表生成未启用\r\n");
#endif
}

/**
  * @brief  获取当前剩余堆内存
  * @param  None
  * @retval 剩余堆内存大小（字节）
  */
size_t Get_FreeHeapSize(void)
{
    return xPortGetFreeHeapSize();
}

/**
  * @brief  获取历史最小剩余堆内存
  * @param  None
  * @retval 历史最小剩余堆内存大小（字节）
  */
size_t Get_MinimumEverFreeHeapSize(void)
{
    return xPortGetMinimumEverFreeHeapSize();
}

/**
  * @brief  获取任务堆栈高水位线（最小剩余）
  * @param  xTask: 任务句柄，NULL表示当前任务
  * @retval 堆栈最小剩余量（字）
  */
UBaseType_t Get_TaskStackHighWaterMark(TaskHandle_t xTask)
{
    return uxTaskGetStackHighWaterMark(xTask);
}

/**
  * @brief  打印内存统计信息
  * @param  None
  * @retval None
  * @note   通过串口打印
  */
void Print_MemoryStats(void)
{
    printf("\r\n=== FreeRTOS内存统计 ===\r\n");
    printf("当前剩余堆内存: %d 字节\r\n", Get_FreeHeapSize());
    printf("历史最小剩余堆: %d 字节\r\n", Get_MinimumEverFreeHeapSize());
    printf("总堆内存大小: %d 字节\r\n", configTOTAL_HEAP_SIZE);
    printf("已使用堆内存: %d 字节\r\n", configTOTAL_HEAP_SIZE - Get_FreeHeapSize());
    printf("========================\r\n\r\n");
}

/**
  * @brief  打印任务堆栈统计信息
  * @param  None
  * @retval None
  * @note   需要引用外部的任务句柄，使用extern
  */
void Print_TaskStackStats(void)
{
    extern TaskHandle_t DisplayTaskHandle;
    extern TaskHandle_t KeyTaskHandle;
    extern TaskHandle_t MPU6050TaskHandle;
    
    printf("\r\n=== 任务堆栈统计 ===\r\n");
    
    if(DisplayTaskHandle != NULL)
    {
        UBaseType_t stackHighWater = Get_TaskStackHighWaterMark(DisplayTaskHandle);
        printf("DisplayTask 剩余: %d 字\r\n", stackHighWater);
    }
    
    if(KeyTaskHandle != NULL)
    {
        UBaseType_t stackHighWater = Get_TaskStackHighWaterMark(KeyTaskHandle);
        printf("KeyTask 剩余: %d 字\r\n", stackHighWater);
    }
    
    if(MPU6050TaskHandle != NULL)
    {
        UBaseType_t stackHighWater = Get_TaskStackHighWaterMark(MPU6050TaskHandle);
        printf("MPU6050Task 剩余: %d 字\r\n", stackHighWater);
    }
    
    TaskHandle_t idleHandle = xTaskGetIdleTaskHandle();
    if(idleHandle != NULL)
    {
        UBaseType_t stackHighWater = Get_TaskStackHighWaterMark(idleHandle);
        printf("IdleTask 剩余: %d 字\r\n", stackHighWater);
    }
    
    printf("======================\r\n\r\n");
}

/**
  * @brief  打印任务列表
  * @param  None
  * @retval None
  * @note   通过串口打印
  */
void Print_TaskList(void)
{
#if (configUSE_TRACE_FACILITY == 1)
    char buffer[STATS_BUFFER_SIZE];
    
    printf("\r\n=== FreeRTOS任务列表 ===\r\n");
    printf("任务名称\t\t状态\t优先级\t堆栈\t任务号\r\n");
    printf("----------------------------------------------\r\n");
    
    Get_TaskList(buffer, STATS_BUFFER_SIZE);
    printf("%s", buffer);
    
    printf("======================\r\n\r\n");
    
    /* 状态代码:
     * X - 运行中
     * R - 就绪
     * B - 阻塞
     * S - 挂起
     * D - 删除
     */
#else
    printf("任务列表生成未启用\r\n");
#endif
}

/**
  * @brief  打印任务运行时间统计
  * @param  None
  * @retval None
  * @note   通过串口打印
  */
void Print_TaskRunTimeStats(void)
{
#if (configGENERATE_RUN_TIME_STATS == 1)
    char buffer[STATS_BUFFER_SIZE];
    
    printf("\r\n=== 任务运行时间统计 ===\r\n");
    printf("任务名称\t\t运行计数\t百分比\r\n");
    printf("-------------------------------\r\n");
    
    Get_TaskRunTimeStats(buffer, STATS_BUFFER_SIZE);
    printf("%s", buffer);
    
    printf("======================\r\n\r\n");
#else
    printf("运行时间统计未启用\r\n");
#endif
}

/**
  * @brief  打印系统诊断报告
  * @param  None
  * @retval None
  * @note   汇总打印所有调试信息
  */
void Print_SystemDiagnostics(void)
{
    printf("\r\n");
    printf("*************************************************\r\n");
    printf("*        STM32系统诊断报告                 *\r\n");
    printf("*************************************************\r\n");
    
    /* 打印内存统计 */
    Print_MemoryStats();
    
    /* 打印堆栈统计 */
    Print_TaskStackStats();
    
    /* 打印任务列表 */
    Print_TaskList();
    
    /* 打印运行时间 */
    Print_TaskRunTimeStats();
    
    printf("*************************************************\r\n\r\n");
}

/**
  * @brief  检查系统健康状况
  * @param  None
  * @retval 0=正常, 1=内存不足, 2=堆栈溢出风险
  */
uint8_t Check_SystemHealth(void)
{
    size_t freeHeap = Get_FreeHeapSize();
    
    /* 堆内存警告阈值：1KB */
    if(freeHeap < 1024)
    {
        printf("警告: 堆内存不足! 剩余 %d 字节\r\n", freeHeap);
        return 1;
    }
    
    /* 堆栈警告阈值：20字 */
    extern TaskHandle_t DisplayTaskHandle;
    extern TaskHandle_t KeyTaskHandle;
    extern TaskHandle_t MPU6050TaskHandle;
    
    if(DisplayTaskHandle != NULL && Get_TaskStackHighWaterMark(DisplayTaskHandle) < 20)
    {
        printf("警告: DisplayTask 堆栈溢出风险!\r\n");
        return 2;
    }
    
    if(KeyTaskHandle != NULL && Get_TaskStackHighWaterMark(KeyTaskHandle) < 20)
    {
        printf("警告: KeyTask 堆栈溢出风险!\r\n");
        return 2;
    }
    
    if(MPU6050TaskHandle != NULL && Get_TaskStackHighWaterMark(MPU6050TaskHandle) < 20)
    {
        printf("警告: MPU6050Task 堆栈溢出风险!\r\n");
        return 2;
    }
    
    return 0; // 系统正常
}

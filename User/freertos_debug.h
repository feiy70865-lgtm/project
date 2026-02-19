/**
  ******************************************************************************
  * @file           : freertos_debug.h
  * @brief          : FreeRTOS???????????????
  ******************************************************************************
  */

#ifndef __FREERTOS_DEBUG_H
#define __FREERTOS_DEBUG_H

#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

/* ???????? */
void Get_TaskRunTimeStats(char *buffer, uint16_t bufferSize);
void Get_TaskList(char *buffer, uint16_t bufferSize);
size_t Get_FreeHeapSize(void);
size_t Get_MinimumEverFreeHeapSize(void);
UBaseType_t Get_TaskStackHighWaterMark(TaskHandle_t xTask);

void Print_MemoryStats(void);
void Print_TaskStackStats(void);
void Print_TaskList(void);
void Print_TaskRunTimeStats(void);
void Print_SystemDiagnostics(void);

uint8_t Check_SystemHealth(void);

#endif /* __FREERTOS_DEBUG_H */

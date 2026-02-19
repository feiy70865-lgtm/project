#ifndef __DATA_CENTER_H
#define __DATA_CENTER_H

#include "stm32f10x.h"

/* 传感器数据结构体 */
typedef struct {
    float Roll;
    float Pitch;
    float Yaw;
} SensorData_t;

/* 秒表数据结构体 */
typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t isRunning;
} StopWatchData_t;

/* 初始化 */
void DataCenter_Init(void);

/* 传感器数据访问 */
void DataCenter_SetSensorData(float roll, float pitch, float yaw);
SensorData_t DataCenter_GetSensorData(void);

/* 秒表控制与访问 */
void DataCenter_StopWatch_Tick(void); // 供定时器中断调用
void DataCenter_StopWatch_Start(void);
void DataCenter_StopWatch_Pause(void);
void DataCenter_StopWatch_Reset(void);
StopWatchData_t DataCenter_GetStopWatchData(void);

#endif

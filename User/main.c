#include "stm32f10x.h"                  // Device header
#include "bsp_delay.h"
#include "bsp_oled.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_rtc.h"
#include "bsp_mpu6050.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_manager.h"
#include "task_common.h"
#include "app_config.h"

/**
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  * 
  *       0             X轴           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/* 设置NVIC优先级分组为Group 4 (FreeRTOS要求) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* 初始化SysTick（用于Delay_us，必须在MPU6050初始化前） */
	SysTick_Init(72);
	
	/* 初始化外设 */
	OLED_Init();
	Key_Init();
	LED_Init();
	Timer_Init();
	MyRTC_Init();
	
	/* 初始化MPU6050 */
	// extern uint8_t MPU6050_Available;
	uint8_t mpu_status = MPU6050_Init();
	MPU6050_Available = (mpu_status == 0) ? 1 : 0;
	
	/* 创建启动任务 */
	xTaskCreate((TaskFunction_t)StartTask,
	            (const char*)"StartTask",
	            (uint16_t)TASK_STACK_SIZE_START,
	            (void*)NULL,
	            (UBaseType_t)TASK_PRIORITY_START,
	            (TaskHandle_t*)&StartTaskHandle);
	
	/* 确俞SysTick和PendSV中断优先级正确 */
	NVIC_SetPriority(PendSV_IRQn, 15);
	NVIC_SetPriority(SysTick_IRQn, 15);
	
	/* 启动FreeRTOS调度器（正常情况下不会返回） */
	vTaskStartScheduler();
	
	while(1);  // 不应该执行到这里
}

/* TIM2中断处理函数已移至Timer.c */


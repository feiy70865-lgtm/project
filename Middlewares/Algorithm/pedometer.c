#include "stm32f10x.h"
#include "math.h"
#include "pedometer.h"
#include "bsp_flash.h"
#include "bsp_rtc.h"
#include "debug_serial.h"

// 计步算法参数
#define STEP_THRESHOLD 		2000  // 加速度变化阈值 (基准是16384)
#define STEP_MIN_INTERVAL 	300   // 最小步数间隔(ms)

static uint32_t StepCount = 0;
static uint32_t LastStepTime = 0;
static float LastAccelMag = 0;
static uint8_t StepState = 0; // 0: 等待峰值, 1: 等待波谷

// 存储相关
#define FLASH_SECTOR_BASE 	0x000000 // 从第0个扇区开始存
#define RECORD_SIZE 		8 		 // 每天记录大小: 4字节日期(YYYYMMDD) + 4字节步数

void Pedometer_Init(void)
{
	// 读取当天的步数（如果有）
	// 这里简化处理，每次上电先清零，或者从Flash读取最后一条记录
	// 实际应用需要遍历Flash找到今天的记录
	StepCount = 0;
}

uint32_t Pedometer_GetStepCount(void)
{
	return StepCount;
}

void Pedometer_Reset(void)
{
	StepCount = 0;
}

// 传入MPU6050原始数据
void Pedometer_Update(int16_t ax, int16_t ay, int16_t az, uint32_t current_tick)
{
	float accel_mag = sqrt(ax*ax + ay*ay + az*az);
	
	// 简单的峰值检测
	// 实际重力加速度约为 16384
	
	if (StepState == 0)
	{
		if (accel_mag > 16384 + STEP_THRESHOLD)
		{
			StepState = 1;
		}
	}
	else if (StepState == 1)
	{
		if (accel_mag < 16384)
		{
			if (current_tick - LastStepTime > STEP_MIN_INTERVAL)
			{
				StepCount++;
				LastStepTime = current_tick;
				Serial_Printf("Steps: %d\r\n", StepCount); // 蓝牙发送步数
			}
			StepState = 0;
		}
	}
}

// 保存步数到Flash
// 格式: [YYYYMMDD][STEPS]
void Pedometer_SaveToFlash(void)
{
	extern int MyRTC_Time[];
	uint32_t date = MyRTC_Time[0]*10000 + MyRTC_Time[1]*100 + MyRTC_Time[2];
	uint32_t steps = StepCount;
	
	uint8_t data[8];
	data[0] = (date >> 24) & 0xFF;
	data[1] = (date >> 16) & 0xFF;
	data[2] = (date >> 8) & 0xFF;
	data[3] = date & 0xFF;
	
	data[4] = (steps >> 24) & 0xFF;
	data[5] = (steps >> 16) & 0xFF;
	data[6] = (steps >> 8) & 0xFF;
	data[7] = steps & 0xFF;
	
	// 简单实现：写入固定地址，实际应该做磨损均衡和追加写入
	// 这里为了演示，写入扇区0的起始位置
	W25Q64_SectorErase(FLASH_SECTOR_BASE);
	W25Q64_PageProgram(FLASH_SECTOR_BASE, data, 8);
	
	Serial_Printf("Saved: Date=%d, Steps=%d\r\n", date, steps);
}

void Pedometer_LoadFromFlash(void)
{
	uint8_t data[8];
	W25Q64_ReadData(FLASH_SECTOR_BASE, data, 8);
	
	uint32_t date = (data[0]<<24) | (data[1]<<16) | (data[2]<<8) | data[3];
	uint32_t steps = (data[4]<<24) | (data[5]<<16) | (data[6]<<8) | data[7];
	
	extern int MyRTC_Time[];
	uint32_t current_date = MyRTC_Time[0]*10000 + MyRTC_Time[1]*100 + MyRTC_Time[2];
	
	if(date == current_date)
	{
		StepCount = steps;
		Serial_Printf("Loaded: Steps=%d\r\n", StepCount);
	}
}

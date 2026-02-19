#include "Storage.h"
#include "bsp_flash.h"
#include "debug_serial.h"

/* 
 * 存储规划：
 * W25Q64 总容量 8MB (0x000000 - 0x7FFFFF)
 * 使用最后一个 64KB 块 (Block 127) 存储步数记录
 * 起始地址: 0x7F0000
 * 每个记录大小: 8字节 (2+1+1+4)
 * 一个扇区(4KB)可存 512 条记录
 */

#define STORAGE_START_ADDR  0x7F0000
#define RECORD_SIZE         sizeof(DailyStepRecord_t)

void Storage_Init(void)
{
    W25Q64_Init();
}

/**
  * @brief  保存当天的步数 (简单的追加写入，实际产品需考虑磨损均衡和查找逻辑)
  */
void Storage_SaveDailySteps(uint16_t year, uint8_t month, uint8_t day, uint32_t steps)
{
    DailyStepRecord_t record;
    record.Year = year;
    record.Month = month;
    record.Day = day;
    record.Steps = steps;
    
    /* 简单实现：查找第一个空闲位置写入 */
    /* 注意：Flash写入前需擦除，这里假设已擦除或顺序写入空地址 */
    /* 实际应用中，应该在初始化时扫描找到写入点 */
    
    uint32_t addr = STORAGE_START_ADDR;
    DailyStepRecord_t temp;
    
    // 遍历查找空位置 (0xFF表示空)
    // 为了演示简单，这里只检查前100条记录
    for(int i=0; i<100; i++) {
        W25Q64_ReadData(addr, (uint8_t*)&temp, RECORD_SIZE);
        
        // 如果找到空位置，或者找到当天的记录(覆盖更新)
        if(temp.Year == 0xFFFF || (temp.Year == year && temp.Month == month && temp.Day == day)) {
            // 如果是更新，需要扇区擦除再重写整个扇区(Flash特性)，这里简化为只追加
            // 如果是追加到空位置
            if(temp.Year == 0xFFFF) {
                W25Q64_PageProgram(addr, (uint8_t*)&record, RECORD_SIZE);
                Serial_Printf("Data Saved: %d-%d-%d Steps:%d @ %X\r\n", year, month, day, steps, addr);
                return;
            }
            // 如果是当天记录已存在，这里为了简单，暂不处理覆盖(Flash不能直接覆盖写)
            // 实际做法：将数据读到RAM，修改后擦除扇区再写回
             Serial_Printf("Data Exists for today. (Overwrite not implemented in simple demo)\r\n");
             return;
        }
        addr += RECORD_SIZE;
    }
}

void Storage_ReadHistory(void)
{
    uint32_t addr = STORAGE_START_ADDR;
    DailyStepRecord_t temp;
    
    Serial_Printf("--- History Data ---\r\n");
    for(int i=0; i<100; i++) {
        W25Q64_ReadData(addr, (uint8_t*)&temp, RECORD_SIZE);
        if(temp.Year == 0xFFFF) break; // 遇到空数据停止
        
        Serial_Printf("[%d] %d-%02d-%02d: %d steps\r\n", i, temp.Year, temp.Month, temp.Day, temp.Steps);
        addr += RECORD_SIZE;
    }
    Serial_Printf("--- End ---\r\n");
}

void Storage_ClearAll(void)
{
    W25Q64_SectorErase(STORAGE_START_ADDR);
    Serial_Printf("Storage Cleared.\r\n");
}

#ifndef __STORAGE_H
#define __STORAGE_H

#include <stdint.h>

/* ´æ´¢½á¹¹Ìå */
typedef struct {
    uint16_t Year;
    uint8_t Month;
    uint8_t Day;
    uint32_t Steps;
} DailyStepRecord_t;

void Storage_Init(void);
void Storage_SaveDailySteps(uint16_t year, uint8_t month, uint8_t day, uint32_t steps);
void Storage_ReadHistory(void);
void Storage_ClearAll(void);

#endif

/**
  ******************************************************************************
  * @file           : app_config.h
  * @brief          : 应用配置文件
  * @details        : 包含任务优先级、堆栈大小、队列长度等系统配置
  ******************************************************************************
  */

#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/* ============================================================================ */
/*                            任务优先级配置                                      */
/* ============================================================================ */
/* 优先级范围: 0-7 (configMAX_PRIORITIES-1)
 * 数值越大优先级越高
 * 空闲任务优先级为0
 */
#define TASK_PRIORITY_START         4    /* 开始任务优先级，负责创建其他任务 */
#define TASK_PRIORITY_KEY           3    /* 按键任务优先级，需要及时响应 */
#define TASK_PRIORITY_DISPLAY       2    /* 显示任务优先级 */
#define TASK_PRIORITY_MPU6050       1    /* MPU6050任务优先级，数据采集 */
#define TASK_PRIORITY_DATASYNC      1    /* 数据同步任务优先级 */

/* ============================================================================ */
/*                            任务堆栈大小配置                                      */
/* ============================================================================ */
/* 堆栈大小单位: Word (4字节)
 * STM32F103C8T6总RAM: 20KB
 * FreeRTOS内核占用: 约6KB
 * 用户可用堆栈: 约14KB
 */
#define TASK_STACK_SIZE_START       80   /* 开始任务堆栈大小，仅用于创建任务 */
#define TASK_STACK_SIZE_DISPLAY     300  /* 显示任务堆栈大小，OLED_Printf需要较大栈 */
#define TASK_STACK_SIZE_KEY         90   /* 按键任务堆栈大小 */
#define TASK_STACK_SIZE_MPU6050     256  /* MPU6050任务堆栈大小，涉及浮点运算 */
#define TASK_STACK_SIZE_DATASYNC    128  /* 数据同步任务堆栈大小 */

/* ============================================================================ */
/*                            队列配置                                    */
/* ============================================================================ */
#define QUEUE_LENGTH_KEY            10   /* 按键队列长度 */
#define QUEUE_ITEM_SIZE_KEY         sizeof(uint8_t)  /* 按键队列项目大小 */

/* ============================================================================ */
/*                            显示刷新配置                                      */
/* ============================================================================ */
/* 刷新周期单位: 毫秒 */
#define DISPLAY_REFRESH_PERIOD      50   /* 显示刷新周期 50ms (20fps) */

/* 不同界面的刷新周期定义 */
#define DISPLAY_REFRESH_CLOCK       1000 /* 时钟界面刷新 1000ms (1fps) */
#define DISPLAY_REFRESH_MENU        30   /* 菜单界面刷新 30ms (33fps，保证流畅) */
#define DISPLAY_REFRESH_GAME        50   /* 游戏界面刷新 50ms (20fps) */
#define DISPLAY_REFRESH_EMOJI       100  /* 表情界面刷新 100ms (10fps) */
#define DISPLAY_REFRESH_MPU6050     100  /* MPU6050界面刷新 100ms (10fps) */
#define DISPLAY_REFRESH_DEFAULT     200  /* 默认刷新周期 200ms (5fps) */

/* ============================================================================ */
/*                            MPU6050配置                                    */
/* ============================================================================ */
#define MPU6050_SAMPLE_PERIOD_FAST  5    /* MPU6050快速采样周期 5ms (200Hz) */
#define MPU6050_SAMPLE_PERIOD_SLOW  100  /* MPU6050慢速采样周期 100ms (10Hz) */

/* ============================================================================ */
/*                            电源管理配置                                         */
/* ============================================================================ */
#define ENABLE_TICKLESS_IDLE        1    /* 启用Tickless空闲模式以省电 */
#define OLED_AUTO_OFF_TIMEOUT       180   /* OLED自动熄屏超时 (秒) 0=不熄屏 */
#define OLED_AUTO_DIM_TIMEOUT       30   /* OLED自动变暗超时 (秒) 0=不变暗 */

/* ============================================================================ */
/*                            抬手唤醒配置                                       */
/* ============================================================================ */
#define WAKE_UP_ANGLE_ROLL_MIN      -45  /* 唤醒角度 Roll 最小值 */
#define WAKE_UP_ANGLE_ROLL_MAX      45   /* 唤醒角度 Roll 最大值 */
#define WAKE_UP_ANGLE_PITCH_MIN     -45  /* 唤醒角度 Pitch 最小值 */
#define WAKE_UP_ANGLE_PITCH_MAX     10   /* 唤醒角度 Pitch 最大值 */
#define MOVEMENT_THRESHOLD          5.0f /* 运动阈值 (检测动作幅度) */

/* ============================================================================ */
/*                            调试配置                                           */
/* ============================================================================ */
#define ENABLE_RUNTIME_STATS        1    /* 启用运行时间统计 */
#define ENABLE_STACK_CHECK          1    /* 启用堆栈溢出检查 */
#define ENABLE_MALLOC_CHECK         1    /* 启用内存分配检查 */

/* 内存警告阈值 */
#define HEAP_WARNING_THRESHOLD      1024 /* 堆内存警告阈值 (字节) */
#define STACK_WARNING_THRESHOLD     20   /* 堆栈警告阈值 (字) */

/* ============================================================================ */
/*                            按键配置                                           */
/* ============================================================================ */
#define KEY_SCAN_PERIOD             1    /* 按键扫描周期 (TIM2中断周期ms) */
#define KEY_DEBOUNCE_COUNT          20   /* 按键消抖计数 (20ms) */

/* ============================================================================ */
/*                            秒表配置                                           */
/* ============================================================================ */
#define STOPWATCH_TICK_PERIOD       1    /* 秒表滴答周期 (ms) */

/* ============================================================================ */
/*                            游戏配置                                           */
/* ============================================================================ */
#define GAME_UPDATE_PERIOD          20   /* 游戏逻辑更新周期 (ms) */
#define GAME_SCORE_UPDATE_PERIOD    100  /* 游戏分数更新周期 (ms) */

/* ============================================================================ */
/*                            内存校验                                        */
/* ============================================================================ */
/* 计算总任务堆栈大小 */
#define TOTAL_TASK_STACK_SIZE  (TASK_STACK_SIZE_START + \
                                TASK_STACK_SIZE_DISPLAY + \
                                TASK_STACK_SIZE_KEY + \
                                TASK_STACK_SIZE_MPU6050 + \
                                configMINIMAL_STACK_SIZE + \
                                configTIMER_TASK_STACK_DEPTH)

/* 估计总内存占用 */
#define ESTIMATED_TOTAL_MEMORY  (TOTAL_TASK_STACK_SIZE * 4 + configTOTAL_HEAP_SIZE)

/* 内存检查警告 */
/* 如果估计内存超过物理内存，编译器会发出警告 */
/* #if (ESTIMATED_TOTAL_MEMORY > 18432) */
/*     #warning "估计总内存超过了可用内存 (18KB)" */
/* #endif */

/* ============================================================================ */
/*                            版本信息                                           */
/* ============================================================================ */
#define APP_VERSION_MAJOR       1
#define APP_VERSION_MINOR       0
#define APP_VERSION_PATCH       0
#define APP_VERSION_STRING      "V1.0.0"
#define APP_BUILD_DATE          __DATE__
#define APP_BUILD_TIME          __TIME__

#endif /* __APP_CONFIG_H */

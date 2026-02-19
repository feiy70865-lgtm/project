#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

    /****************   外部头文件声明   ****************/
    #include "bsp_sys.h"



    /****************    函数外部声明   *****************/
    void SysTick_Init(uint16_t sysclk);/* 滴答定时器初始化 */
    void Delay_us(uint32_t us);        /* 微妙延时 */
    void Delay_ms(uint16_t ms);        /* 毫秒延时 */
    void Delay_s(uint16_t s);          /* 秒延时 */

#endif






























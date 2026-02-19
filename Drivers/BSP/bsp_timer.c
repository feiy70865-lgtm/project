#include "stm32f10x.h"                  // Device header
#include "bsp_timer.h"
#include "DataCenter.h"

/* FreeRTOS运行时统计相关变量 */
uint32_t FreeRTOSRunTimeTicks = 0;

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* 注意：FreeRTOS使用NVIC_PriorityGroup_4，这里配置也要对应 */
	/* 优先级5以上才能调用FreeRTOS API */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;  /* 优先级6，可调用FreeRTOS API */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

/**********************************************************
* @funcName ：ConfigureTimeForRunTimeStats
* @brief    ：配置FreeRTOS运行时统计所需的定时器
* @param    ：void
* @retval   ：void
* @details  ：FreeRTOS运行时统计功能需要一个定时器提供时基
* @fn       ：
************************************************************/
void ConfigureTimeForRunTimeStats(void)
{
	/* 使用TIM2作为运行时统计的时基，已在Timer_Init中初始化 */
	/* 此函数可以为空，因为Timer_Init已经配置好了TIM2 */
	FreeRTOSRunTimeTicks = 0;
}

/**********************************************************
* @funcName ：TIM2_IRQHandler
* @brief    ：TIM2中断服务函数
* @param    ：void
* @retval   ：void
* @details  ：处理按键扫描、秒表计时和游戏计分，并更新FreeRTOS运行时统计
* @fn       ：
************************************************************/
void TIM2_IRQHandler(void)
{
	extern void Key_Tick(void);
	extern void Score_Tick(void);
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		DataCenter_StopWatch_Tick();
		Score_Tick();
		FreeRTOSRunTimeTicks++; /* 更新FreeRTOS运行时统计计数器 */
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


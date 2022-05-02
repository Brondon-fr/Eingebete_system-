/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f4xx_it.h"
#include <stopwatch/stopwatch.h>
// extern TIM_HandleTypeDef tim2_handle_struct;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

void TIM2_IRQHandler(void)
{
HAL_TIM_IRQHandler(&tim2_handle_struct);

}

void EXTI15_10_IRQHandler(void)
{
EXTI_HandleTypeDef hexti;
HAL_EXTI_GetHandle(&hexti, EXTI_LINE_12);
if(HAL_EXTI_GetPending(&hexti, EXTI_TRIGGER_FALLING))
{
HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}
}

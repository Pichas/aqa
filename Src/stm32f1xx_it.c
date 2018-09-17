/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "rtc.h"
#include "oled_lcd.h"
#include "ledControl.h"
#include "myMenu.h"


/* USER CODE BEGIN 0 */
typedef struct {
  uint8_t left;
  uint8_t right;
  uint8_t middle;
}encoder_t;


volatile extern uint8_t effectIndex;

encoder_t myEnc;

extern element_t* mCursor;
uint8_t selected = 0;

uint8_t buf[21]; //row buffer
const uint8_t screenOnTime = 5 * 10; //10 sec
uint8_t screenOn = screenOnTime; 

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles DMA1 channel6 global interrupt.
*/
void DMA1_Channel6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

  /* USER CODE END DMA1_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_i2c1_tx);
  /* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

 
  I2C1->CR1 |= I2C_CR1_STOP;
  hi2c1.State = HAL_I2C_STATE_READY;
  /* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel7 global interrupt.
*/
void DMA1_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel7_IRQn 0 */

  /* USER CODE END DMA1_Channel7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_tim2_ch2_ch4);
  /* USER CODE BEGIN DMA1_Channel7_IRQn 1 */

  /* USER CODE END DMA1_Channel7_IRQn 1 */
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
  if (!myEnc.middle)
    myEnc.middle = !(uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
  
  if(!(myEnc.left | myEnc.right)){ //если лево и право не установлено
    myEnc.left  = !(uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); //выставить первое пришедшее значение. оно и будет указывать на направление поворота
    myEnc.right = !(uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
  }
  
  if(myEnc.left & myEnc.right){ //если оба установлены
    myEnc.left = 0; //сбросить в 0
    myEnc.right = 0;
  }
  
  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
  
  RTC_DateTypeDef sDate;
  RTC_TimeTypeDef sTime;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  
  if(sTime.Hours ==  6 && sTime.Minutes == 00) effectIndex = 1; //sunrise
  if(sTime.Hours == 21 && sTime.Minutes == 00) effectIndex = 2; //sunrise
  
  
  
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */
  
  if (myEnc.left | myEnc.right | myEnc.middle) screenOn = screenOnTime; //set display to
  if(screenOn) {
    OLED_send_command(OLED_DISPLAYON);
    screenOn--;
  }else{
    OLED_send_command(OLED_DISPLAYOFF);
    return;
  }
  
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  
  OLED_blank(); //clear 
  

  /* Display time Format : hh:mm:ss */
  sprintf((char *)buf, "%2d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
  OLED_write(0, 0, buf);
  /* Display date Format : dd-mm-yyyy */
  sprintf((char *)buf, "%2d-%02d-%4d", sDate.Date, sDate.Month, 2000 + sDate.Year);
  OLED_write(0, 11, buf);


 
  element_t* tCursor = mCursor->prev->prev; //temp pos-2
  OLED_write(5, 0, (uint8_t*)">");
  
  for (int i = 0 ; i < 6 ; i++){
    if(tCursor->eType == PARAM) menuLoadActualValToElement(tCursor);
      
    sprintf ((char *)buf, "%-17s%3d", tCursor->name, tCursor->curVal);

    OLED_write(i + 3, 1, buf);
    
    if(tCursor->eType == FOLDER || tCursor->eType == FUNC)
      OLED_write(i + 3, 20, (uint8_t*)" ");
     
    tCursor = tCursor->next;
  }
    
    
   if(selected) {
    OLED_invertRow(5);
  }
  
  OLED_refresh();
  
  
  if (!(myEnc.left | myEnc.right | myEnc.middle)) return; // если ничего не нажато, то просто выйти
  
  if (selected){
    if(myEnc.left) {
      menuSetNewValToElem(mCursor->curVal - 1);
      if(mCursor->eType == PARAM) menuMainFunc();
    }
    if(myEnc.right) {
      menuSetNewValToElem(mCursor->curVal + 1);
      if(mCursor->eType == PARAM) menuMainFunc();
    }
    if(myEnc.middle) {
      if(mCursor->eType == FUNC) menuMainFunc();
      selected = 0;
    }
  }else{
    if(myEnc.left && mCursor->prev != 0) mCursor = mCursor->prev;
    if(myEnc.right && mCursor->next != 0) mCursor = mCursor->next;
    if(myEnc.middle){
      if(mCursor->child != 0) mCursor = mCursor->child; //folder
      else if(mCursor->child == 0) selected = 1; //other
    }
  }
  
  
  myEnc.left = 0;
  myEnc.right = 0;
  myEnc.middle = 0;
  /* USER CODE END TIM4_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

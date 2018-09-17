#ifndef __MYMENU_H__
#define __MYMENU_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "stdlib.h"
#include "string.h"
#include "rtc.h"
#include "config.h"
#include "ledcontrol.h"
#include "oled_lcd.h"

typedef enum{
  FOLDER = 0,
  PARAM,
  FUNC
} meType;

typedef enum {
  ENTER = 0,
  YEAR,
  MONTH,
  DAY,
  HOUR,
  MINUTE,
  SECOND,
  LEDCOUNT,
  REDLIMIT,
  GREENLIMIT,
  BLUELIMIT,
  BRIGHTLIMIT,
  FLASHLEDCOUNT,
  CURRENTEFF,
  RGBTIME,
  FLASHWRITE
  
} meCode;



   
typedef struct element{
  
  const char name[16];
  meType eType;
  
  struct element* prev;
  struct element* next;
  struct element* child;
  
  uint16_t minVal;
  uint16_t curVal;
  uint16_t maxVal;
  
  uint8_t code;
} element_t;
   
   
element_t* menuGet (void);

void menuLoadActualValToElement(element_t* e);
void menuSetNewValToElem( uint16_t nv);
   
   
void menuMainFunc(void);
   
   
   
   
   
   
   
   
   
   
   
   
   

#ifdef __cplusplus
}
#endif

#endif 



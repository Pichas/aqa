#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define PAGEADDR 0x0801FC00U //последн€€ страница в пам€ти (из дш)


typedef struct {
  uint16_t ledCount;
  uint16_t redLimit;
  uint16_t greenLimit;
  uint16_t blueLimit;
  uint16_t brightLimit;
  uint16_t flashLedCount;
  
  void (*save)(void); //указатель на функцию сохранени€ данных в флэш
}setting;


setting* loadSettings(void);
setting* getSettings(void);
void saveSettings(void);


#ifdef __cplusplus
}
#endif

#endif 

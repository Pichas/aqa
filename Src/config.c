#include "config.h"

setting set;

void Internal_Flash_Erase(unsigned int pageAddress); //стереть —“–јЌ»÷”!
void Internal_Flash_Write(uint16_t* data, uint32_t address, uint16_t count); //записать 2 байта во флэш


setting* loadSettings(void){ //загрузка параметров из пам€ти в оперативу
  set.save = &saveSettings; //установить указатель на сохранение
  uint8_t ci = 0; //индекс параметра в пам€ти
  
  set.ledCount      = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов
  set.redLimit      = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //ограничение по красному цвету
  set.greenLimit    = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //ограничение по зеленому
  set.blueLimit     = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //ограничение по синему
  set.brightLimit   = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //ограничение по €ркости дл€ облачности
  set.flashLedCount = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.sunrise       = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.sunset        = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.R0_0          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.R0_1          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.R1_0          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  set.R1_1          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //количество светодиодов дл€ эфекта молнии
  
  
  set.ledCount      = set.ledCount      > 300  ? 300  : set.ledCount; //ограничить до 300 штук
  set.redLimit      = set.redLimit      > 255  ? 255  : set.redLimit; //ограничить цвет до 255 
  set.greenLimit    = set.greenLimit    > 255  ? 255  : set.greenLimit; //ограничить цвет до 255 
  set.blueLimit     = set.blueLimit     > 255  ? 255  : set.blueLimit; //ограничить цвет до 255 
  set.brightLimit   = set.brightLimit   > 255  ? 255  : set.brightLimit; //ограничить €ркость до 255 
  set.flashLedCount = set.flashLedCount > 300  ? 300  : set.flashLedCount; //ограничить молнии до 300 
  set.sunrise       = set.sunrise       > 1440 ? 1440 : set.sunrise; // 24h timer off
  set.sunset        = set.sunset        > 1440 ? 1440 : set.sunset; //24h timer off
  set.R0_0          = set.R0_0          > 1440 ? 1440 : set.R0_0; //24h timer off
  set.R0_1          = set.R0_1          > 1440 ? 1440 : set.R0_1; //24h timer off
  set.R1_0          = set.R1_0          > 1440 ? 1440 : set.R1_0; //24h timer off
  set.R1_1          = set.R1_1          > 1440 ? 1440 : set.R1_1; //24h timer off

  
  return &set;
}

setting* getSettings(void){
  return &set;
}


void saveSettings(void){
  //массив дл€ записи
  uint16_t arrToSave[] = {set.ledCount,
                          set.redLimit,
                          set.greenLimit,
                          set.blueLimit,
                          set.brightLimit,
                          set.flashLedCount,
                          set.sunrise,
                          set.sunset,
                          set.R0_0,
                          set.R0_1,
                          set.R1_0,
                          set.R1_1};
  
  
  //unlock
  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;
  
  Internal_Flash_Erase(PAGEADDR); //стереть 
  Internal_Flash_Write(arrToSave, PAGEADDR, sizeof(arrToSave)/sizeof(*arrToSave)); // записать

  //lock
  FLASH->CR |= FLASH_CR_LOCK;
}



void Internal_Flash_Erase(unsigned int pageAddress) {
  while (FLASH->SR & FLASH_SR_BSY);
  if (FLASH->SR & FLASH_SR_EOP) {
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = pageAddress;
  FLASH->CR |= FLASH_CR_STRT;
  while (!(FLASH->SR & FLASH_SR_EOP));
  FLASH->SR = FLASH_SR_EOP;
  FLASH->CR &= ~FLASH_CR_PER;
}

void Internal_Flash_Write(uint16_t* data, uint32_t address, uint16_t count) {
  while (FLASH->SR & FLASH_SR_BSY);
  if (FLASH->SR & FLASH_SR_EOP) {
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR |= FLASH_CR_PG;

  for (uint32_t i = 0; i < count; i++) {
    *(__IO uint16_t*)(address + i*2) = (uint16_t)data[i]; //адрес увеличиваетс€ на 2 байта дл€ записи 2 байт данных
    while (!(FLASH->SR & FLASH_SR_EOP));
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR &= ~(FLASH_CR_PG);
}












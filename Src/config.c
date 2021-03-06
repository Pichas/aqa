#include "config.h"

setting set;

void Internal_Flash_Erase(unsigned int pageAddress); //������� ��������!
void Internal_Flash_Write(uint16_t* data, uint32_t address, uint16_t count); //�������� 2 ����� �� ����


setting* loadSettings(void){ //�������� ���������� �� ������ � ���������
  set.save = &saveSettings; //���������� ��������� �� ����������
  uint8_t ci = 0; //������ ��������� � ������
  
  set.ledCount      = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� �����������
  set.redLimit      = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //����������� �� �������� �����
  set.greenLimit    = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //����������� �� ��������
  set.blueLimit     = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //����������� �� ������
  set.brightLimit   = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //����������� �� ������� ��� ����������
  set.flashLedCount = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.sunrise       = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.sunset        = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.R0_0          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.R0_1          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.R1_0          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  set.R1_1          = (*(__IO uint16_t*) (PAGEADDR + sizeof(uint16_t) * ci++)); //���������� ����������� ��� ������ ������
  
  
  set.ledCount      = set.ledCount      > 300  ? 300  : set.ledCount; //���������� �� 300 ����
  set.redLimit      = set.redLimit      > 255  ? 255  : set.redLimit; //���������� ���� �� 255 
  set.greenLimit    = set.greenLimit    > 255  ? 255  : set.greenLimit; //���������� ���� �� 255 
  set.blueLimit     = set.blueLimit     > 255  ? 255  : set.blueLimit; //���������� ���� �� 255 
  set.brightLimit   = set.brightLimit   > 255  ? 255  : set.brightLimit; //���������� ������� �� 255 
  set.flashLedCount = set.flashLedCount > 300  ? 300  : set.flashLedCount; //���������� ������ �� 300 
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
  //������ ��� ������
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
  
  Internal_Flash_Erase(PAGEADDR); //������� 
  Internal_Flash_Write(arrToSave, PAGEADDR, sizeof(arrToSave)/sizeof(*arrToSave)); // ��������

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
    *(__IO uint16_t*)(address + i*2) = (uint16_t)data[i]; //����� ������������� �� 2 ����� ��� ������ 2 ���� ������
    while (!(FLASH->SR & FLASH_SR_EOP));
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR &= ~(FLASH_CR_PG);
}












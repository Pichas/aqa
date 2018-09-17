#include "ledControl.h"



//private func
//effects
void ledsOff(uint32_t* delayTime); //�������� �����
void sunrise(uint32_t* delayTime); //������
void sunset(uint32_t* delayTime); //�����

void snake(uint32_t* delayTime); //������
void discoteka (uint32_t* delayTime); //....

void cloud(uint32_t* delayTime); //�������, �������� �����
void rgbSensor(uint32_t* delayTime); //sensor mode
void rgbFreeze(uint32_t* delayTime);
void manual(uint32_t* delayTime);

//other
void setPalitColor( uint16_t num, enum palit p, uint8_t bright); //���������� ���� �� �������
void setUserColor( uint16_t num, uint8_t red, uint8_t green, uint8_t blue); //RGB
uint32_t colorConvToLED(uint8_t red, uint8_t green, uint8_t blue); //�������������� RGB � ������ 24 ����� ��� �����
void colorConvToRGB(ledx* led); //�� ������� � RGB

ledx getLedX(uint16_t num); //�������� ��������� �� ����� �������

void addEffect(pfn_effect); //�������� ������ � ������ ������� ��� ������


//vars
ledsArray ledCntrl; //���������� ������
setting* pSet; //���������
effects effAr; //�������
uint16_t tsc_r, tsc_g, tsc_b, tsc_c;

//func
//public func
ledsArray* init(uint16_t ledCount){
  ledCntrl.ledCount = ledCount - 1; // ���������� ����������� � �����
  ledCntrl.getLedX = &getLedX; //������������� ����������
  ledCntrl.setPalitColor = &setPalitColor;
  ledCntrl.setUserColor = &setUserColor;
  
  ledCntrl.BufArray = (uint8_t*)malloc((PREAMBLESIZE+ledCount*24)*sizeof(uint8_t)); //��������� ������������ ������. ���������� ����� ���������� Heap_Size ���� ��������� �����������.
  ledCntrl.BufSize = (PREAMBLESIZE+ledCount*24)*sizeof(uint8_t); //������ �������
  
  if (!ledCntrl.BufArray) _Error_Handler(__FILE__, __LINE__); //���� ������ �� �������� ���� � �������

  for (int i = 0 ; i < ledCntrl.BufSize ; i++){ //��������� 
    if (i<PREAMBLESIZE) ledCntrl.BufArray[i] = 0; //������ �������� ������ ��� ������ ����� ��� ������
    else ledCntrl.BufArray[i] = DOWN; //��� ��� ����. ����� ������ ����
  }
  
  //������� ���������
  pSet = loadSettings(); 
  tsc_init(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X); //init rgb sensor
  
  //������������� ������� �������
  effAr.count = 0;
  
  //������ ��� ������������ ������������������ �������
  addEffect(&ledsOff);
  addEffect(&sunrise);
  addEffect(&sunset);
  addEffect(&snake);
  addEffect(&discoteka);
  addEffect(&rgbSensor);
  addEffect(&rgbFreeze);
  addEffect(&manual);
  
  menuGet()[11].maxVal = effAr.count;
  
  return &ledCntrl;
}

ledsArray* getLedControl(void){
  return &ledCntrl;
}


void addEffect(pfn_effect nameEff){
  effAr.count++;
  effAr.arr = (pfn_effect*)realloc(effAr.arr, effAr.count * sizeof(pfn_effect)); //�������� ������ ���������� ������ ��� ������ �������
  
  if (!effAr.arr) _Error_Handler(__FILE__, __LINE__); //���� ������ �� �������� ���� � �������

  effAr.arr[effAr.count - 1] = nameEff; //� ����� ������ ������� ������
}


pfn_effect getEffect(uint8_t num){
  return effAr.arr[num % effAr.count]; //�������� ��������� �� ������� � ������� �� �������
}
uint8_t getEffectCount(void){
  return effAr.count; 
}


/*
������ *(led.ptr + i) = mask & p ? 16 : 8;
����������

if (mask & p) //���� ��� � ����� ����������
  led.ptr[i] = 16; //������ �� ������ ������ ���� 1, � ��� ����� 16 � ������
else
  led.ptr[i] = 8; //����� ����� 8 - ��� ��� ����� ����� ��� ������������� � 0

*/

//private func
void setPalitColor( uint16_t num, enum palit p, uint8_t bright){
  ledx led = getLedX(num); //�������� ��������� �� ������
  uint32_t mask = 1 << 23;
  
  for (int i = 0 ; i < 24 ; i++){
    *(led.ptr + i) = mask & p ? UP : DOWN; //���������� ������ �������
    mask >>= 1; //��������� ���
  }
}

void setUserColor( uint16_t num, uint8_t red, uint8_t green, uint8_t blue){
  ledx led = getLedX(num);
  uint32_t p = colorConvToLED(red, green, blue); //��������������� RGB � ������ ��� ��� ������ �� �����
  uint32_t mask = 1 << 23;
  
  for (int i = 0 ; i < 24 ; i++){
    *(led.ptr + i) = mask & p ? UP : DOWN;
    mask >>= 1;
  }
}

uint32_t colorConvToLED(uint8_t red, uint8_t green, uint8_t blue){
  return (red<<redShift | green<<greenShift | blue<<blueShift);
}


void colorConvToRGB(ledx* led){
  uint32_t colorBit = 0;
  
  for (int i = 0 ; i < 24 ; i++)
    if(*(led->ptr + i) == UP)
      colorBit |= 1 << (23 - i);

  led->red   =   (colorBit >> redShift)     & 255;
  led->green =   (colorBit >> greenShift)   & 255;
  led->blue  =   (colorBit >> blueShift)    & 255;
}


ledx getLedX(uint16_t num){
  ledx ledX = {ledCntrl.BufArray + PREAMBLESIZE, //������� � ������� + �������� �����. ��� ������ �� ������� ���������
               0,                   //���������� ����� 
               0,                   //red
               0,                   //green
               0};                  //blue

  if ((PREAMBLESIZE + num*24) > ledCntrl.BufSize) return ledX; //���� ���� �� ������� ������� ������ ������

  ledX.ptr = ledCntrl.BufArray + PREAMBLESIZE + num*24;
  ledX.num = num;
  colorConvToRGB(&ledX); //������ ����� �� ������� � ����������

  return ledX;
}



//effects
void ledsOff(uint32_t* delayTime){
  uint8_t red, green, blue;
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    red = ledCntrl.getLedX(i).red; //�������� ������� �������� ������
    green = ledCntrl.getLedX(i).green;
    blue = ledCntrl.getLedX(i).blue;
    
    //�������� �� ����������
    if(red > 0 ) red--;
    if(green > 0 ) green--;
    if(blue > 0 ) blue--;

    ledCntrl.setUserColor(i, red, green, blue); //�������� � �����
  }
  *delayTime = 10; //��������� �������� ����������
}




void sunrise(uint32_t* delayTime){
  uint8_t red, green, blue;
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    red = ledCntrl.getLedX(i).red;
    green = ledCntrl.getLedX(i).green;
    blue = ledCntrl.getLedX(i).blue;
    
    if(red < pSet->redLimit) red++;
    if(red > pSet->redLimit * 0.8 && green < pSet->greenLimit) green++;
    if(red > pSet->redLimit * 0.8 && green > pSet->greenLimit * 0.8 && blue < pSet->blueLimit) blue++;
    ledCntrl.setUserColor(i, red, green, blue);
  }
  *delayTime = 6000;
}

void sunset(uint32_t* delayTime){
  uint8_t red, green, blue;
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    red = ledCntrl.getLedX(i).red;
    green = ledCntrl.getLedX(i).green;
    blue = ledCntrl.getLedX(i).blue;
    
    if(blue > 0 ) blue--;
    if(blue < pSet->blueLimit * 0.2 && green > 0) green--;
    if(blue < pSet->blueLimit * 0.2 && green < pSet->blueLimit * 0.2 && red > 0) red--;
    ledCntrl.setUserColor(i, red, green, blue);
  }
  *delayTime = 6000;
}


void snake(uint32_t* delayTime){
  uint8_t red = rand()%pSet->redLimit, green = rand()%pSet->greenLimit, blue = rand()%pSet->blueLimit; //�������� ����� � ��������� 0-�����������
  
  for (int i = 1 ; i <= ledCntrl.ledCount ; i++){ //�������� ����� ����������� �� ���� ������
    ledCntrl.setUserColor(i, ledCntrl.getLedX(i-1).red, ledCntrl.getLedX(i-1).green, ledCntrl.getLedX(i-1).blue);
  }
  ledCntrl.setUserColor(0, red, green, blue);//� ������� ������ ����� ����
  
  *delayTime = 500;  
}

void discoteka (uint32_t* delayTime){
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    ledCntrl.setUserColor(i, rand()%pSet->redLimit, rand()%pSet->greenLimit, rand()%pSet->blueLimit); //������� ������������ ���� �� ������������ ���������
  }
  *delayTime = 500;
}

void cloud(uint32_t* delayTime){
  uint8_t bright = rand()%pSet->brightLimit; //��� ����� � ���������� ��������
  
  uint16_t flashCount = pSet->flashLedCount >= pSet->ledCount ? pSet->ledCount : pSet->flashLedCount; //���� ������� ����� ������ ��� ����, �� ��������� ����������� ����������

  for (int i = 1 ; i <= ledCntrl.ledCount ; i++){
    ledCntrl.setUserColor(i, ledCntrl.getLedX(i-1).red, ledCntrl.getLedX(i-1).green, ledCntrl.getLedX(i-1).blue);
  } //������� ��������
  ledCntrl.setUserColor(0, bright, bright, bright);
  
  if (rand()%100 == 1){ //1 �� 100 ��� ����� ������
    uint16_t flash = rand()%(pSet->ledCount - flashCount);
    for (uint16_t i = flash ; i <= flash + flashCount ; i++){
      ledCntrl.setUserColor(i, 255, 255, 255); //blink leds
    }
    HAL_Delay(100);
    for (uint16_t i = flash ; i <= flash + flashCount ; i++){
      uint8_t bright = rand()%pSet->brightLimit;

      ledCntrl.setUserColor(i, bright, bright, bright); //return bright
    }
  }
  
  *delayTime = 500;  
}


void rgbSensor(uint32_t* delayTime){
  tsc_getRawData(&tsc_r, &tsc_g, &tsc_b, &tsc_c);

  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    ledCntrl.setUserColor(i, tsc_r > 255 ? 255 : tsc_r, tsc_g > 255 ? 255 : tsc_g, tsc_b > 255 ? 255 : tsc_b); //rgb data
  }

  *delayTime = 500; 
}

void rgbFreeze(uint32_t* delayTime){
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    ledCntrl.setUserColor(i, tsc_r > 255 ? 255 : tsc_r, tsc_g > 255 ? 255 : tsc_g, tsc_b > 255 ? 255 : tsc_b); //freeze rgb data
  }
  
  *delayTime = 500; 
}

void manual(uint32_t* delayTime){
  for (int i = 0 ; i <= ledCntrl.ledCount ; i++){
    ledCntrl.setUserColor(i, pSet->redLimit, pSet->greenLimit, pSet->blueLimit); //limit vals
  }
  
  *delayTime = 500; 
}


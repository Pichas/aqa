#include "myMenu.h"

volatile extern uint8_t effectIndex;

element_t _MENU[] = {
  //name                  type      pr          nx           ch           mn  cr  mx    cd  
  {"Дата",                FOLDER,   &_MENU[2],  &_MENU[1],   &_MENU[3],   0,  0,  0,    ENTER}, //0
  {"Время",               FOLDER,   &_MENU[0],  &_MENU[2],   &_MENU[7],   0,  0,  0,    ENTER}, //1
  {"Лента",               FOLDER,   &_MENU[1],  &_MENU[0],   &_MENU[11],  0,  0,  0,    ENTER},  //2
  
  {"Год",                 PARAM,    &_MENU[6],  &_MENU[4],   0,           0,  0, 99,    YEAR}, //3
  {"Месяц",               PARAM,    &_MENU[3],  &_MENU[5],   0,           1,  1, 12,    MONTH}, //4
  {"День",                PARAM,    &_MENU[4],  &_MENU[6],   0,           1,  1, 31,    DAY}, //5
  {"<<",                  FOLDER,   &_MENU[5],  &_MENU[3],   _MENU,       0,  0,  0,    ENTER}, //6
  
  {"Час",                 PARAM,    &_MENU[10], &_MENU[8],   0,           0,  0, 23,    HOUR}, //7
  {"Минута",              PARAM,    &_MENU[7],  &_MENU[9],   0,           0,  0, 59,    MINUTE}, //8
  {"Секунда",             PARAM,    &_MENU[8],  &_MENU[10],  0,           0,  0, 59,    SECOND}, //9
  {"<<",                  FOLDER,   &_MENU[9],  &_MENU[7],   _MENU,       0,  0,  0,    ENTER}, //10
  
  {"N Эффекта",           PARAM,    &_MENU[20], &_MENU[12],  0,           0,  0,  50,   CURRENTEFF}, //11
  {"Кол-во светод",       PARAM,    &_MENU[11], &_MENU[13],  0,           0,  0, 300,   LEDCOUNT}, //12
  {"Красный",             PARAM,    &_MENU[12], &_MENU[14],  0,           0,  0, 255,   REDLIMIT}, //13
  {"Зеленый",             PARAM,    &_MENU[13], &_MENU[15],  0,           0,  0, 255,   GREENLIMIT}, //14
  {"Синий",               PARAM,    &_MENU[14], &_MENU[16],  0,           0,  0, 255,   BLUELIMIT}, //15
  {"Яркость",             PARAM,    &_MENU[15], &_MENU[17],  0,           0,  0, 255,   BRIGHTLIMIT}, //16
  {"Кол-во вспышки",      PARAM,    &_MENU[16], &_MENU[18],  0,           0,  0, 300,   FLASHLEDCOUNT},//17
  {"RGB время опр",       PARAM,    &_MENU[17], &_MENU[19],  0,           0,  1,   5,   RGBTIME},//18
  {"Сохранить",           FUNC,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"<<",                  FOLDER,   &_MENU[19], &_MENU[11],  _MENU,       0,  0,   0,   ENTER}, //20
  
  {"Таймеры",               FOLDER,   &_MENU[1],  &_MENU[0],   &_MENU[11],  0,  0,  0,    ENTER},  //2
  {"Рассвет ч",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"Закат ч",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"Реле 0.0",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"Реле 0.1",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"Реле 1.0",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  {"Реле 1.1",           PARAM,     &_MENU[18], &_MENU[20],  0,           0,  0,   0,   FLASHWRITE},//19
  
  {"<<",                  FOLDER,   &_MENU[19], &_MENU[11],  _MENU,       0,  0,   0,   ENTER}, //20
  
};


element_t* mCursor = _MENU;


element_t* menuGet (void){
  return _MENU;
}

void menuMainFunc(void){
  uint16_t curVal = mCursor->curVal;
  switch(mCursor->code){
    case ENTER:
      break;
    case YEAR:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      sDate.Year = curVal;
      HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    }
      break;
    case MONTH:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      sDate.Month = curVal;
      HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    }
      break;
    case DAY:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      sDate.Date = curVal;
      HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    }
      break;
    case HOUR:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      sTime.Hours = curVal;
      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    }
      break;
    case MINUTE:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      sTime.Minutes = curVal;
      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    }
      break;
    case SECOND:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      sTime.Seconds = curVal;
      HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    }
      break;
    case LEDCOUNT:
      getSettings()->ledCount = curVal;
      break;
    case REDLIMIT:
      getSettings()->redLimit = curVal;
      break;
    case GREENLIMIT:
      getSettings()->greenLimit = curVal;
      break;
    case BLUELIMIT:
      getSettings()->blueLimit = curVal;
      break;
    case BRIGHTLIMIT:
      getSettings()->brightLimit = curVal;
      break;
    case FLASHLEDCOUNT:
      getSettings()->flashLedCount = curVal;
      break;
    case CURRENTEFF:
      effectIndex = curVal;
      break;
    case RGBTIME:
      tsc_setIntgrTime(curVal);
      break;
    case FLASHWRITE:
      getSettings()->save();
      break;
     
    default:
      break;
  }
}


void menuLoadActualValToElement(element_t* e){
    switch(e->code){
    case ENTER:
      break;
    case YEAR:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      e->curVal = sDate.Year;
    }
      break;
    case MONTH:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      e->curVal = sDate.Month;
    }
      break;
    case DAY:{
      RTC_DateTypeDef sDate;   
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
      e->curVal = sDate.Date;
    }
      break;
    case HOUR:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      e->curVal = sTime.Hours;
    }
      break;
    case MINUTE:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      e->curVal = sTime.Minutes;
    }
      break;
    case SECOND:{
      RTC_TimeTypeDef sTime;   
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      e->curVal = sTime.Seconds;
    }
      break;
    case LEDCOUNT:
      e->curVal = getSettings()->ledCount;
      break;
    case REDLIMIT:
      e->curVal = getSettings()->redLimit;
      break;
    case GREENLIMIT:
      e->curVal = getSettings()->greenLimit;
      break;
    case BLUELIMIT:
      e->curVal = getSettings()->blueLimit;
      break;
    case BRIGHTLIMIT:
      e->curVal = getSettings()->brightLimit;
      break;
    case FLASHLEDCOUNT:
      e->curVal = getSettings()->flashLedCount;
      break;
    case CURRENTEFF:
      e->curVal = effectIndex;
      break;
    case RGBTIME:
      e->curVal = tsc_getIntgrTime();
      break;
    case FLASHWRITE:
      break;
     
    default:
      break;
  }
  
}

void menuSetNewValToElem(uint16_t nv){
  if (nv < mCursor->minVal || nv == 0xFFFF)       mCursor->curVal = mCursor->maxVal;
  else if (nv > mCursor->maxVal)                  mCursor->curVal = mCursor->minVal;
  else                                            mCursor->curVal = nv;
}




#ifndef __OLED_LCD_H__
#define __OLED_LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"

#include "stdlib.h"
#include "string.h"
#include "i2c.h"


#define OLED_adress 0x78                  //аддрес устройства
#define OLED_SETCONTRAST 0x81             //контраст
#define OLED_DISPLAYALLON_RESUME 0xA4     //отображать изменения в памяти на экране
#define OLED_DISPLAYALLON 0xA5            //игнорировать изменение памяти на экране (заморозить экран)
#define OLED_NORMALDISPLAY 0xA6           //нормальное отображение 
#define OLED_INVERTDISPLAY 0xA7           //инверсные цвета
#define OLED_DISPLAYOFF 0xAE              //выключить экран
#define OLED_DISPLAYON 0xAF               //включить экран
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D
#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 5

#define COMAND 0x00
#define DATA   0x40





void OLED_init(void);
void OLED_refresh(void);
void OLED_blank(void);
void OLED_invertRow(uint8_t row);
void OLED_write(uint8_t row, uint8_t column, uint8_t* data);

void OLED_send_command(uint8_t command);

void OLED_goto(uint8_t row, uint8_t column);
void OLED_goto_start(void);






#ifdef __cplusplus
}
#endif

#endif 











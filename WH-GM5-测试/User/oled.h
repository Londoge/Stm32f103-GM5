#ifndef __OLED_H_
#define __OLED_H_

#define OLED_ADDRESS 0x78
#include "main.h"
#include "i2c.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowString(unsigned char Line, unsigned char Column,char *String);
void OLED_ShowChar(unsigned char Line, unsigned char Column, char Char);

void OLED_SetCursor(unsigned char Y, unsigned char X);


void OLED_WriteCommand(unsigned char command);
void OLED_WriteData(unsigned char data);




#endif


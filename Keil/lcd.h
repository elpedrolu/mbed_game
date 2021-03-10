#ifndef LCD_H
#define LCD_H

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "Driver_SPI.h"

#define PUERTO 0
#define A0     6
#define RESET  8
#define CS     18


static void retardo(uint32_t);
void lcd_init(void);
static void wr_data(unsigned char);
static void wr_cmd(unsigned char);
void lcd_reset(void);
void copy_to_lcd(uint8_t);
int EscribeLetra_L1(uint8_t, bool);
int EscribeLetra_L2(uint8_t, bool);
void EscribeFrase_L1(char str[64]);
void EscribeFrase_L2(char str[64]);
void lcd_clear(void);
void lcd_clear_L1(void);
void lcd_clear_L2(void);
int init_pelota(void);
void mover_pelota_der(int);
void mover_pelota_izq(int);

#endif //LCD_H

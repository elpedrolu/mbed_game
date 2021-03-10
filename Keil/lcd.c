#include "lcd.h"
#include "Arial12x12.h"
#include <stdio.h>
#include <string.h>

extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI *SPIdrv = &Driver_SPI1;

uint8_t buffer[512];

static void retardo(uint32_t n_microsegundos) {
  for(uint32_t cuenta=0;cuenta<20*n_microsegundos;cuenta++);
}

void lcd_init(void) {
  //Inicialización y configuración SPI
  SPIdrv->Initialize(NULL);
  SPIdrv->PowerControl(ARM_POWER_FULL);
  SPIdrv->Control(ARM_SPI_MODE_MASTER |
                           ARM_SPI_CPOL1_CPHA1 |
                           ARM_SPI_MSB_LSB |
                           ARM_SPI_DATA_BITS(8), 20000000);
  //Configuración GPIO
  GPIO_SetDir(PUERTO, A0, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PUERTO, RESET, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PUERTO, CS, GPIO_DIR_OUTPUT);
  //Valores iniciales GPIO
  GPIO_PinWrite(PUERTO, A0, 1);
  GPIO_PinWrite(PUERTO, RESET, 1);
  GPIO_PinWrite(PUERTO, CS, 1);
  //Pulso de reset
  GPIO_PinWrite(PUERTO, RESET, 0);
  retardo(1);
  GPIO_PinWrite(PUERTO, RESET, 1);
  retardo(1000);
}

static void wr_data(unsigned char data) {
  GPIO_PinWrite(PUERTO, CS, 0);
  GPIO_PinWrite(PUERTO, A0, 1);
  SPIdrv->Send(&data, sizeof(data));
  GPIO_PinWrite(PUERTO, CS, 1);
}

static void wr_cmd(unsigned char cmd) {
  GPIO_PinWrite(PUERTO, CS, 0);
  GPIO_PinWrite(PUERTO, A0, 0);
  SPIdrv->Send(&cmd, sizeof(cmd));
  GPIO_PinWrite(PUERTO, CS, 1);
}

void lcd_reset(void) {
  wr_cmd(0xAE); //Display OFF
  wr_cmd(0xA2); //Relación de tensión de pol. del LCD a 1/9
  wr_cmd(0xA0); //Direccionamiento RAM de datos normal
  wr_cmd(0xC8); //Scan de salidas COM normal
  wr_cmd(0x22); //Relación de R interna a 2
  wr_cmd(0x2F); //Power ON
  wr_cmd(0x40); //Display empieza en linea 0
  wr_cmd(0xAF); //Display ON
  wr_cmd(0x81); //Contraste
  wr_cmd(0x17); //Valor contraste
  wr_cmd(0xA4); //Display all points normal
  wr_cmd(0xA6); //LCD Display normal
}

void copy_to_lcd(uint8_t linea) {
  int i;
  switch(linea) {
    case 1:         //Linea 1
      wr_cmd(0x00);
      wr_cmd(0x10);
      wr_cmd(0xB0); //Pagina 0

      for(i=0;i<128;i++) {
        wr_data(buffer[i]);
      }

      wr_cmd(0x00);
      wr_cmd(0x10);
      wr_cmd(0xB1); //Pagina 1

      for(i=128;i<256;i++) {
        wr_data(buffer[i]);
      }
      break;

    case 2:         //Linea 2
      wr_cmd(0x00);
      wr_cmd(0x10);
      wr_cmd(0xB2); //Pagina 2

      for(i=256;i<384;i++) {
        wr_data(buffer[i]);
      }

      wr_cmd(0x00);
      wr_cmd(0x10);
      wr_cmd(0xB3); //Pagina 3

      for(i=384;i<512;i++) {
        wr_data(buffer[i]);
      }
      break;
  }
}

int EscribeLetra_L1(uint8_t letra, bool reset) {
  uint8_t i, valor1, valor2;
  uint16_t comienzo = 0;
  static uint8_t posicionL1 = 0;
  
  if (reset)
    posicionL1 = 0;
  
  comienzo = 25*(letra - ' ');
  
  for(i=0;i<12;i++){
    valor1 = Arial12x12[comienzo+i*2+1];
    valor2 = Arial12x12[comienzo+i*2+2];
    
    buffer[i+posicionL1] = valor1;
    buffer[i+128+posicionL1] = valor2;
  }
  posicionL1 = posicionL1 + Arial12x12[comienzo];
  return 0;
}

int EscribeLetra_L2(uint8_t letra, bool reset) {
  uint8_t i, valor1, valor2;
  uint16_t comienzo = 0;
  static uint16_t posicionL2 = 256;
  
  if (reset)
    posicionL2 = 256;
  
  comienzo = 25*(letra - ' ');
  
  for(i=0;i<12;i++){
    valor1 = Arial12x12[comienzo+i*2+1];
    valor2 = Arial12x12[comienzo+i*2+2];
    
    buffer[i+posicionL2] = valor1;
    buffer[i+128+posicionL2] = valor2;
  }
  posicionL2 = posicionL2 + Arial12x12[comienzo];
  return 0;
}

void EscribeFrase_L1(char str[64]) {
  lcd_clear_L1();
  EscribeLetra_L1(str[0], true);
  for(int i=1;i<strlen(str);i++){
    EscribeLetra_L1(str[i], false);
  }
  copy_to_lcd(1);
}

void EscribeFrase_L2(char str[64]) {
  lcd_clear_L2();
  EscribeLetra_L2(str[0], true);
  for(int i=1;i<strlen(str);i++){
    EscribeLetra_L2(str[i], false);
  }
  copy_to_lcd(2);
}

void lcd_clear(void) {
	lcd_clear_L1();
  lcd_clear_L2();
}

void lcd_clear_L1(void) {
  for(int i=0;i<256;i++){
    buffer[i] = 0x00;
  }
  copy_to_lcd(1);
}

void lcd_clear_L2(void) {
  for(int i=256;i<512;i++){
    buffer[i] = 0x00;
  }
  copy_to_lcd(2);
}

int init_pelota(void) {
	lcd_clear_L1();
	int pos = 52;
  for(int i=pos;i<pos+8;i++){
    buffer[i] = 0xFF;
  }
  copy_to_lcd(1);
	return pos;
}

void mover_pelota_der(int pos_orig) {
	lcd_clear_L1();
	for(int i=pos_orig+1;i<pos_orig+9;i++){
    buffer[i] = 0xFF;
  }
  copy_to_lcd(1);
}

void mover_pelota_izq(int pos_orig) {
	lcd_clear_L1();
	for(int i=pos_orig-1;i<pos_orig+7;i++){
    buffer[i] = 0xFF;
  }
  copy_to_lcd(1);
}

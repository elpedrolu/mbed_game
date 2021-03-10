#ifndef PERIFERICOS_H
#define PERIFERICOS_H

#define PUERTO_LEDS  1
#define LED_1       18
#define LED_2       20
#define LED_3       21
#define LED_4       23

#define PUERTO_JOY  0
#define LEFT        15
#define CENTER      16
#define DOWN        17
#define UP          23
#define RIGHT       24

#define PUERTO_LED_RGB  2
#define RED             3
#define GREEN           2
#define BLUE            1

#define PUERTO_SPEAKER  2
#define SPEAKER         0

#define LM75B_I2C_ADDR  0x48
#define REG_TEMP        0x00

#define MMA7660_I2C_ADDR  0x4C
#define REG_MODE          0x07
#define REG_TILT          0x03

void perif_init(void);

#endif //PERIFERICOS_H

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "perifericos.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

bool init = true;
char str[64];

void control (void const *argument);                             // thread function
void joy (void const *argument);


osThreadId tid_control;                                          // thread id
osThreadId tid_joy;;

osThreadDef (control, osPriorityNormal, 1, 0);                   // thread object
osThreadDef (joy, osPriorityNormal, 1, 0);

int Init_Thread (void) {

  tid_control = osThreadCreate (osThread(control), NULL);
  tid_joy = osThreadCreate (osThread(joy), NULL);
  if (!tid_control || !tid_joy) return(-1);
  return(0);
}

void EINT3_IRQHandler(void) {
  if (LPC_GPIOINT->IO0IntStatR > 0) {
    if (LPC_GPIOINT->IO0IntStatR == 1 << LEFT) {
      LPC_GPIOINT->IO0IntClr = 1 << LEFT;
      osSignalSet(tid_joy, 0x01);
    }
    else if (LPC_GPIOINT->IO0IntStatR == 1 << DOWN) {
      LPC_GPIOINT->IO0IntClr = 1 << DOWN;
      osSignalSet(tid_joy, 0x02);
    }
    else if (LPC_GPIOINT->IO0IntStatR == 1 << UP) {
      LPC_GPIOINT->IO0IntClr = 1 << UP;
      osSignalSet(tid_joy, 0x04);
    }
    else if (LPC_GPIOINT->IO0IntStatR == 1 << CENTER) {
      LPC_GPIOINT->IO0IntClr = 1 << CENTER;
      osSignalSet(tid_joy, 0x08);
    }
    else if (LPC_GPIOINT->IO0IntStatR == 1 << RIGHT) {
      LPC_GPIOINT->IO0IntClr = 1 << RIGHT;
      osSignalSet(tid_joy, 0x10);
    }
  } else LPC_GPIOINT->IO0IntClr = 1 << LEFT | 1 << DOWN | 1 << UP | 1 << CENTER | 1 << RIGHT;
}

void control (void const *argument) {
  osEvent e;
	static int posicion; 
	posicion = init_pelota();
	static char str[64];
  while (1) {
		e = osSignalWait(0x01, 0); // LEFT
    if (e.status == osEventSignal) {
			if (posicion > 0) {
				mover_pelota_izq(posicion);
				posicion--;
			}
    }
		
		e = osSignalWait(0x10, 0); // RIGHT
    if (e.status == osEventSignal) {
			if (posicion < 120) {
				mover_pelota_der(posicion);
				posicion++;
			}				
    }
		sprintf(str,"Posicion: %d-%d", posicion, posicion + 7);
		EscribeFrase_L2(str);
    osDelay(250);
    osThreadYield ();                                           // suspend thread
  }
}

void joy (void const *argument) {
  osEvent e;
  while (1) {
    e = osSignalWait(0x01, 0);
    if (e.status == osEventSignal) {
      osDelay(50);
      if (GPIO_PinRead(PUERTO_JOY, LEFT) == 1) {
        osSignalSet(tid_control, 0x01);
      }
    }
    
    e = osSignalWait(0x02, 0);
    if (e.status == osEventSignal) {
      osDelay(50);
      if (GPIO_PinRead(PUERTO_JOY, DOWN) == 1) {
        osSignalSet(tid_control, 0x02);
      }
    }
    
    e = osSignalWait(0x04, 0);
    if (e.status == osEventSignal) {
      osDelay(50);
      if (GPIO_PinRead(PUERTO_JOY, UP) == 1) {
        osSignalSet(tid_control, 0x04);
      }
    }
    
    e = osSignalWait(0x08, 0);
    if (e.status == osEventSignal) {
      osDelay(50);
      if (GPIO_PinRead(PUERTO_JOY, CENTER) == 1) {
        osSignalSet(tid_control, 0x08);
      }
    }
    
    e = osSignalWait(0x10, 0);
    if (e.status == osEventSignal) {
      osDelay(50);
      if (GPIO_PinRead(PUERTO_JOY, RIGHT) == 1) {
        osSignalSet(tid_control, 0x10);
      }
    }
    osThreadYield ();                                           // suspend thread
  }
}

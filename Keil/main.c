#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "perifericos.h"
#include "lcd.h"

extern int Init_Thread (void);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
  
	perif_init();
  lcd_init();
  lcd_reset();
  lcd_clear();
  
  Init_Thread();
  
  osKernelStart ();                         // start thread execution 
}

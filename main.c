/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions


#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

#include "main.h"
#include "rtc.h"
#include "lcd.h"

#include <string.h>
#include <stdio.h>



osThreadId id_thread_lcd, id_thread_led3;

extern int init_threads_rtc (void);


char lcd_text[2][20];

void lcd_initialize(void);



void lcd_initialize(void) {
	init_lcd();
  reset_lcd();
	sprintf (lcd_text[0], CLEAR_STRING);
  sprintf (lcd_text[1], CLEAR_STRING);
	escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
	escribe_frase_L2(lcd_text[1], sizeof(lcd_text[1]));
	
	copy_to_lcd();
}


void leds_initialize(void) {
	GPIO_SetDir(PORT_LED, PIN_LED0, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED1, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED2, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED3, GPIO_DIR_OUTPUT);
  
  GPIO_PinWrite(PORT_LED, PIN_LED3, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED2, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED1, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED0, 0);
}


void joystick_initialize(void) {
	PIN_Configure(PORT_PINS, PIN_JST_C, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	LPC_GPIOINT->IO0IntEnR |= (1UL << PIN_JST_C);

	NVIC_EnableIRQ(EINT3_IRQn);
}



void init_peripherals () {
  lcd_initialize();
  leds_initialize();
	joystick_initialize();
	
	rtc_initialize(RTC_CIIR_CONFIG);
}



/**
 * Callback from the RTC Interrupt Service Routine
 */
void rtc_handle_interrupt() {
	osSignalSet(id_thread_led3, 0x01);
}


/**
 * GPIO Interrupt Service Routine
 */
void EINT3_IRQHandler() {
	
	if (LPC_GPIOINT->IO0IntStatR & (1 << PIN_JST_C)) {
		rtc_reset_full_time();
  }
	LPC_GPIOINT->IO0IntClr |= (1 << PIN_JST_C);
}
	
	


/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize();
	
	init_peripherals();
	
	init_threads_rtc();

  osKernelStart();
}

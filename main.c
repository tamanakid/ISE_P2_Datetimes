/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions


#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

#include "main.h"
#include "lcd.h"

#include <string.h>
#include <stdio.h>


#define CLEAR_STRING "                    "


#define RTC_ILR_RTCCIF    0

#define RTC_CCR_CLKEN     0
#define RTC_CCR_CTCRST    4
#define RTC_CCR_CCALEN    4

//#define RTC_CIIR_CONFIG   0x000000CF
#define RTC_CIIR_CONFIG   	0x00000002



osThreadId id_thread_rtc, id_thread_led3;

extern void reset_rtc_date (void);

extern int init_threads (void);
extern osThreadId id_thread_rtc;

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


void init_peripherals () {
  lcd_initialize();
  
  GPIO_SetDir(PORT_LED, PIN_LED0, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED1, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED2, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED3, GPIO_DIR_OUTPUT);
  
  GPIO_PinWrite(PORT_LED, PIN_LED3, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED2, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED1, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED0, 0);
  
	/** RTC */
	
  // RTC Reset, Calibration and Enabling
  LPC_RTC->CCR = ((1 << RTC_CCR_CCALEN) | (1 << RTC_CCR_CTCRST));
	LPC_RTC->CALIBRATION = 0x00;
	LPC_RTC->CCR = (1 << RTC_CCR_CLKEN);
  
  // RTC Interrupt Configuration
	LPC_RTC->CIIR = RTC_CIIR_CONFIG;
  
  // RTC Interrupts Enabling in NVIC
  NVIC_EnableIRQ(RTC_IRQn);
	
	
	/** JOYSTICKS */
	PIN_Configure(PORT_PINS, PIN_JST_C, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	LPC_GPIOINT->IO0IntEnR |= (1UL << PIN_JST_C);

	NVIC_EnableIRQ(EINT3_IRQn);
}


void RTC_IRQHandler () {
  // Clear ILR "Counter Increment Interrupt" bit
  LPC_RTC->ILR |= (1 << RTC_ILR_RTCCIF);
	
	osSignalSet(id_thread_led3, 0x01);
}



/**
 * GPIO Interrupt Service Routine
 */
void EINT3_IRQHandler() {
	
	if (LPC_GPIOINT->IO0IntStatR & (1 << PIN_JST_C)) {
		reset_rtc_date();
  }
	LPC_GPIOINT->IO0IntClr |= (1 << PIN_JST_C);
}
	
	


/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize();
	
	init_peripherals();
	
	init_threads();

  osKernelStart();
}

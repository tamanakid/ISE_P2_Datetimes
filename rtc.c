#include <string.h>
#include <stdio.h>

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

#include "lcd.h"


#define PORT_LED 1
#define PIN_LED0 18
#define PIN_LED1 20
#define PIN_LED2 21
#define PIN_LED3 23
#define CLEAR_STRING "                    "

#define RTC_ILR_RTCCIF    0
#define RTC_CCR_CLKEN     0
#define RTC_CIIR_IMSEC    0
#define RTC_CIIR_IMMIN    1



extern int init_thread_rtc (void)

char lcd_text[2][20];

void lcd_initialize(void) {
	init_lcd();
  reset_lcd();
	// sprintf (lcd_text[0], CLEAR_STRING);
  sprintf (lcd_text[1], CLEAR_STRING);
	
	snprintf(lcd_text[0], sizeof(lcd_text[0]), "%d", 56);
	escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
	
	copy_to_lcd();
}

void lcd_write() {
	snprintf(lcd_text[0], sizeof(lcd_text[0]), "%d", 813573);
	escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
	
	copy_to_lcd();
}


int main () {
  lcd_initialize();
  
  GPIO_SetDir(PORT_LED, PIN_LED0, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED1, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED2, GPIO_DIR_OUTPUT);
  GPIO_SetDir(PORT_LED, PIN_LED3, GPIO_DIR_OUTPUT);
  
  GPIO_PinWrite(PORT_LED, PIN_LED3, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED2, 1);
  GPIO_PinWrite(PORT_LED, PIN_LED1, 0);
  GPIO_PinWrite(PORT_LED, PIN_LED0, 1);
  
  // Enables time counters
  LPC_RTC->CCR |= (1 << RTC_CCR_CLKEN);
  
  // Generates interrupt every minute 
  LPC_RTC->CIIR |= (1 << RTC_CIIR_IMSEC);
  LPC_RTC->CIIR |= (1 << RTC_CIIR_IMMIN);
  LPC_RTC->ALMIN = 1; // Necessary?
  
  
  // Enable Interrupts for RTC
  NVIC_EnableIRQ(RTC_IRQn);
}


void RTC_IRQHandler () {
  int seconds = (int) LPC_RTC->SEC;
  
  // snprintf(lcd_text[0], sizeof(lcd_text[0]), "%d", seconds);
	// escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
  
  // Clear ILR "Counter Increment Interrupt" bit
  LPC_RTC->ILR |= (1 << RTC_ILR_RTCCIF);
	lcd_write();
	
  // copy_to_lcd();
}


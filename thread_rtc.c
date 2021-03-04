#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "cmsis_os.h"

#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"

#include "main.h"
#include "lcd.h"


extern char lcd_text[2][20];


void thread_rtc (void const *argument);
void thread_led3 (void const *argument);

osThreadDef (thread_rtc, osPriorityNormal, 1, 0);
osThreadDef (thread_led3, osPriorityNormal, 1, 0);


int read_bits (int first_bit, int last_bit, uint32_t reg);


int init_threads (void) {

  id_thread_rtc = osThreadCreate (osThread(thread_rtc), NULL);
	id_thread_led3 = osThreadCreate (osThread(thread_led3), NULL);
  if (!id_thread_rtc || !id_thread_led3) return(-1);
  
  return(0);
}


void thread_led3 (void const *argument) {
	static bool led3_on = true;
	const int delay = 250;
	int seconds;
	
	while (1) {
		osSignalWait(0x01, osWaitForever);

		do {
			led3_on = !led3_on;
			GPIO_PinWrite(PORT_LED, PIN_LED3, led3_on);
			osDelay(delay);
			
			uint32_t ctime0 = LPC_RTC->CTIME0;
			seconds = read_bits(0, 5, ctime0);
		} while (seconds < 5);
		
		led3_on = false;
		GPIO_PinWrite(PORT_LED, PIN_LED3, led3_on);
	}
}


void thread_rtc (void const *argument) {

  while (1) {	
		uint32_t ctime0 = LPC_RTC->CTIME0;
		int seconds = read_bits(0, 5, ctime0);
		int minutes = read_bits(8, 13, ctime0);
		int hours = read_bits(16, 20, ctime0);
		
		snprintf(lcd_text[0], sizeof(lcd_text[0]), "%.2d:%.2d:%.2d", hours, minutes, seconds);
		escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
		
		uint32_t ctime1 = LPC_RTC->CTIME1;
		int day = read_bits(0, 4, ctime1);
		int month = read_bits(8, 11, ctime1);
		int year = read_bits(16, 27, ctime1);
		
		snprintf(lcd_text[1], sizeof(lcd_text[1]), "%.2d/%.2d/%.4d", day, month, year);
		escribe_frase_L2(lcd_text[1], sizeof(lcd_text[1]));
	
		copy_to_lcd();
		osDelay(100);
  }
}


void reset_rtc_date () {
	LPC_RTC->SEC = INIT_DATE_SEC;
	LPC_RTC->MIN = INIT_DATE_MIN;
	LPC_RTC->HOUR = INIT_DATE_HOUR;
	LPC_RTC->DOM = INIT_DATE_DOM;
	LPC_RTC->MONTH = INIT_DATE_MONTH;
	LPC_RTC->YEAR = INIT_DATE_YEAR;
}


int read_bits (int first_bit, int last_bit, uint32_t reg) {
	int and_operator = pow((double) 2, last_bit - first_bit + 1) - 1;
	
	uint32_t value = (reg >> first_bit) & and_operator;
	return (int) value;
}

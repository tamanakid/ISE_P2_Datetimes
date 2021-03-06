#include <stdbool.h>
#include <stdio.h>

#include "cmsis_os.h"
#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"

#include "rtc.h"
#include "lcd.h"

#include "main.h"



void thread_lcd (void const *argument);
void thread_led3 (void const *argument);

osThreadDef (thread_lcd, osPriorityNormal, 1, 0);
osThreadDef (thread_led3, osPriorityNormal, 1, 0);


int init_threads_rtc (void) {

  id_thread_lcd = osThreadCreate (osThread(thread_lcd), NULL);
	id_thread_led3 = osThreadCreate (osThread(thread_led3), NULL);
  if (!id_thread_lcd || !id_thread_led3) return(-1);
  
  return(0);
}


void thread_led3 (void const *argument) {
	static bool led3_on = false;
	const int delay = 750;

	while (1) {
		osSignalWait(0x01, osWaitForever);

		do {
			led3_on = !led3_on;
			GPIO_PinWrite(PORT_LED, PIN_LED3, led3_on);
			osDelay(delay);
		} while (rtc_seconds < 5);
		
		led3_on = false;
		GPIO_PinWrite(PORT_LED, PIN_LED3, led3_on);
	}
}


void thread_lcd (void const *argument) {

  while (1) {
		rtc_get_full_time();
		
		snprintf(lcd_text[0], sizeof(lcd_text[0]),
						 "%.2d:%.2d:%.2d", rtc_hours, rtc_minutes, rtc_seconds);
		escribe_frase_L1(lcd_text[0], sizeof(lcd_text[0]));
		
		snprintf(lcd_text[1], sizeof(lcd_text[1]),
						 "%.2d/%.2d/%.4d", rtc_days, rtc_months, rtc_years);
		escribe_frase_L2(lcd_text[1], sizeof(lcd_text[1]));
	
		copy_to_lcd();
		osDelay(100);
  }
}

#include "cmsis_os.h"



#define INIT_DATE_SEC 		55
#define INIT_DATE_MIN 		00
#define INIT_DATE_HOUR 		00
#define INIT_DATE_DOM 		01
#define INIT_DATE_MONTH		01
#define INIT_DATE_YEAR 		2000


#define PORT_LED 1
#define PIN_LED0 18
#define PIN_LED1 20
#define PIN_LED2 21
#define PIN_LED3 23

#define PORT_PINS 	0
#define PIN_JST_C		16

#define CLEAR_STRING "                    "

//#define RTC_CIIR_CONFIG   0x000000CF
#define RTC_CIIR_CONFIG   	0x00000002 // Interrupt every minute



/** From RTC threads */

extern osThreadId id_thread_lcd, id_thread_led3;

extern int init_threads_rtc (void);


/* From Main */

extern char lcd_text[2][20];

extern void lcd_initialize(void);
void leds_initialize(void);
void joystick_initialize(void);
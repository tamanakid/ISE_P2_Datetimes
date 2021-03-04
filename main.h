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



extern char lcd_text[2][20];

extern int init_thread_rtc (void);

extern void reset_rtc_date (void);

extern osThreadId id_thread_rtc, id_thread_led3;

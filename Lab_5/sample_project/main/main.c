#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "FreeRTOSConfig.h"

TimerHandle_t xTimer_1;
TimerHandle_t xTimer_2;

uint8_t counter_timer_1 = 0;
uint8_t counter_timer_2 = 0;

TickType_t xTimeNow;
void callback_func(TimerHandle_t xTimer)
{
	xTimeNow = (xTaskGetTickCount() - 1)/100;
	const uint8_t *timer_id = pvTimerGetTimerID(xTimer);
	if (timer_id == 111)
	{
		printf("ahihi ------------ %d \r\n", xTimeNow);
		counter_timer_1++;
		if (counter_timer_1 == 10)
		{
			printf("=========================================\n");
			printf("After 10 times printing, the first timer has been stopped \r\n");
			printf("=========================================\n");
			printf("Exit program! \r\n");
			xTimerStop(xTimer, 0);
		}
	}
	else if (timer_id == 222)
	{
		printf("----- ihaha ------- %d \r\n", xTimeNow);
		counter_timer_2++;
		if (counter_timer_2 == 5)
		{
			printf("=========================================\n");
			printf("After 5 times printing, the secound timer has been stopped \r\n");
			printf("=========================================\n");
			xTimerStop(xTimer, 0);
		}
	}
}

void app_main()
{
	xTimer_1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000), pdTRUE, (void *)111, callback_func);
	xTimer_2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000), pdTRUE, (void *)222, callback_func);

	if (xTimer_1 != NULL && xTimer_2 != NULL)
	{
		printf("=========================================\n");
		printf("Start counting timer \r\n");
		printf("=========================================\n");
		xTimerStart(xTimer_1, 0);
		xTimerStart(xTimer_2, 0);
	}
	else
		printf("One of any timer is not created successfully\r\n");
}
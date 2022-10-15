#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#define BTN_GPIO 18
#define STUDENT_ID 1915144

void print_mssv(void *pvParameter)
{
	while (1)
	{
		printf("%d\n", STUDENT_ID);
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void button_task(void *pvParameter)
{
	gpio_pad_select_gpio(BTN_GPIO);
	gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BTN_GPIO, GPIO_PULLUP_ONLY);
	while (1)
	{
		if (gpio_get_level(BTN_GPIO) == 1)
			printf("ESP32\n");
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
	vTaskDelete(NULL);
}

void app_main()
{
	xTaskCreate(&print_mssv, "print_mssv", 4096, NULL, 10, NULL);
	xTaskCreate(&button_task, "button_task", 4096, NULL, 10, NULL);
}
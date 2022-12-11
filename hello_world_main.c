/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#define QUEUE_SIZE 10

#define TOTAL_TASK 3
#define TASK1_ID 0
#define TASK2_ID 1
#define TASK3_ID 2
#define DUMB_ID 9

struct Request
{
    int taskID;
    char data[10];
};

QueueHandle_t xQueue;
bool isRead[TOTAL_TASK];

int genTaskID()
{
    int tmp = rand() % 10;
    if (tmp > 8)
        return DUMB_ID;
    else
        return rand() % TOTAL_TASK;
}

void clearAllRead()
{
    for (int i = 0; i < TOTAL_TASK; ++i)
    {
        isRead[i] = false;
    }
}

bool isAllRead()
{
    bool result = true;
    for (int i = 0; i < TOTAL_TASK; ++i)
    {
        result = result && isRead[i];
    }
    return result;
}

void printQueueInfo(QueueHandle_t xQueue)
{
    printf("-----------------\n");
    printf("QUEUE INFO\n");
    printf("Number of Request in queue   : %d\n", uxQueueMessagesWaiting(xQueue));
    printf("Number of free item in queue: %d\n", uxQueueSpacesAvailable(xQueue));
    printf("-----------------\n");
}

void receptionTask(void *pvParameter)
{
    struct Request xRequest;
    while (1)
    {
        xRequest.taskID = genTaskID();
        if (xQueueSendToBack(xQueue, &xRequest, (TickType_t)10) != pdPASS)
        {
            printf("ReceptionTask: Send failed!!!\n");
        }
        else
        {
            printf("ReceptionTask: Send TaskID: %d\n", xRequest.taskID+1);
        }
        printQueueInfo(xQueue);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void functionalTask1(void *pvParameter)
{
    struct Request xRequest;
    while (1)
    {
        // Check Receive from queue but don't delete item in queue '
        if (xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS)
        {
            printf("FunctionalTask1: Receive failed!!!\n");
        }
        else
        {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASK1_ID)
            {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearAllRead();
                // HANDLE
                printf("FunctionalTask1: Handle task1\n");
            }
            else
            {
                isRead[TASK1_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void functionalTask2(void *pvParameter)
{
    struct Request xRequest;
    while (1)
    {
        if (xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS)
        {
            printf("FunctionalTask2: Receive failed!!!\n");
        }
        else
        {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASK2_ID)
            {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearAllRead();
                // HANDLE
                printf("FunctionalTask2: Handle task2\n");
            }
            else
            {
                isRead[TASK2_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void functionalTask3(void *pvParameter)
{
    struct Request xRequest;
    while (1)
    {
        if (xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS)
        {
            printf("FunctionalTask3: Receive failed!!!\n");
        }
        else
        {
            // printf("Receive ID: %d\n", xRequest.taskID);
            if (xRequest.taskID == TASK3_ID)
            {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                clearAllRead();
                // HANDLE
                printf("FunctionalTask3: Handle task3\n");
            }
            else
            {
                isRead[TASK3_ID] = true;
            }
        }
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void errorHandlerTask(void *pvParameter)
{
    struct Request xRequest;
    while (1)
    {
        if (xQueuePeek(xQueue, &xRequest, (TickType_t)10) != pdPASS)
        {
            printf("ErrorHandlerTask: Receive failed!!!\n");
        }
        else
        {
            // printf("Receive ID: %d\n",xRequest.taskID);
            if (isAllRead())
            {
                xQueueReceive(xQueue, &xRequest, (TickType_t)10);
                // HANDLE
                printf("ErrorHandlerTask: Handle\n");
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
    xQueue = xQueueCreate(QUEUE_SIZE, sizeof(struct Request *));
    clearAllRead();

    xTaskCreate(&receptionTask, "ReceptionTask", 2048, NULL, 1, NULL);
    xTaskCreate(&functionalTask1, "FunctionalTask1", 2048, NULL, 1, NULL);
    xTaskCreate(&functionalTask2, "FunctionalTask2", 2048, NULL, 1, NULL);
    xTaskCreate(&functionalTask3, "FunctionalTask3", 2048, NULL, 1, NULL);
    xTaskCreate(&errorHandlerTask, "ErrorHandlerTask", 2048, NULL, 1, NULL);
}
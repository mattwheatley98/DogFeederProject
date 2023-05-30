//
// Created by matt on 5/12/23.
//

#include <Arduino.h>
#include "../../.pio/libdeps/esp32dev/Keypad/src/Keypad.h"
#include "keypadTask.h"

#define ROWS 4
#define COLS 4

//Keypad-related variables and objects
char hexaKeys[ROWS][COLS] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};
uint8_t rowPins[ROWS] = {23, 19, 18, 5};
uint8_t colPins[COLS] = {32, 33, 15, 4};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//Variables and a buffer that are passed as arguments to daytimeButtonPressed
char am[5] = " AM";
char pm[5] = " PM";
char setTimeBuffer[50];
char setCycleBuffer[50];

void daytimeButtonPressed(SemaphoreHandle_t inputSemaphore, char key, char timeInput[],
                          QueueHandle_t inputQueue, char daytime[], SemaphoreHandle_t doneSemaphore);

void clearCycleInput(int &count);

void keypadTask(void *parameter) {
    while (1) {
        char key = customKeypad.getKey();
        //Allows the setting of an AM feeding time
        if (key == 'A') {
            daytimeButtonPressed(
                    inputSemaphoreA,
                    key,
                    setTimeBuffer,
                    daytimeInputQueueA,
                    am,
                    doneSemaphoreA
            );
            //Allows the setting of a PM feeding time
        } else if (key == 'B') {
            daytimeButtonPressed(
                    inputSemaphoreB,
                    key,
                    setTimeBuffer,
                    daytimeInputQueueB,
                    pm,
                    doneSemaphoreB
            );
            //Allows the setting of feeding cycles
        } else if (key == 'C') {
            vTaskSuspend(feedingTaskHandle);
            xSemaphoreGive(setCyclesSemaphore);
            int count = 0;
            while (1) {
                key = customKeypad.getKey();
                //Clears cycles if above 20
                if (atoi(setCycleBuffer) > 20) {
                    clearCycleInput(count);
                    //Continues so that count isn't incremented
                    continue;
                }
                if (key) {
                    Serial.println(key);
                    if (key >= '0' && key <= '9') {
                        setCycleBuffer[count] = key;
                        Serial.println(setCycleBuffer);
                    } else if (key == 'C') {
                        clearCycleInput(count);
                        //Continues so that count isn't incremented
                        continue;
                    } else if (key == 'D') {
                        xSemaphoreGive(saveCyclesSemaphore);
                        break;
                    }
                    else {
                        continue;
                    }
                    count++;
                    //Sends the number of cycles to queues relating to the display and feeding tasks accordingly
                    xQueueSend(displayCyclesQueue, setCycleBuffer, 0);
                    xQueueSend(feedingCyclesQueue, setCycleBuffer, 0);
                }
            }
            //Triggers a manual feeding
        } else if (key == '*') {
            xSemaphoreGive(timerFeedingSemaphore);
        }
        vTaskResume(feedingTaskHandle);
    }
}

//Function that can be used to set AM and PM feeding times
void daytimeButtonPressed(SemaphoreHandle_t inputSemaphore, char key, char timeInput[],
                          QueueHandle_t inputQueue, char daytime[], SemaphoreHandle_t doneSemaphore) {
    vTaskSuspend(feedingTaskHandle);
    xSemaphoreGive(inputSemaphore); //Input semaphore
    int count = 0;
    while (1) {
        key = customKeypad.getKey();
        if (count == 2 || count == 5) {
            timeInput[count] = ':'; //Time input
            count++;
            xQueueSend(inputQueue, timeInput, 0);
        } else if (count == 8) {
            strcat(timeInput, daytime);
            Serial.println(timeInput);
            count = 0;
            xQueueSend(inputQueue, timeInput, 0);
            memset(timeInput, 0, 50 * sizeof(char));
            //Delay to allow the Queue to send properly
            vTaskDelay(250 / portTICK_PERIOD_MS);
            xSemaphoreGive(doneSemaphore);
            vTaskResume(feedingTaskHandle);
            break;
        }
        if (key) {
            Serial.println(key);
            if (key >= '0' && key <= '9') {
                timeInput[count] = key;
                Serial.println(timeInput);
            } else {
                continue;
            }
            count++;
            xQueueSend(inputQueue, timeInput, 0);
            Serial.println(count);
        }
    }
}

//Function that clears the set number of cycles and updates the display accordingly
void clearCycleInput(int &count) {
    memset(setCycleBuffer, 0, sizeof(setCycleBuffer));
    setCycleBuffer[0] = '1';
    xQueueSend(displayCyclesQueue, setCycleBuffer, 0);
    xQueueSend(feedingCyclesQueue, setCycleBuffer, 0);
    count = 0;
}


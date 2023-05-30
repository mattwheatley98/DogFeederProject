//
// Created by matt on 5/10/23.
//

#include <Arduino.h>
#include "feedingTask.h"
#include "../../.pio/libdeps/esp32dev/ESP32Servo/src/ESP32Servo.h"

#define SERVO_PIN_OPENER 13
#define SERVO_PIN_PUSHER 12

//Number of cycles
char cycles[50] = "4";

//Servo object
Servo servoOpener;
Servo servoPusher;

void feedingTask(void *parameter) {
    servoOpener.attach(SERVO_PIN_OPENER, 500, 2400);
    servoPusher.attach(SERVO_PIN_PUSHER, 500, 2400);
    vTaskDelay(250 / portTICK_PERIOD_MS);

    while(1) {
        //Receives cycles input from the keypad task and prints to the serial monitor upon receival
        if (xQueueReceive(feedingCyclesQueue, cycles, 0) == pdTRUE) {
            Serial.println("Received cycles from keypad task!");
        }
        //Actuates the servo motors to perform feedings based on the number of cycles
        if(xSemaphoreTake(timerFeedingSemaphore, 0)) {
            vTaskSuspend(keypadTaskHandle);
            servoOpener.write(15);
            xSemaphoreGive(feedingDisplaySemaphore);
            for (int i = 0; i < atoi(cycles); ++i) {
                servoPusher.write(90);
                vTaskDelay(500   / portTICK_PERIOD_MS);
                servoPusher.write(180);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                servoPusher.write(90);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                servoPusher.write(0);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            //Default servo opener state
        } else {
            servoOpener.write(90);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            vTaskResume(keypadTaskHandle);
        }
    }
}
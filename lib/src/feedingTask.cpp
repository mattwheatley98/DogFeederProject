//
// Created by matt on 5/10/23.
//

#include <Arduino.h>
#include "feedingTask.h"
#include "../../.pio/libdeps/esp32dev/ESP32Servo/src/ESP32Servo.h"

#define POTENTIOMETER_PIN 34
#define SERVO_PIN_OPENER 13
#define SERVO_PIN_PUSHER 12

char cycleActuationBuffer[50] = "4";


//Servo object
Servo servoOpener;
Servo servoPusher;

void feedingTask(void *parameter) {
    servoOpener.attach(SERVO_PIN_OPENER, 500, 2400);
    servoPusher.attach(SERVO_PIN_PUSHER, 500, 2400);
    vTaskDelay(250 / portTICK_PERIOD_MS);

    while(1) {
        //Reads the value from the potentiometer and divides the 12 bit unsigned number by 16
        //to have a 255 maximum for the DC motor's duty cycle (can't go higher, per documentation)
        if (xQueueReceive(feedingCyclesActuationQueue, cycleActuationBuffer, 0) == pdTRUE) {
            Serial.println("Received from FEEDING TASK");
        }
        if(xSemaphoreTake(timerFeedingSemaphore, 0)) {
            vTaskSuspend(keypadTaskHandle);
            servoOpener.write(15);
            xSemaphoreGive(feedingDisplaySemaphore);
            for (int i = 0; i < atoi(cycleActuationBuffer); ++i) {
                servoOpener.write(15);
                servoPusher.write(90);
                vTaskDelay(500   / portTICK_PERIOD_MS);
                servoPusher.write(180);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                servoPusher.write(90);
                vTaskDelay(500 / portTICK_PERIOD_MS);
                servoPusher.write(0);
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            //Default servo and DC motor state
        } else {
            servoOpener.write(90);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            vTaskResume(keypadTaskHandle);
        }
    }
}
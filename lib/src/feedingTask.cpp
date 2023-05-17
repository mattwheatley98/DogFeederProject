//
// Created by matt on 5/10/23.
//

#include <Arduino.h>
#include "feedingTask.h"
#include "../../.pio/libdeps/esp32dev/ESP32Servo/src/ESP32Servo.h"

#define ENABLE_PIN  14
#define MOTOR_PIN1 27
#define MOTOR_PIN2 26
#define POTENTIOMETER_PIN 34
#define SERVO_PIN 13

//DC motor-related variables
const int freq = 30000;
const int pwmChannel = 2;
const int resolution = 8;
int dutyCycle = 255;
int readValue;
char cycleActuationBuffer[50] = "4";


//Servo object
Servo myServo;

void feedingTask(void *parameter) {
    myServo.attach(SERVO_PIN, 500, 2400);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_PIN1, OUTPUT);
    pinMode(MOTOR_PIN2, OUTPUT);
    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(ENABLE_PIN, pwmChannel);
    vTaskDelay(250 / portTICK_PERIOD_MS);

    while(1) {
        //Reads the value from the potentiometer and divides the 12 bit unsigned number by 16
        //to have a 255 maximum for the DC motor's duty cycle (can't go higher, per documentation)
        readValue = analogRead(POTENTIOMETER_PIN) / 16;
        if (xQueueReceive(feedingCyclesActuationQueue, cycleActuationBuffer, 0) == pdTRUE) {
            Serial.println("Received from FEEDING TASK");
        }
        //Manual trigger for feeding
        if (readValue > 170) {
            vTaskSuspend(keypadTaskHandle);
            myServo.write(15);
            dutyCycle = readValue;
            ledcWrite(pwmChannel, dutyCycle);
            digitalWrite(MOTOR_PIN1, HIGH);
            digitalWrite(MOTOR_PIN2, LOW);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            xSemaphoreGive(manualFeedingSemaphore);
            //Time-based trigger for feeding
        } else if(xSemaphoreTake(timerFeedingSemaphore, 0)) {
            vTaskSuspend(keypadTaskHandle);
            dutyCycle = 255;
            myServo.write(15);
            xSemaphoreGive(feedingDisplaySemaphore);
            for (int i = 0; i < atoi(cycleActuationBuffer); ++i) {
                ledcWrite(pwmChannel, dutyCycle);
                digitalWrite(MOTOR_PIN1, HIGH);
                digitalWrite(MOTOR_PIN2, LOW);
                vTaskDelay(1500 / portTICK_PERIOD_MS);
                digitalWrite(MOTOR_PIN1, LOW);
                digitalWrite(MOTOR_PIN2, LOW);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            //Default servo and DC motor state
        } else {
            myServo.write(90);
            dutyCycle = 0;
            ledcWrite(pwmChannel, dutyCycle);
            digitalWrite(MOTOR_PIN1, LOW);
            digitalWrite(MOTOR_PIN2, LOW);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            vTaskResume(keypadTaskHandle);
        }
    }
}
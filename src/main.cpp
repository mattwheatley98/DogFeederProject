#include <Arduino.h>
#include "../lib/src/feedingTask.h"
#include "../lib/src/displayTask.h"
#include "../lib/src/keypadTask.h"
#include <WiFi.h>

QueueHandle_t feedingCyclesQueue; //Queue for passing the number of feeding cycles from keypadTask to feedingTask
QueueHandle_t displayCyclesQueue; //Queue for passing the number of feeding cycles from keypadTask to displayTask
QueueHandle_t daytimeInputQueueA; //Queue for AM feeding time input
QueueHandle_t daytimeInputQueueB; //Queue for PM feeding time input
SemaphoreHandle_t timerFeedingSemaphore; //Semaphore for triggering feedings
SemaphoreHandle_t inputSemaphoreA; //Semaphore to trigger AM feeding inputs
SemaphoreHandle_t inputSemaphoreB; //Semaphore to trigger PM feeding inputs
SemaphoreHandle_t blinkSemaphoreA; //Semaphore to signal AM display blinking
SemaphoreHandle_t blinkSemaphoreB; //Semaphore to signal PM display blinking
SemaphoreHandle_t blinkSemaphoreCycles; //Semaphore to signal cycle setting display blinking
SemaphoreHandle_t doneSemaphoreA; //Semaphore to signal that AM time input has completed
SemaphoreHandle_t doneSemaphoreB; //Semaphore to signal that PM time input has completed
SemaphoreHandle_t setCyclesSemaphore; //Semaphore to signal the setting of feeding cycles
SemaphoreHandle_t saveCyclesSemaphore; //Semaphore that signals when cycles are to be saved
SemaphoreHandle_t feedingDisplaySemaphore; //Semaphore that signals to the display that feeding began

//Handles used for task suspension
TaskHandle_t feedingTaskHandle;
TaskHandle_t keypadTaskHandle;

//Timer handles for blinking
TimerHandle_t blinkingTimerA;
TimerHandle_t blinkingTimerB;
TimerHandle_t blinkingCyclesTimer;

const char *ssid = "REDACTED";
const char *password = "REDACTED";

//Function that uses a timer's ID to determine which semaphore to send and which serial message to print
void timerCallback(TimerHandle_t xTimer) {
    int timerId = (int) pvTimerGetTimerID(xTimer);

    switch (timerId) {
        case 1:
            xSemaphoreGive(blinkSemaphoreA);
            Serial.println("Timer A!");
            break;
        case 2:
            xSemaphoreGive(blinkSemaphoreB);
            Serial.println("Timer B!");
            break;
        case 3:
            xSemaphoreGive(blinkSemaphoreCycles);
            Serial.println("Cycles Timer!");
            break;
    }
}

void setup() {
    Serial.begin(9600);

    feedingCyclesQueue = xQueueCreate(3, 20 * sizeof(char));
    displayCyclesQueue = xQueueCreate(3, 20 * sizeof(char));
    daytimeInputQueueA = xQueueCreate(5, 20 * sizeof(char));
    daytimeInputQueueB = xQueueCreate(5, 20 * sizeof(char));
    timerFeedingSemaphore = xSemaphoreCreateBinary();
    inputSemaphoreA = xSemaphoreCreateBinary();
    inputSemaphoreB = xSemaphoreCreateBinary();
    blinkSemaphoreA = xSemaphoreCreateBinary();
    blinkSemaphoreB = xSemaphoreCreateBinary();
    blinkSemaphoreCycles = xSemaphoreCreateBinary();
    doneSemaphoreA = xSemaphoreCreateBinary();
    doneSemaphoreB = xSemaphoreCreateBinary();
    setCyclesSemaphore = xSemaphoreCreateBinary();
    saveCyclesSemaphore = xSemaphoreCreateBinary();
    feedingDisplaySemaphore = xSemaphoreCreateBinary();

    //Begin connecting to WiFi and print a message if successful
    WiFi.begin(ssid, password);
    delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi network!");

    xTaskCreate(
            feedingTask,
            "Feeding Task",
            1200,
            nullptr,
            0,
            &feedingTaskHandle
    );
    xTaskCreate(
            displayTask,
            "Display Task",
            2000,
            nullptr,
            0,
            nullptr
    );
    xTaskCreate(
            keypadTask,
            "Keypad Task",
            1200,
            nullptr,
            0,
            &keypadTaskHandle
    );
    //Timers that synchronize blinking on the display (first two for setting time, third one for setting cycles)
    blinkingTimerA = xTimerCreate(
            "Blinking Timer A",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 1,
            timerCallback
    );
    blinkingTimerB = xTimerCreate(
            "Blinking Timer B",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 2,
            timerCallback
    );
    blinkingCyclesTimer = xTimerCreate(
            "Blinking Cycles Timer",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 3,
            timerCallback
    );
}

void loop() {
    //Leave empty
}
#include <Arduino.h>
#include "../lib/src/feedingTask.h"
#include "../lib/src/displayTask.h"
#include "../lib/src/keypadTask.h"
#include <WiFi.h>

//Handles relating to feedingTask
SemaphoreHandle_t manualFeedingSemaphore;
SemaphoreHandle_t timerFeedingSemaphore;
QueueHandle_t feedingCyclesQueue;

//Handles relating to keypadTask and displayTask
QueueHandle_t daytimeInputQueueA;
QueueHandle_t daytimeInputQueueB;
SemaphoreHandle_t inputSemaphoreA;
SemaphoreHandle_t inputSemaphoreB;
SemaphoreHandle_t blinkSemaphoreA;
SemaphoreHandle_t blinkSemaphoreB;
SemaphoreHandle_t doneSemaphoreA;
SemaphoreHandle_t doneSemaphoreB;
TimerHandle_t blinkingTimerA;
TimerHandle_t blinkingTimerB;
SemaphoreHandle_t setCyclesSemaphore;
SemaphoreHandle_t saveCyclesSemaphore;
TimerHandle_t blinkingCyclesTimer;
SemaphoreHandle_t blinkSemaphoreCycles;
SemaphoreHandle_t feedingDisplaySemaphore;

//Task handles for suspension
TaskHandle_t feedingTaskHandle;
TaskHandle_t keypadTaskHandle;

QueueHandle_t feedingCyclesActuationQueue;


const char *ssid = "REDACTED";
const char *password = "REDACTED";

void timerCallbackA(TimerHandle_t xTimer) {
    xSemaphoreGive(blinkSemaphoreA);
    Serial.println("Timer A!");
}

void timerCallbackB(TimerHandle_t xTimer) {
    xSemaphoreGive(blinkSemaphoreB);
    Serial.println("Timer B!");
}

void timerCallbackCycles(TimerHandle_t xTimer) {
    xSemaphoreGive(blinkSemaphoreCycles);
    Serial.println("Cycles Timer!");
}

void setup() {
    Serial.begin(9600);

    manualFeedingSemaphore = xSemaphoreCreateBinary();
    timerFeedingSemaphore = xSemaphoreCreateBinary();
    feedingCyclesQueue = xQueueCreate(3, 20 * sizeof(char));

    daytimeInputQueueA = xQueueCreate(5, 20 * sizeof(char));
    daytimeInputQueueB = xQueueCreate(5, 20 * sizeof(char));
    inputSemaphoreA = xSemaphoreCreateBinary();
    inputSemaphoreB = xSemaphoreCreateBinary();
    blinkSemaphoreA = xSemaphoreCreateBinary();
    blinkSemaphoreB = xSemaphoreCreateBinary();
    doneSemaphoreA = xSemaphoreCreateBinary();
    doneSemaphoreB = xSemaphoreCreateBinary();
    setCyclesSemaphore = xSemaphoreCreateBinary();
    blinkSemaphoreCycles = xSemaphoreCreateBinary();
    saveCyclesSemaphore = xSemaphoreCreateBinary();
    feedingDisplaySemaphore = xSemaphoreCreateBinary();

    feedingCyclesActuationQueue = xQueueCreate(3, 20 * sizeof(char));

    //Begin connecting to WiFi and print a message if successful
    WiFi.begin(ssid, password);
    delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi network!");

    //Synchronizes blinking for the displays when feeding times are being set
    blinkingTimerA = xTimerCreate(
            "Blinking Timer A",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 0,
            timerCallbackA
    );
    blinkingTimerB = xTimerCreate(
            "Blinking Timer B",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 1,
            timerCallbackB
    );
    blinkingCyclesTimer = xTimerCreate(
            "Blinking Cycles Timer",
            500 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 2,
            timerCallbackCycles
    );
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
}

void loop() {
    //Leave empty
}
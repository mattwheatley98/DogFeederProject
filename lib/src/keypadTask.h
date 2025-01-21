//
// Created by matt on 5/12/23.
//

#pragma once

extern TaskHandle_t feedingTaskHandle;
extern QueueHandle_t daytimeInputQueueA;
extern QueueHandle_t daytimeInputQueueB;
extern SemaphoreHandle_t inputSemaphoreA;
extern SemaphoreHandle_t inputSemaphoreB;
extern TimerHandle_t blinkingTimerA;
extern SemaphoreHandle_t blinkSemaphoreA;
extern SemaphoreHandle_t doneSemaphoreA;
extern SemaphoreHandle_t doneSemaphoreB;
extern SemaphoreHandle_t setCyclesSemaphore;
extern QueueHandle_t displayCyclesQueue;
extern TimerHandle_t blinkingCyclesTimer;
extern SemaphoreHandle_t blinkSemaphoreCycles;
extern SemaphoreHandle_t saveCyclesSemaphore;
extern QueueHandle_t feedingCyclesQueue;
extern SemaphoreHandle_t feedingDisplaySemaphore;
extern SemaphoreHandle_t timerFeedingSemaphore;

void keypadTask(void *parameter);

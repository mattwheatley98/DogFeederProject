//
// Created by matt on 5/10/23.
//

#pragma once

extern SemaphoreHandle_t timerFeedingSemaphore;
extern QueueHandle_t displayCyclesQueue;
extern QueueHandle_t daytimeInputQueueA;
extern QueueHandle_t daytimeInputQueueB;
extern SemaphoreHandle_t inputSemaphoreA;
extern SemaphoreHandle_t inputSemaphoreB;
extern TimerHandle_t blinkingTimerA;
extern TimerHandle_t blinkingTimerB;
extern SemaphoreHandle_t blinkSemaphoreA;
extern SemaphoreHandle_t blinkSemaphoreB;
extern SemaphoreHandle_t doneSemaphoreA;
extern SemaphoreHandle_t doneSemaphoreB;
extern SemaphoreHandle_t setCyclesSemaphore;
extern TimerHandle_t blinkingCyclesTimer;
extern SemaphoreHandle_t blinkSemaphoreCycles;
extern SemaphoreHandle_t saveCyclesSemaphore;
extern QueueHandle_t feedingCyclesQueue;
extern SemaphoreHandle_t feedingDisplaySemaphore;

void displayTask(void *parameter);

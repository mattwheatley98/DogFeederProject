//
// Created by matt on 5/10/23.
//

#ifndef DOGFEEDERPROJECT_FEEDINGTASK_H
#define DOGFEEDERPROJECT_FEEDINGTASK_H

#endif //DOGFEEDERPROJECT_FEEDINGTASK_H

extern SemaphoreHandle_t timerFeedingSemaphore;
extern QueueHandle_t displayCyclesQueue;
extern TaskHandle_t keypadTaskHandle;
extern QueueHandle_t feedingCyclesQueue;
extern SemaphoreHandle_t feedingDisplaySemaphore;

void feedingTask(void *parameter);


//
// Created by matt on 5/10/23.
//

#include <Arduino.h>
#include </home/matt/CLionProjects/DogFeederProject/.pio/libdeps/esp32dev/U8g2/src/U8g2lib.h>
#include "displayTask.h"
#include <ctime>

//Variables and an object for setting up the display and retrieving the current time (EST) from an NTP server
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;
char currentDate[30];
char currentTime[30];

//Non-ntp-related variables
char feedTimeA[30] = "09:11:15 AM";
char feedTimeB[30] = "06:28:50 PM";
char displayBuffer[50];
char cycleDisplayBuffer[50] = "4";
boolean display = true;
char countBuffer[10];

//Function that has the display reflect keypad input from keypadTask
void feedInput(TimerHandle_t blinkingTimerHandle, SemaphoreHandle_t feedBlinkSemaphore, QueueHandle_t daytimeInputQueue,
               char feedTime[], void (*blinkDisplay)(), SemaphoreHandle_t doneSemaphore);

//Makes the AM and PM parts of the display blink accordingly
void blinkDisplayA();
void blinkDisplayB();

//Gets the local time from an NTP server
void getLocalTime();

void displayTask(void *parameter) {
    //Configures the system's time settings with the necessary offsets for EST
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    u8g2.begin();
    while (1) {
        u8g2.clearBuffer();
        if (strcmp(currentTime, feedTimeA) == 0 || strcmp(currentTime, feedTimeB) == 0) {
            xSemaphoreGive(timerFeedingSemaphore);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            getLocalTime();
        } else if (xSemaphoreTake(feedingDisplaySemaphore, 0) == pdTRUE) {
            u8g2.setFont(u8g2_font_10x20_tf);
            for (int i = 1; i < (atoi(cycleDisplayBuffer) + 1); ++i) {
                u8g2.clearBuffer();
                //Spaces the display's count if it is more than one digit
                if (i > 9) {
                    u8g2.drawStr(24, 20, "FEEDING!");
                    u8g2.drawStr(32, 44, itoa(i, countBuffer, 10));
                    u8g2.drawStr(58, 44, "/");
                    u8g2.drawStr(74, 44, cycleDisplayBuffer);
                } else {
                    u8g2.drawStr(24, 20, "FEEDING!");
                    u8g2.drawStr(42, 44, itoa(i, countBuffer, 10));
                    u8g2.drawStr(58, 44, "/");
                    u8g2.drawStr(74, 44, cycleDisplayBuffer);
                }
                u8g2.sendBuffer();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
            }
        } else if (xSemaphoreTake(inputSemaphoreA, 0) == pdTRUE) {
            feedInput(
                    blinkingTimerA,
                    blinkSemaphoreA,
                    daytimeInputQueueA,
                    feedTimeA,
                    blinkDisplayA,
                    doneSemaphoreA
            );
        } else if (xSemaphoreTake(inputSemaphoreB, 0) == pdTRUE) {
            feedInput(
                    blinkingTimerB,
                    blinkSemaphoreB,
                    daytimeInputQueueB,
                    feedTimeB,
                    blinkDisplayB,
                    doneSemaphoreB
            );
        } else if (xSemaphoreTake(setCyclesSemaphore, 0) == pdTRUE) {
            xTimerStart(blinkingCyclesTimer, 0);
            while (1) {
                u8g2.clearBuffer();
                if (xSemaphoreTake(blinkSemaphoreCycles, 0) == pdTRUE) {
                    display = !display;
                }
                if (xQueueReceive(displayCyclesQueue, cycleDisplayBuffer, 0) == pdTRUE) {
                    Serial.print("From DISPLAY CYCLE: ");
                    Serial.println(cycleDisplayBuffer);
                }
                u8g2.setFont(u8g2_font_8x13_tf);
                u8g2.drawStr(8, 10, "Feeding Cycles");
                u8g2.setFont(u8g2_font_10x20_tf);
                //Shifts the cycle's number over if it's more than one digit
                if (display) {
                    if (atoi(cycleDisplayBuffer) > 9) {
                        u8g2.drawStr(54, 34, cycleDisplayBuffer);
                    } else {
                        u8g2.drawStr(60, 34, cycleDisplayBuffer);
                    }
                } else {}
                u8g2.setFont(u8g2_font_6x10_tf);
                u8g2.drawStr(2, 50, "\"C\" to clear cycles");
                u8g2.drawStr(2, 62, "\"D\" to save and exit");
                u8g2.sendBuffer();
                if (xSemaphoreTake(saveCyclesSemaphore, 0) == pdTRUE) {
                    xTimerStop(blinkingCyclesTimer, 0);
                    break;
                }
            }
            //Default display state
        } else {
            u8g2.setFont(u8g2_font_6x10_tf);
            u8g2.drawStr(0, 10, currentDate);
            u8g2.setFont(u8g2_font_10x20_tf);
            u8g2.drawStr(8, 32, currentTime);
            u8g2.setFont(u8g2_font_6x10_tf);
            u8g2.drawStr(0, 50, "Feed at: ");
            u8g2.drawStr(50, 50, feedTimeA);
            u8g2.drawStr(0, 60, "Feed at: ");
            u8g2.drawStr(50, 60, feedTimeB);
            u8g2.sendBuffer();
            getLocalTime();
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
    }
}

//Fetches the current date and time and stores them in currentDate and currentTime accordingly
void getLocalTime() {
    struct tm timeInfo;
    if (!getLocalTime(&timeInfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    strftime(currentDate, 30, "%A, %B %d %Y", &timeInfo);
    strftime(currentTime, 30, "%I:%M:%S %p", &timeInfo);
}

//Function that receives input from keypadTask, handles the blinking feature, and displays keypad input
void feedInput(TimerHandle_t blinkingTimerHandle, SemaphoreHandle_t feedBlinkSemaphore, QueueHandle_t daytimeInputQueue,
               char feedTime[], void (*blinkDisplay)(), SemaphoreHandle_t doneSemaphore) {
    xTimerStart(blinkingTimerHandle, 0);
    while (1) {
        u8g2.clearBuffer();
        if (xSemaphoreTake(feedBlinkSemaphore, 0) == pdTRUE) {
            display = !display;
        }
        if (xQueueReceive(daytimeInputQueue, displayBuffer, 0) == pdTRUE) {
            memset(feedTime, 0, 30 * sizeof(char));
            strcpy(feedTime, displayBuffer);
            Serial.print("From DISPLAY: ");
            Serial.println(feedTime);
        }
        blinkDisplay();
        u8g2.sendBuffer();
        getLocalTime();
        if (xSemaphoreTake(doneSemaphore, 0) == pdTRUE) {
            xTimerStop(blinkingTimerHandle, 0);
            break;
        }
    }
}

//Responsible for blinking the AM feeding time
void blinkDisplayA() {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, currentDate);
    u8g2.setFont(u8g2_font_10x20_tf);
    u8g2.drawStr(8, 32, currentTime);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 50, "Feed at: ");
    if (display) {
        u8g2.drawStr(50, 50, feedTimeA);
    } else {}
    u8g2.drawStr(0, 60, "Feed at: ");
    u8g2.drawStr(50, 60, feedTimeB);
}

//Responsible for blinking the PM feeding time
void blinkDisplayB() {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, currentDate);
    u8g2.setFont(u8g2_font_10x20_tf);
    u8g2.drawStr(8, 32, currentTime);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 50, "Feed at: ");
    u8g2.drawStr(50, 50, feedTimeA);
    u8g2.drawStr(0, 60, "Feed at: ");
    if (display) {
        u8g2.drawStr(50, 60, feedTimeB);
    } else {}
}


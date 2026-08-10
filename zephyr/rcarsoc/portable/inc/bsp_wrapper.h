/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#ifndef BSP_WRAPPER_H
#define BSP_WRAPPER_H

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define portSTACK_TYPE           uint32_t
#define configSTACK_DEPTH_TYPE   uint32_t
#define portMAX_DELAY            UINT32_MAX

#define pdMS_TO_TICKS(ms) ((TickType_t)k_ms_to_ticks_ceil32((uint32_t)(ms)))

#define pdFALSE                 0
#define pdTRUE                  1

typedef portSTACK_TYPE StackType_t;
typedef int BaseType_t;
typedef unsigned int UBaseType_t;
typedef uint32_t TickType_t;

typedef void (*TaskFunction_t)(void *arg);
typedef struct k_sem *SemaphoreHandle_t;

typedef struct
{
    struct k_thread thread;
    k_thread_stack_t *stack;
    size_t stack_size_bytes;
    k_tid_t tid;
} TaskHandle_t;

void vTaskDelay(const TickType_t xTicksToDelay);

BaseType_t xTaskCreate(TaskFunction_t pxTaskCode,
                       const char *const pcName,
                       const configSTACK_DEPTH_TYPE uxStackDepth,
                       void *const pvParameters,
                       UBaseType_t uxPriority,
                       TaskHandle_t *const pxCreatedTask);

BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore,
                          TickType_t xTicksToWait);

BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);

void *pvPortMalloc(size_t xWantedSize);
void *pvPortCalloc(size_t xNum, size_t xSize);
void vPortFree(void *pv);

UBaseType_t taskENTER_CRITICAL_FROM_ISR(void);
void taskEXIT_CRITICAL_FROM_ISR(UBaseType_t uxSavedInterruptStatus);

#endif /* BSP_WRAPPER_H */

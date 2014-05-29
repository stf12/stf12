/**
 * CTask.cpp
 *
 *  Created on: Jul 5, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CTask.h"
#include <assert.h>


CTask::CTask() {
	m_handleTask = NULL;
}

CTask::CTask(TaskHandle_t handleTask) {
	Attach(handleTask);
}

CTask::~CTask() {
	if (IsValid())
		Delete();
}

void CTask::Attach(GenericHandle_t handle) {
	assert(handle != NULL);

	if (IsValid()) {
		Delete();
	}
	m_handleTask = handle;
}

BaseType_t CTask::Create(TaskFunction_t pvTaskCode, const char * const pcName, uint16_t usStackDepth, void *pvParameters, UBaseType_t uxPriority)
{
	BaseType_t res;
	TaskHandle_t handle;
	res = xTaskCreate(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, &handle);
	if (res == pdTRUE)
		Attach(handle);

	return res;
}

BaseType_t CTask::CreateRestricted(TaskParameters_t *pxTaskDefinition)
{
	BaseType_t res = pdFALSE;
#if( portUSING_MPU_WRAPPERS == 1 )
	TaskHandle_t handle;
	res = xTaskCreateRestricted(pxTaskDefinition, &handle);
	if (res == pdTRUE)
		Attach(handle);
#endif

	return res;
}


void CTask::Delete() {
	if (m_handleTask != NULL) {
#if ( INCLUDE_vTaskDelete == 1 )
		TaskHandle_t handleTaskToDelete = m_handleTask;
		m_handleTask = NULL;
		vTaskDelete(handleTaskToDelete);
#else
		vTaskSuspend(m_handleTask);
#endif
		m_handleTask = NULL;
	}
}

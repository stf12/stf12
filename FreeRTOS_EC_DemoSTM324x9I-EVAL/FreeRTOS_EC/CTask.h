/**
 * @file CTask.h
 * @class CTask
 * @ingroup FreeRTOS_Wrapper
 *
 * The CTask class wraps a native FreeRTOS task handle (TaskHandle_t). To create a task instance
 * an object of this class and then call its CTask::Create method like showed in the following sample:
 * \code
 * CTask g_aTask // global task declaration.
 *
 * void main()
 * {
 * 	g_aTask.Create(task_control_func, "Led2", configMINIMAL_STACK_SIZE, mainFLASH_TASK_PRIORITY);
 *
 * 	CFreeRTOS::StartScheduler();
 * }
 * \endcode
 *
 * Usually the application subclass this class to create an application specific task object.
 * See the CHelloWorld class for an example.
 *
 * Note: the API vTaskStartTrace, ulTaskEndTrace are not supported because they are deprecated.
 *
 * \sa <a href="http://www.freertos.org/taskandcr.html">task documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 5, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CTASK_H_
#define CTASK_H_

#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#include "IFreeRTOSProtocol.h"

namespace freertosec {
namespace wrapper {

class CTask: public IFreeRTOSObj {

	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	TaskHandle_t m_handleTask;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a task handle and attach it to the object.
	 */
	CTask();

	/**
	 * Create a CTask instance and attach it to a valid handle.
	 *
	 * @param handleTask a valid task handle.
	 */
	CTask(TaskHandle_t handleTask);

	/**
	 * Delete the native FreeRTOS task.
	 */
	virtual ~CTask();

	/**
	 * Retrieve the native FreeRTOS task handle attached to this object.
	 *
	 * @return the native FreeRTOS task handle attached to this object.
	 */
	inline operator TaskHandle_t() const { return m_handleTask; }

	// FreeRTOS API Wrappers

	/**
	 * Create a FreeRTOS task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/a00125.html">xTaskCreate</a> FreeRTOS API function.
	 */
	BaseType_t Create(TaskFunction_t pvTaskCode, const char * const pcName, uint16_t usStackDepth, void *pvParameters, UBaseType_t uxPriority);

	/**
	 * Create a Memory Protection Unit (MPU) restricted task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/xTaskCreateRestricted.html">xTaskCreateRestricted</a> FreeRTOS API function.
	 */
	BaseType_t CreateRestricted(TaskParameters_t *pxTaskDefinition);

	/**
	 * \sa <a href="http://www.freertos.org/a00126.html">vTaskDelete</a>  FreeRTOS API function.
	 */
	void Delete();

	/**
	 * \sa <a href="http://www.freertos.org/a00127.html">vTaskDelay</a>  FreeRTOS API function.
	 */
	inline static void Delay(const TickType_t xTicksToDelay);

	/**
	 * \sa <a href="http://www.freertos.org/vtaskdelayuntil.html">vTaskDelayUntil</a>  FreeRTOS API function.
	 */
    inline static void DelayUntil(TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement);

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskYIELD">taskYIELD</a>  FreeRTOS API function.
     */
    inline static void Yeld() { taskYIELD(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENTER_CRITICAL">taskENTER_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void EnterCritical() { taskENTER_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskEXIT_CRITICAL">taskEXIT_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void ExitCritical() { taskEXIT_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskDISABLE_INTERRUPTS">taskDISABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void DisableInterrupt() { taskDISABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENABLE_INTERRUPTS">taskENABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void EnableInterrupt() { taskENABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetCurrentTaskHandle">xTaskGetCurrentTaskHandle</a>  FreeRTOS API function.
     */
    inline static TaskHandle_t GetCurrentTaskHandle();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetTickCount">xTaskGetTickCount</a>  FreeRTOS API function.
     */
    inline static TickType_t GetTickCount();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetSchedulerState">xTaskGetSchedulerState</a>  FreeRTOS API function.
     */
    inline static BaseType_t GetSchedulerState();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#usTaskGetNumberOfTasks">uxTaskGetNumberOfTasks</a>  FreeRTOS API function.
     */
    inline static UBaseType_t GetNumberOfTasks();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskList">vTaskList</a>  FreeRTOS API function.
     */
    inline static void GetTaskList(char *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskGetRunTimeStats">vTaskGetRunTimeStats</a>  FreeRTOS API function.
     */
    inline static void GetRunTimeStats(char *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00128.html">uxTaskPriorityGet</a>  FreeRTOS API function.
     */
    inline UBaseType_t PriorityGet() const;

    /**
     * \sa <a href="http://www.freertos.org/a00129.html">vTaskPrioritySet</a>  FreeRTOS API function.
     */
    inline void PrioritySet(UBaseType_t uxNewPriority);

    /**
     * \sa <a href="http://www.freertos.org/a00130.html">vTaskSuspend</a>  FreeRTOS API function.
     */
    inline void Suspend();

    /**
     * \sa <a href="http://www.freertos.org/a00131.html">vTaskResume</a>  FreeRTOS API function.
     */
    inline void Resume();

    /**
     * \sa <a href="http://www.freertos.org/taskresumefromisr.html">xTaskResumeFromISR</a>  FreeRTOS API function.
     */
    inline BaseType_t ResumeFromISR();

    /**
     * \sa <a href="http://www.freertos.org/vTaskSetApplicationTag.html">vTaskSetApplicationTaskTag</a>  FreeRTOS API function.
     */
    inline void SetApplicationTag(TaskHookFunction_t pxTagValue);

    /**
     * \sa <a href="http://www.freertos.org/xTaskCallApplicationTaskHook.html">xTaskCallApplicationTaskHook</a>  FreeRTOS API function.
     */
    inline BaseType_t CallApplicationTaskHook(void *pvParameter);

    /**
     * \sa <a href="http://www.freertos.org/vTaskAllocateMPURegions.html">vTaskAllocateMPURegions</a>  FreeRTOS API function.
     */
    inline void AllocateMPURegions(const MemoryRegion_t * const xRegions);

    /* Since FreeRTOS v8 and FreeRTOS_EC v2.0.0*/

    /**
     * \sa <a href="http://www.freertos.org/uxTaskGetSystemState.html">uxTaskGetSystemState</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline static UBaseType_t GetSystemState(TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime);

    /**
     * \sa <a href="http://www.freertos.org/xTaskGetApplicationTaskTag.html">xTaskGetApplicationTaskTag</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline TaskHookFunction_t GetApplicationTaskTag();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetIdleTaskHandle">xTaskGetIdleTaskHandle</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline static TaskHandle_t GetIdleTaskHandle();

    /**
     * \sa <a href="http://www.freertos.org/uxTaskGetStackHighWaterMark.html">uxTaskGetStackHighWaterMark</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline UBaseType_t GetStackHighWaterMark();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#eTaskGetState">eTaskGetState</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline eTaskState GetState() const;

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#pcTaskGetTaskName">pcTaskGetTaskName</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline char *GetTaskName() const;

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetTickCountFromISR">xTaskGetTickCountFromISR</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline static TickType_t GetTickCountFromISR();

    /**
     * \sa <a href="http://www.freertos.org/vTaskSetApplicationTag.html">vTaskSetApplicationTaskTag</a>  FreeRTOS API function.
     * \since FreeRTOS_EC v2.0.0 (FreeRTOS vX.X.X todo: to check FreeRTOS version)
     */
    inline void SetApplicationTaskTag(TaskHookFunction_t pxTagValue);

    // FreeRTOS class extension.
	inline bool IsValid() const;
	void Attach(GenericHandle_t handle);
	inline GenericHandle_t Detach();
};

// inline method
////////////////

inline
void CTask::Delay(const TickType_t xTicksToDelay) {
#if ( INCLUDE_vTaskDelay == 1 )
	vTaskDelay(xTicksToDelay);
#endif
}

inline
void CTask::DelayUntil(TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement) {
#if ( INCLUDE_vTaskDelayUntil == 1 )
	vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
#endif
}

inline
TaskHandle_t CTask::GetCurrentTaskHandle() {
	return xTaskGetCurrentTaskHandle();
}

inline
TickType_t CTask::GetTickCount() {
	return xTaskGetTickCount();
}

inline
BaseType_t CTask::GetSchedulerState() {
#if ( INCLUDE_xTaskGetSchedulerState == 1 )
	return xTaskGetSchedulerState();
#else
	return 0;
#endif
}

inline
UBaseType_t CTask::GetNumberOfTasks() {
	return uxTaskGetNumberOfTasks();
}

inline
void CTask::GetTaskList(char *pcWriteBuffer) {
#if ( configUSE_TRACE_FACILITY == 1 )
	vTaskList(pcWriteBuffer);
#endif
}

inline
void CTask::GetRunTimeStats(char *pcWriteBuffer) {
#if (configGENERATE_RUN_TIME_STATS == 1)
	vTaskGetRunTimeStats(pcWriteBuffer);
#else
	(void)pcWriteBuffer;
#endif
}

inline
UBaseType_t CTask::PriorityGet() const {
#if ( INCLUDE_uxTaskPriorityGet == 1 )
	return uxTaskPriorityGet(m_handleTask);
#else
	return 0;
#endif
}

inline
void CTask::PrioritySet(UBaseType_t uxNewPriority) {
#if ( INCLUDE_vTaskPrioritySet == 1 )
	vTaskPrioritySet(m_handleTask, uxNewPriority);
#endif
}

inline
void CTask::Suspend() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskSuspend(m_handleTask);
#endif
}

inline
void CTask::Resume() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskResume(m_handleTask);
#endif
}

inline
BaseType_t CTask::ResumeFromISR() {
#if ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) )
	return xTaskResumeFromISR(m_handleTask);
#else
	return pdFALSE;
#endif
}

inline
void CTask::SetApplicationTag(TaskHookFunction_t pxTagValue) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	vTaskSetApplicationTaskTag(m_handleTask, pxTagValue);
#else
	(void)pxTagValue;
#endif
}

inline
BaseType_t CTask::CallApplicationTaskHook(void *pvParameter) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	return xTaskCallApplicationTaskHook(m_handleTask, pvParameter);
#else
	(void)pvParameter;
	return 0;
#endif
}

inline
void CTask::AllocateMPURegions(const MemoryRegion_t * const xRegions) {
#if ( portUSING_MPU_WRAPPERS == 1 )
	vTaskAllocateMPURegions(m_handleTask, xRegions);
#else
	(void)xRegions;
#endif
}

inline
bool CTask::IsValid() const{
	return m_handleTask != NULL;
}

inline
GenericHandle_t CTask::Detach() {
	TaskHandle_t res = m_handleTask;
	m_handleTask = NULL;
	return res;
}

inline
UBaseType_t CTask::GetSystemState(
		TaskStatus_t * const pxTaskStatusArray,
		const UBaseType_t uxArraySize,
		uint32_t * const pulTotalRunTime) {
#if ( configUSE_TRACE_FACILITY == 1 )
	return uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, pulTotalRunTime);
#else
	return 0;
#endif
}

inline
TaskHookFunction_t CTask::GetApplicationTaskTag() {
#if ( configUSE_APPLICATION_TASK_TAG == 1)
	return xTaskGetApplicationTaskTag(m_handleTask);
#else
	return NULL;
#endif
}

inline
TaskHandle_t CTask::GetIdleTaskHandle() {
#if ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
	return  xTaskGetIdleTaskHandle();
#else
	return NULL;
#endif
}

inline
UBaseType_t CTask::GetStackHighWaterMark() {
#if ( INCLUDE_uxTaskGetStackHighWaterMark  == 1)
	return uxTaskGetStackHighWaterMark(m_handleTask);
#else
	return 0;
#endif
}

inline
eTaskState CTask::GetState() const {
#if ( INCLUDE_eTaskGetState == 1 )
	return xTaskGetIdleTaskHandle(m_handleTask);
#else
	return eSuspended;
#endif
}

inline
char *CTask::GetTaskName() const {
#if ( INCLUDE_pcTaskGetTaskName == 1 )
	return pcTaskGetTaskName(m_handleTask);
#else
	return NULL;
#endif
}

inline
TickType_t CTask::GetTickCountFromISR() {
	return xTaskGetTickCountFromISR();
}

inline
void CTask::SetApplicationTaskTag(TaskHookFunction_t pxTagValue) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	vTaskSetApplicationTaskTag(m_handleTask, pxTagValue);
#endif
}

} /* namespace wrapper */
} /* namespace freertosec */

#endif /* CTASK_H_ */

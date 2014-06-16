/**
 * CQueueSet.cpp
 *
 *  Created on: June .3, 2014
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include <CQueueSet.h>

CQueueSet::CQueueSet() {
	m_handleQueueSet = NULL;

}

CQueueSet::CQueueSet(QueueSetHandle_t handleQueueSet) {
	Attach(handleQueueSet);
}

CQueueSet::~CQueueSet() {
}

void CQueueSet::Attach(GenericHandle_t handle) {
	assert(handle != NULL);

	m_handleQueueSet = handle;
}

CQueueSet &CQueueSet::CreateSet(const UBaseType_t uxEventQueueLength) {
#if (configUSE_QUEUE_SETS == 1)
	assert(!IsValid());

	QueueSetHandle_t handle;

	handle = xQueueCreateSet(uxEventQueueLength);
	if (handle != NULL)
		Attach(handle);
#endif
	return *this;
}


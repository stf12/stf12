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
//	if (IsValid())
//		Delete();
}

void CQueueSet::Attach(GenericHandle_t handle) {
	assert(handle != NULL);

//	if (IsValid())
//		Delete();

	m_handleQueueSet = handle;
}


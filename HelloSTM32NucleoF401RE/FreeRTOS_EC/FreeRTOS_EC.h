/*
 * FreeRTOS_EC.h
 *
 *  Created on: Jun 24, 2014
 *      Author: oliveris
 */

#ifndef FREERTOS_EC_H_
#define FREERTOS_EC_H_

#include "FreeRTOS.h"

// Include the FreeRTOS_EC Wrapper layer
#include "wrapper/IFreeRTOSProtocol.h"
#include "wrapper/IQueueSetMember.h"
#include "wrapper/ATimer.h"
#include "wrapper/ASyncObject.h"
#include "wrapper/CBinarySemaphore.h"
#include "wrapper/CCountingSemaphore.h"
#include "wrapper/CMutex.h"
#include "wrapper/CRecursiveMutex.h"
#include "wrapper/CQueue.h"
#include "wrapper/CQueueSet.h"
#include "wrapper/CFreeRTOS.h"
#include "wrapper/CTask.h"


// Include the FreeRTOS_EC Managed layer
#if ( configUSE_FREERTOS_EC_MANAGED_EXTENSION == 1 )

#include "managed/IManagedApplicationDelegate.h"
#include "managed/IMessageTaskDelegate.h"
#include "managed/AManagedTask.h"
#include "managed/AManagedApplicationDelegate.h"
#include "managed/CManagedFreeRTOSApp.h"
#include "managed/CMessageTask.h"
#include "managed/CMTContext.h"
#include "managed/CSharedResource.h"
#include "managed/Message.h"
#include "managed/MessageMacro.h"

#endif

// Include the FreeRTOS_EC Math layer
//#include "arm_math.h"
//#include "math/CMatrix.h"


#endif /* FREERTOS_EC_H_ */

/*
 * CINemo.h
 *
 *  Created on: Nov 9, 2010
 *      Author: oliveris
 */

#ifndef CINEMO_H_
#define CINEMO_H_

#include "CQueue.h"
#include "CBinarySemaphore.h"
#include "iNEMO_lib.h"

class CINemo {
//	friend void USB_LP_CAN1_RX0_IRQHandler(void);
//	friend void TIM2_IRQHandler(void);

public:
	static CQueue s_usbQueue;
	static CBinarySemaphore s_timSemaphore;
	static iNEMO_DATA s_xSharedData;
public:

	static void StartTasks();
};

#endif /* CINEMO_H_ */

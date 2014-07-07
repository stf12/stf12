/*
 * CMyTask.h
 *
 *  Created on: 27/giu/2014
 *      Author: stefano
 */

#ifndef CMYTASK_H_
#define CMYTASK_H_

#include "stm32f4xx_nucleo.h"

using freertosec::managed::AManagedTask;
using freertosec::managed::CMTContext;

struct TIM_HandleTypeDefEx {
	TIM_HandleTypeDef xTimerHandle;
	uint64_t nTimerTickCount;
};

class CMyTask: public freertosec::managed::AManagedTask {
public:

	// Constant to configure the tests.
	enum {
		MATRIX_INVERSE_COUNT = 10 //< Specifies the number of cycles ...
	};


private:

	TIM_HandleTypeDefEx m_xTimerHandle;


	/**
	 * Execute a test for each method of the class.
	 *
	 * @return true if success, false otherwise
	 */
	bool doMatrixTest1();

	bool doMatrixTest2();

	void LowLevelHadwareInit();

public:

	CMyTask(CMTContext *pContext);
	virtual ~CMyTask();

	void Run();
	bool HardwareInit();

	uint64_t GetTimerTickCount() const { return m_xTimerHandle.nTimerTickCount; }
};

#endif /* CMYTASK_H_ */

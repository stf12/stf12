/*
 * CMyTask.cpp
 *
 *  Created on: 27/giu/2014
 *      Author: stefano
 */

#include <FreeRTOS_EC.h>
#include "CMyTask.h"
#include "diag/Trace.h"
#include "math/CMatrixF32.h"
#include "math/CMatrix.h"
#include "iNEMO_math.h"

using namespace freertosec::math;

static float32_t s_fHilbertMAtrix5x5_data[] = {
		1.0/1.0, 1.0/2.0, 1.0/3.0, 1.0/4.0, 1.0/5.0,
		1.0/2.0, 1.0/3.0, 1.0/4.0, 1.0/5.0, 1.0/6.0,
		1.0/3.0, 1.0/4.0, 1.0/5.0, 1.0/6.0, 1.0/7.0,
		1.0/4.0, 1.0/5.0, 1.0/6.0, 1.0/7.0, 1.0/8.0,
		1.0/5.0, 1.0/6.0, 1.0/7.0, 1.0/8.0, 1.0/9.0
};

static float32_t s_m3x3_data[] = {
		6.0, -7.0, 10.0,
		0.0, 3.0, -1.0,
		0.0, 5.0, -7.0
};

static  float32_t s2_m3x3_data[] = {
		1.1, 1.2, 1.3,
		2.1, 2.2, 2.3,
		3.1, 3.2, 3.3
};

static float s3_m7x7_data[] = {
		1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 6.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 7.0
};

static TIM_HandleTypeDef *s_pCurrentTimerHandle = NULL;

CMyTask::CMyTask(CMTContext *pContext): AManagedTask(pContext) {
	// TODO Auto-generated constructor stub

}

CMyTask::~CMyTask() {
	// TODO Auto-generated destructor stub
}

bool CMyTask::HardwareInit() {
	BSP_LED_Init(LED2);
	BSP_LED_Off(LED2);

	// TIM3 initialization.
	m_xTimerHandle.xTimerHandle.Instance = TIM3;
	m_xTimerHandle.nTimerTickCount = 0;
	s_pCurrentTimerHandle = &m_xTimerHandle.xTimerHandle;
	LowLevelHadwareInit();

	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
//	uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 100000) - 1;

	/* Initialize TIM3 peripheral as follows:
	       + Period = 10000 - 1
	       + Prescaler = ((SystemCoreClock/2)/10000) - 1
	       + ClockDivision = 0
	       + Counter direction = Up
	 */
//	m_xTimerHandle.xTimerHandle.Init.Period = 10000 - 1;
	m_xTimerHandle.xTimerHandle.Init.Period = 10 - 1;
	m_xTimerHandle.xTimerHandle.Init.Prescaler = uwPrescalerValue;
	m_xTimerHandle.xTimerHandle.Init.ClockDivision = 0;
	m_xTimerHandle.xTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&m_xTimerHandle.xTimerHandle) != HAL_OK)
	{
		/* Initialization Error */
		while (1);
	}

	return true;
}

void CMyTask::Run() {
	int n = 1;

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if(HAL_TIM_Base_Start_IT(&m_xTimerHandle.xTimerHandle) != HAL_OK)
	{
		/* Starting Error */
		DisableInterrupt();
		while (1);
	}

	// Stack overflow detection test
//	char buff[512];
//	buff[1] = 12;

	doMatrixTest1();
	doMatrixTest2();
	doMatrixTest3();

	for(;;) {
		trace_printf("My first task: %i\n", n);
		Delay(2000 / portTICK_PERIOD_MS);
		BSP_LED_Toggle(LED2);
		n++;
	}
}

void CMyTask::LowLevelHadwareInit() {
	// Configure TIM3. This code is implemented in the void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) function

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIM3 Peripheral clock enable */
	__TIM3_CLK_ENABLE();

	/*##-2- Configure the NVIC for TIMx #########################################*/
	/* Set the TIMx priority */
	HAL_NVIC_SetPriority(TIM3_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY-2, 0);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

bool CMyTask::doMatrixTest1() {
	bool bRes = true;
	CMatrixF32<2, 2> m1, m2;

	// The subscript operator []
	m1[0][0] = 1.0;
	m1[0][1] = 0.0;
	m1[1][0] = 0.0;
	m1[1][1] = 1.0;

	m2[0][0] = 1.5;
	m2[0][1] = 0.5;
	m2[1][0] = 0.5;
	m2[1][1] = 1.5;

	// The copy constructor operator CMatrixF32(const CMatrixF32 &obj);
	CMatrixF32<2,2> m3 = m2;

	// Arithmetic member unary operator + - *
	CMatrixF32<2,2> sum = m1 + m2;
	CMatrixF32<2,2> minus = m1 - m2;
	CMatrixF32<2,2> scalar = m1 * 0.12;

	// Binary non member operator *
	CMatrixF32<2,2> mul = m1 * m2;

	// Inverse algorithm.
	CMatrixF32<3,3> m4, m4Inv, m4Check;
	m4[0][0] = 2.0; m4[0][1] = 0.0;	m4[0][2] = 0.0;
	m4[1][0] = 0.0;	m4[1][1] = 2.0;	m4[1][2] = 0.0;
	m4[2][0] = 0.0;	m4[2][1] = 0.0;	m4[2][2] = 2.0;
	CMatrixF32<3,3> m4Clone;
	m4Inv = m4.Clone(m4Clone).Inverse();
	m4Check = m4 * m4Inv;
	// Conditional operator;
	CMatrixF32<3,3> mIdentity;
	mIdentity.InitWithIdentity();
	bRes = !(m4Check != mIdentity);

	// Transpose method
	m4[0][0] = 1.0; m4[0][1] = 0.0;	m4[0][2] = 0.0;
	m4[1][0] = 2.0;	m4[1][1] = 2.0;	m4[1][2] = 0.0;
	m4[2][0] = 3.0;	m4[2][1] = 3.0;	m4[2][2] = 3.0;
	m4.Transpose();

	// Assignment operator =
	CMatrixF32<3,3> m5;
	m5 = m4;
	m5 = 16.0;
	m5 = s2_m3x3_data;

	// Assignments modify & return lvalue
	m4.InitWithOne();
	m4 += m5;
	m4 -= m5;
	m4 *= 2.0;

	// LUP decomposition.
	m4.InitWithArray(s_m3x3_data);
	CMatrixF32<3,1> p;
	int nPerm = m4.LUP(p);

	// Test others method
	m5.InitWithZero();

	return bRes;
}

bool CMyTask::doMatrixTest2() {
	bool bRes = true;
	uint64_t nStartTime;

	// Start the test
	nStartTime = GetTimerTickCount();

	// Load the matrix data.
	CMatrixF32<5,5> identity, m1;
	m1 = s_fHilbertMAtrix5x5_data;
	identity.InitWithIdentity();

	// Make MATRIX_INVERSE_COUNT inversion using the CMatrixF32 class
	//***************************************************************
	for (int i=0; i<MATRIX_INVERSE_COUNT; ++i) {
		CMatrixF32<5,5> mClone = m1;
		CMatrixF32<5,5> mInverse = mClone.Inverse();
		CMatrixF32<5,5> mCheck = m1 * mInverse;
	}

	uint64_t nInverseTestTime = GetTimerTickCount() - nStartTime;
	trace_printf("Inversion of %i matrix<5,5> = %i ticks\n", MATRIX_INVERSE_COUNT, nInverseTestTime);


	// Make the test using iNEMO_math
	//*******************************
	nStartTime = GetTimerTickCount();

	iNEMO_fMATRIX_TYPE *in_identity = iNEMO_fMatCreateInit(5, 5, iNEMO_IDEN_MATRIX);
	iNEMO_fMATRIX_TYPE *in_m1 = iNEMO_fMatCreate(5, 5);
	for (int i=0; i<5; i++)
		for (int j=0; j<5; j++)
			iNEMO_MatData(in_m1)[i][j] = s_fHilbertMAtrix5x5_data[(i*5)+j];
	iNEMO_fMATRIX_TYPE *in_Inverse = iNEMO_fMatCreate(5, 5);
	iNEMO_fMATRIX_TYPE *in_Check = iNEMO_fMatCreate(5, 5);
	for (int i=0; i<MATRIX_INVERSE_COUNT; ++i) {
		iNEMO_fMatInv(in_m1, in_Inverse);
		iNEMO_fMatMulMat(in_m1, in_Inverse, in_Check);
	}

	uint64_t nIN_InverseTestTime = GetTimerTickCount() - nStartTime;
	trace_printf("IN Inversion of %i matrix<5,5> = %i ticks\n", MATRIX_INVERSE_COUNT, nIN_InverseTestTime);


	// Make the test using CMatrix<T,m,n>
	//************************************
	nStartTime = GetTimerTickCount();

	CMatrix<float, 5,5>mIdentity, mm1;
	mm1 = s_fHilbertMAtrix5x5_data;
	mIdentity.InitWithIdentity();
	for (int i=0; i<MATRIX_INVERSE_COUNT; ++i) {
		CMatrix<float, 5,5> mmInverse = mm1.Inverse();
		CMatrix<float, 5,5> mmCheck = mm1 * mmInverse;
	}

	uint64_t nMM_InverseTestTime = GetTimerTickCount() - nStartTime;
	trace_printf("CMatrix Inversion of %i matrix<5,5> = %i ticks\n", MATRIX_INVERSE_COUNT, nMM_InverseTestTime);

	return bRes;
}

bool CMyTask::doMatrixTest3() {
	bool bRes = true;
	uint64_t nStartTime;

	// Load the matrix data.
//	CMatrix<float, 7,7> m1, identity, mInverse, mCheck;
	CMatrixF32<7,7> m1, identity, mInverse, mCheck, mClone;
	m1 = s3_m7x7_data;
	identity.InitWithIdentity();

	// Make MATRIX_INVERSE_COUNT inversion using the CMatrixF32 class
	//***************************************************************

	// Start the test
	nStartTime = GetTimerTickCount();

	for (int i=0; i<MATRIX_INVERSE_COUNT; ++i) {
		mClone = m1;
		mInverse = mClone.Inverse();
//		mCheck = m1 * mInverse;
	}

	uint64_t nInverseTestTime = GetTimerTickCount() - nStartTime;
	trace_printf("Inversion of %i matrix<7,7> = %i ticks\n", MATRIX_INVERSE_COUNT, nInverseTestTime);


	// Make the test using iNEMO_math
	//*******************************

	iNEMO_fMATRIX_TYPE *in_m1, *in_Inverse, *in_Check;
	in_m1 = iNEMO_fMatCreate(7, 7);
	for (int i=0; i<7; i++)
		for (int j=0; j<7; j++)
			iNEMO_MatData(in_m1)[i][j] = s3_m7x7_data[(i*7)+j];
	in_Inverse = iNEMO_fMatCreate(7, 7);
	in_Check = iNEMO_fMatCreate(7, 7);

	nStartTime = GetTimerTickCount();

	for (int i=0; i<MATRIX_INVERSE_COUNT; ++i) {
		iNEMO_fMatInv(in_m1, in_Inverse);
//		iNEMO_fMatMulMat(in_m1, in_Inverse, in_Check);
	}

	uint64_t nIN_InverseTestTime = GetTimerTickCount() - nStartTime;
	trace_printf("IN Inversion of %i matrix<7,7> = %i ticks\n", MATRIX_INVERSE_COUNT, nIN_InverseTestTime);

	return bRes;
}


/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
extern "C"
void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(s_pCurrentTimerHandle);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
extern "C"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	TIM_HandleTypeDefEx *phtim_ex = (TIM_HandleTypeDefEx*)htim;
	phtim_ex->nTimerTickCount++;

//	BSP_LED_Toggle(LED2);

}

extern "C"
void HAL_TIMEx_BreakCallback(TIM_HandleTypeDef *htim) {
	(void) htim;
};

extern "C"
void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim) {
	(void) htim;
};


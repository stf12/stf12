/**
 * @file   CGuardIncrementTask.h
 *
 * This test shows how to use the CSharedResource template class in order to access a
 * shared resource in a task safe way.
 *
 * @date   Aug 23, 2010
 * @author Stefano Oliveri
 *         E-mail: software@stf12.net
 */

#ifndef CGUARDTESTTASK_H_
#define CGUARDTESTTASK_H_

#include "AManagedTask.h"
#include "CSharedResource.h"
#include "CMutex.h"


class CResource {
public:
	int m_nValue;

public:
	CResource() { m_nValue = 0; }

};

class CSharedResourceEditor {
public:

	static void StartGuardTestClass();

};

//-----------------------------------------------

class CGuardIncrementTask: public AManagedTask {
	friend class CSharedResourceEditor;

	int m_nFrequecy;

	CSharedResource<CResource, CMutex> *m_pSharedResource;
	CSharedResource<int, CMutex> *m_pBasicSharedResource;
public:
	CGuardIncrementTask();
	virtual ~CGuardIncrementTask();

	void Run();
};

//-----------------------------------------------

class CGuardDecrementTask: public AManagedTask {
	friend class CSharedResourceEditor;

	int m_nFrequecy;

	CSharedResource<CResource, CMutex> *m_pGuardedResource;
	CSharedResource<int, CMutex> *m_pBasicSharedResource;
public:
	CGuardDecrementTask();
	virtual ~CGuardDecrementTask();

	void Run();
};

#endif /* CGUARDTESTTASK_H_ */

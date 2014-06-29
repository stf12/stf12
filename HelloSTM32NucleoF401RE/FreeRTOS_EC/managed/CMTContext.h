/**
 * @file CMTContext.h
 * @class CMTContext
 *
 * A managed context is used to group a set o managed task...
 *
 * @date June 3, 2014
 * @author Stefano Oliveri
 *         E-Mail: software@stf12.net
 */


#ifndef CMTCONTEXT_H_
#define CMTCONTEXT_H_

namespace freertosec {
namespace managed {

class AManagedTask;
class IManagedApplicationDelegate;

class CMTContext {
	friend class AManagedTask;
	friend class CManagedFreeRTOSApp;

private:
	/**
	 * Specifies the active managed task context. It could be NULL
	 */
	static CMTContext *s_pActiveContext;

	/**
	 * Specifies the application delegate.
	 */
	IManagedApplicationDelegate *m_pDelegate;

	/**
	 * Specifies the head of the linked list used to group all application managed task
	 * belonging to this context.
	 */
	AManagedTask *m_pMTListHead;

public:
	CMTContext();
	CMTContext(IManagedApplicationDelegate *pApplicationDelegate);
	virtual ~CMTContext();

	/**
	 * Make a context the active context. When a managed task is created
	 * using the default constructor, it is automatically added to the active context.
	 */
	void Activate();

private:
	/**
	 * Add a task to the context. If the task already belong to the context
	 * it is not added twice.
	 *
	 * @param pTaskToAdd specified the task to be added.
	 */
	void Add(AManagedTask *pTaskToAdd);

	/**
	 * Remove a task from the context.
	 *
	 * @param pTaskToRemove specifies the task to be removed.
	 */
	void Remove(AManagedTask *pTaskToRemove);

	/**
	 * Check if a task belong to the context.
	 *
	 * @param pTaskToAdd specifies the task to check.
	 * @return <code>true</code> if the task belong to the context, <code>false</code> otherwise.
	 */
	bool Find(AManagedTask *pTaskToCheck);

	/**
	 * Call the HardwerInit method for all managed task belonging to this context.
	 *
	 * @return <code>true</code> if success, <code>false</code> otherwise.
	 */
	bool DoHardwareInitStep();

	/**
	 * Get the managed application delegate. The delegate object is used process the standard RTOS
	 * callback:
	 * - Idle Hook
	 * - Tick Hook
	 * - Malloc Failed Hook
	 * - Stack Overflow Hook
	 *
	 * \sa <a href="http://www.freertos.org/a00016.html">Hook Functions</a> FreeRTOS documentation.
	 *
	 * @return the application delegate, or <code>NULL</code>.
	 */
	IManagedApplicationDelegate *GetApplicationDelegate();

	/**
	 * Get the active context.
	 *
	 * @return the active task context, or <code>NULL</code> if the application
	 * has not set a default task context.
	 */
	static CMTContext *GetActiveContext();

};

} /* namespace managed */
} /* namespace freertosec */

#endif /* CMTCONTEXT_H_ */

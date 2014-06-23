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

class CMTContext {
	friend class AManagedTask;
	friend class CManagedFreeRTOSApp;

private:
	/**
	 * Specifies the default managed task context. It could be NULL
	 */
	static CMTContext *s_pDefaultContext;

	/**
	 * Specifies the head of the linked list used to group all application managed task
	 * belonging to this context.
	 */
	AManagedTask *m_pMTListHead;

public:
	CMTContext();
	virtual ~CMTContext();

	/**
	 * Make a context the default context. When a managed task is created
	 * using the default constructor, it is automatically added to the default context.
	 */
	void SetDefault();


private:
	void Add(AManagedTask *pTaskToAdd);
	void Remove(AManagedTask *pTaskToRemove);

	/**
	 * Call the HardwerInit method for all managed task belonging to this context.
	 *
	 * @return <code>true</code> if success, <code>false</code> otherwise.
	 */
public: bool DoHardwareInitStep();

	/**
	 * Get the default context.
	 *
	 * @return the default task context, or <code>NULL</code> if the application
	 * has not set a default task context.
	 */

private:
static CMTContext *GetDefaultContext();

};

} /* namespace managed */
} /* namespace freertosec */

#endif /* CMTCONTEXT_H_ */

/*
 * CManagedFreeRTOSApp.h
 *
 *  Created on: Jun 20, 2014
 *      Author: stefano
 */

#ifndef CMANAGEDFREERTOSAPP_H_
#define CMANAGEDFREERTOSAPP_H_

namespace freertosec {
namespace managed {

class CMTContext;

class CManagedFreeRTOSApp {
public:
	CManagedFreeRTOSApp();
	virtual ~CManagedFreeRTOSApp();

	/**
	 * This method must be used to start an application that use the managed tasks.
	 * It performs the final start sequence:
	 * - Call the HardwareInit callback for all managed task before the scheduler starts.
	 * - Start the scheduler.
	 *
	 * @param pApplicationContext specifies the application contest used by the managed tasks.
	 *        It must not be NULL. The application must instantiate an CMTContext object,
	 *        and use it for the managed task creation, or set it as the default context.
	 * @return true if success. false otherwise.
	 */
	static void StartManagedApplication(CMTContext *pApplicationContext);

};

} /* namespace managed */
} /* namespace freertosec */

#endif /* CMANAGEDFREERTOSAPP_H_ */

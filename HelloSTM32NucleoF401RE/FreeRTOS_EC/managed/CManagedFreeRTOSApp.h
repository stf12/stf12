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
private:
	CMTContext *m_pApplicationContext;

	CManagedFreeRTOSApp(CMTContext *pApplicationContext);
public:
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

	void OnIdle();
	void OnTick();
	void OnMallocFailed();
	void OnStackOverflow(TaskHandle_t xTask, char *pcTaskName );

};

} /* namespace managed */
} /* namespace freertosec */

#endif /* CMANAGEDFREERTOSAPP_H_ */

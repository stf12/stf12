/**
 * @file IFreeRTOSProtocol.h
 * @class IFreeRTOSObj
 * @ingroup FreeRTOS_Wrapper
 *
 * This interface provide the basic behavior for all class in the FreeRTOS Wrapper layer. All objects in this layer
 * must be able to manage the owned native FreeRTOS handle. When the handle is created  it is attached to the object.
 * From this moment the object owns the handle. When the object is disposed the handle is deleted and its resources
 * are freed. It is possible to keep the underling native FreeRTOS handle alive by detaching it before the owner
 * object is disposed.
 *
 *
 * @date Aug 5, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef IFREERTOSPROTOCOL_H_
#define IFREERTOSPROTOCOL_H_

namespace freertosec {
namespace wrapper {

typedef void *GenericHandle_t;

class IFreeRTOSObj {
public:
	virtual ~IFreeRTOSObj() {}

	/**
	 * Check if the object is attached to a valid FreRTOS native handle.
	 *
	 * @return true if the is attached to a valid FreRTOS native handle, false otherwise.
	 */
	virtual bool IsValid() const =0;

	/**
	 * Attach the object to a given FreeRTOS handle. A previous attached handle is deleted and its
	 * resource are freed.
	 *
	 * @param handle specifies a native FreeRTOS handle.
	 */
	virtual void Attach(GenericHandle_t handle) =0;

	/**
	 * Usually a FreeRTOS handle is disposed and his resource freed when the attached object is deleted. Call this method
	 * to keep the handle alive.
	 *
	 * @return the native FreeRTOS handle. This handle is valid after the object is disposed.
	 */
	virtual GenericHandle_t Detach() =0;
};

} /* namespace wrapper */
} /* namespace freertosec */


#endif /* IFREERTOSPROTOCOL_H_ */

/*
 * IQueueSetMember.h
 *
 *  Created on: Jun 11, 2014
 *      Author: stefano
 */

#ifndef IQUEUESETMEMBER_H_
#define IQUEUESETMEMBER_H_

#include "queue.h"

namespace freertosec {
namespace wrapper {

class IQueueSetMember {
public:
	virtual ~IQueueSetMember() {};

	virtual BaseType_t Read(TickType_t xTicksToWait, void * const pvBuffer=NULL) =0;
	virtual operator QueueSetMemberHandle_t() const =0;
};

} /* namespace wrapper */
} /* namespace freertosec */


#endif /* IQUEUESETMEMBER_H_ */

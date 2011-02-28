/*
 * IMessageTaskDelegate.h
 *
 *  Created on: Oct 7, 2010
 *      Author: oliveris
 */

#ifndef IMESSAGETASKDELEGATE_H_
#define IMESSAGETASKDELEGATE_H_


class IMessageTaskDelegate {
public:
	virtual void OnHandleEvent(const CMessage &msg) =0;
	virtual void DidHandleEvent(const CMessage &msg) =0;
};

#endif /* IDISPLAYDELEGATE_H_ */

/*
 * Message.h
 *
 *  Created on: Jan 10, 2011
 *      Author: oliveris
 */

#ifndef MESSAGEMACRO_H_
#define MESSAGEMACRO_H_


#include "MessageMacro.h"

/**
 * @class CMessage
 */
class CMessage {
public:
	unsigned short m_nId;
	unsigned long wParam;
};

/**
 *
 */
#define DECLARE_MESSAGE_MAP(className) \
	typedef struct {\
		unsigned short id;\
		void (className::*MessageHandler)(unsigned long wParam);\
	} message_map_entry_t;\
	public:virtual void DispatchMessage(const CMessage &msg);\
	private:static const message_map_entry_t s_message_map[];

/**
 *
 */
#define BEGIN_MESSAGE_MAP(baseClass, className) \
	void className::DispatchMessage(const CMessage &msg) {\
		for (int i=0; s_message_map[i].id!=NULL_MSG; ++i) {\
			if (s_message_map[i].id == msg.m_nId) {\
				(this->*s_message_map[i].MessageHandler)(msg.wParam);\
				return;\
			}\
		}\
		return baseClass::DispatchMessage(msg);\
	}\
	const className::message_map_entry_t className::s_message_map[] = {

#define MESSAGE_MAP_ENTRY(className, messageId, messageHandler) \
	{ messageId, &className::messageHandler },

/**
 *
 */
#define END_MESSAGE_MAP(className) \
	{ NULL_MSG, NULL } \
	};


#endif /* MESSAGEMACRO_H_ */

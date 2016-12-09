#pragma once

#include "SharedMemory.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>


using namespace std;
using namespace boost::interprocess;


class MessageQueue
{
	static MessageQueue *s_instance;

public:
	static MessageQueue *instance();

	HRESULT MQOpen(const char *qname, int maxQSize = 5, int maxQMsg = 64);
	HRESULT MQClose(const char *qname);
	HRESULT MQSend(const char *qname, const char *msg);
	HRESULT MQRecv(const char *qname, char **msg);

private:
	MessageQueue();
	~MessageQueue();
};

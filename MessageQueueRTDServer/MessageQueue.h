#pragma once

#include "SharedMemory.h"
#include <iostream>


using namespace std;
using namespace boost::interprocess;


class MessageQueue
{
	static MessageQueue *s_instance;

public:
	static MessageQueue *instance();

	HRESULT MQOpen(const char *qname);
	HRESULT MQClose(const char *qname);
	HRESULT MQSend(const char *qname, const char *msg);
	HRESULT MQRecv(const char *qname, char **msg);

private:
	MessageQueue();
	~MessageQueue();
};

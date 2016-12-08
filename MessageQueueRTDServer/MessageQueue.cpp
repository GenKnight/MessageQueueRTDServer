#include "stdafx.h"
#include "MessageQueue.h"


MessageQueue *MessageQueue::s_instance = 0;

MessageQueue::MessageQueue()
{
}

MessageQueue::~MessageQueue()
{
}

MessageQueue * MessageQueue::instance()
{
	if (!s_instance)
	{
		s_instance = new MessageQueue();
	}
	return s_instance;
}

HRESULT MessageQueue::MQOpen(const char *qname, int maxQSize, int maxQMsg)
{
	try 
	{
		message_queue mq(open_or_create, qname, maxQSize, maxQMsg);
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return S_OK;
}

HRESULT MessageQueue::MQClose(const char *qname)
{
	try 
	{
		message_queue::remove(qname);
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return S_OK;
}

HRESULT MessageQueue::MQSend(const char *qname, const char *msg)
{
	
	try
	{
		message_queue mq(open_only, qname);
		mq.send(msg, strlen(msg), 0);

		cout << "Sent: '" << msg << "'" << endl;
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return S_OK;
}

HRESULT MessageQueue::MQRecv(const char *qname, char **msg)
{
	try
	{
		message_queue mq(open_only, qname);
		char *m = new char[64];
		unsigned int priority;
		size_t recvd_size;
		if (mq.try_receive(m, strlen(m), recvd_size, priority))
		{
			m[recvd_size] = '\0';
			m = (char *) realloc(m, recvd_size + 1);
			*msg = m;

			cout << "Reveived: '" << *msg << "'" << endl;
		}
		else 
		{
			*msg = "no message";
		}
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return S_OK;
}

bool MessageQueue::MQMessageExists(const char * qname)
{
	try
	{
		message_queue mq(open_only, qname);
		//return mq.get_num_msg() > 0;
		return true;
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}
	return false;
}

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

		SharedMemory::instance()->createSharedMemory(qname);
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
		
		message_notification * notification = SharedMemory::instance()->getSharedMemory(qname)->notification;
		if (notification != nullptr) {
			scoped_lock<interprocess_mutex> lock(notification->mutex);
			notification->close = true;
			notification->cond_msg_exists.notify_all();
		}
		
		SharedMemory::instance()->destroySharedMemory(qname);
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

		message_notification * notification = SharedMemory::instance()->getSharedMemory(qname)->notification;
		if (notification != nullptr) {
			notification->cond_msg_exists.notify_one();
		}
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

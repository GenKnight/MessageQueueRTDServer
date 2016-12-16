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

HRESULT MessageQueue::MQOpen(const char *qname)
{
	try 
	{
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
		ipc_message_struct * ipc_message = SharedMemory::instance()->getSharedMemory(qname)->ipc_message;
		if (ipc_message != nullptr)
		{
			scoped_lock<interprocess_mutex> lock(ipc_message->mutex);
			ipc_message->close = true;
			ipc_message->cond_msg_exists.notify_all();
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
		ipc_message_struct * ipc_message = SharedMemory::instance()->getSharedMemory(qname)->ipc_message;
		if (ipc_message != nullptr)
		{
			string new_msg = string(msg);
			if (new_msg.length() > IPC_MSG_MAX_SIZE) {
				new_msg = new_msg.substr(0, IPC_MSG_MAX_SIZE);
			}
			string curr_msg = ipc_message->message;
			if (new_msg.compare(curr_msg) != 0)
			{
				scoped_lock<interprocess_mutex> lock(ipc_message->mutex);
				ipc_message->message = new_msg;
				ipc_message->cond_msg_exists.notify_one();
			}
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
		ipc_message_struct * ipc_message = SharedMemory::instance()->getSharedMemory(qname)->ipc_message;
		if (ipc_message != nullptr)
		{
			*msg = _strdup(ipc_message->message.c_str());
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

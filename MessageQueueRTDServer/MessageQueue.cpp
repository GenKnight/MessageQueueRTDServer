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
		s_instance = new MessageQueue;
	return s_instance;
}

HRESULT MessageQueue::MQOpen(const char *qname, int maxQSize, int maxQMsg)
{
	try {
		message_queue mq(open_or_create, qname, 10, 32);
	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return E_ABORT;
	}

	return S_OK;
}

HRESULT MessageQueue::MQClose(const char *qname)
{
	try {
		message_queue::remove(qname);
	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return E_ABORT;
	}

	return S_OK;
}

HRESULT MessageQueue::MQSend(const char *qname, const char *msg)
{
	
	try {
		message_queue mq(open_only, qname);
		mq.send(msg, strlen(msg), 0);
		std::cout << "Sent: '" << msg << "'" << std::endl;
	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return E_ABORT;
	}

	return S_OK;
}

HRESULT MessageQueue::MQRecv(const char *qname, char **msg)
{
	try {
		message_queue mq(open_only, qname);
		char *m = new char[64];
		unsigned int priority;
		size_t recvd_size;
		if (mq.try_receive(m, strlen(m), recvd_size, priority)) {
			m[recvd_size] = '\0';
			m = (char *) realloc(m, recvd_size + 1);
			*msg = m;
			std::cout << "Reveived: '" << *msg << "'" << std::endl;
		}
		else {
			*msg = "no message";
		}
	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return E_ABORT;
	}

	return S_OK;
}

Subject::Subject(Event evt)
{
	this->evt = evt;
}

HRESULT Subject::attach(Observer * obs)
{
	observers.push_back(obs);
	return S_OK;
}

HRESULT Subject::detach(Observer * obs)
{
	std::vector<class Observer *>::iterator it = std::find(observers.begin(), observers.end(), obs);
	if (it != observers.end()) {
		observers.erase(it);
	}
	return S_OK;
}

HRESULT Subject::detachAll()
{
	observers.clear();
	return S_OK;
}

HRESULT Subject::notify()
{
	for (vector<Observer *>::const_iterator it = observers.begin(); it != observers.end(); ++it)
	{
		if (it != observers.end()) {
			//(*it)->update();
		}
	}
	return S_OK;
}

Event Subject::getEvent()
{
	return evt;
}


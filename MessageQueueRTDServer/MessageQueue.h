#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <algorithm>
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

enum Event {OPEN, CLOSE, SEND, RECV};

class Subject {

	Event evt;
	vector<class Observer *> observers;

public:
	Subject(Event evt);

	HRESULT attach(Observer *obs);
	HRESULT detach(Observer *obs);
	HRESULT detachAll();
	Event getEvent();
	virtual HRESULT notify() = 0;

};


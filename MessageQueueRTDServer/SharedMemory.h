#pragma once

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <string>


using namespace std;
using namespace boost::interprocess;


struct message_notification
{
	message_notification() : close(false)
	{}

	interprocess_mutex      mutex;
	interprocess_condition  cond_msg_exists;

	bool close;
};

struct shared_memory
{
	shared_memory(string n) {
		name = n + "_shm_";
	};

	string name;
	shared_memory_object sharedMemoryObj;
	mapped_region mappedRegion;
	void * addr;

	message_notification * notification;
};


class SharedMemory
{
	static SharedMemory * s_instance;

public:
	static SharedMemory * instance();

	shared_memory * createSharedMemory(const char * name);
	shared_memory * getSharedMemory(const char * name);
	HRESULT destroySharedMemory(const char * name);

private:
	SharedMemory();
	~SharedMemory();
	
};


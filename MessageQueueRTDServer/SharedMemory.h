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

const int IPC_MSG_MAX_SIZE = 15;


struct ipc_message_struct
{
	ipc_message_struct() : close(false)
	{}

	interprocess_mutex      mutex;
	interprocess_condition  cond_msg_exists;

	string message;

	bool close;
};

struct shared_memory_struct
{
	shared_memory_struct(string n) {
		name = n;
		if (name.length() > IPC_MSG_MAX_SIZE) {
			name = name.substr(0, IPC_MSG_MAX_SIZE);
		}
	};

	string name;
	shared_memory_object sharedMemoryObj;
	mapped_region mappedRegion;
	void * addr;

	ipc_message_struct * ipc_message;
};


class SharedMemory
{
	static SharedMemory * s_instance;

public:
	static SharedMemory * instance();

	shared_memory_struct * createSharedMemory(const char * name);
	shared_memory_struct * getSharedMemory(const char * name);
	HRESULT destroySharedMemory(const char * name);

private:
	SharedMemory();
	~SharedMemory();
	
};


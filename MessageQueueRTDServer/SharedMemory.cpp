#include "stdafx.h"
#include "SharedMemory.h"


SharedMemory * SharedMemory::s_instance = 0;

SharedMemory * SharedMemory::instance()
{
	if (!s_instance)
	{
		s_instance = new SharedMemory();
	}
	return s_instance;
}

shared_memory * SharedMemory::createSharedMemory(const char * name)
{
	shared_memory * sharedMem = new shared_memory(name);

	try {
		shared_memory_object::remove(sharedMem->name.c_str());
		sharedMem->sharedMemoryObj = shared_memory_object(create_only, sharedMem->name.c_str(), read_write);
		sharedMem->sharedMemoryObj.truncate(sizeof(message_notification));
		sharedMem->mappedRegion = mapped_region(sharedMem->sharedMemoryObj, read_write);
		sharedMem->addr = sharedMem->mappedRegion.get_address();
		sharedMem->notification = new (sharedMem->addr) message_notification;
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return sharedMem;
}

shared_memory * SharedMemory::getSharedMemory(const char * name)
{
	shared_memory * sharedMem = new shared_memory(name);
	
	try 
	{
		sharedMem->sharedMemoryObj = shared_memory_object(open_only, sharedMem->name.c_str(), read_write);
		sharedMem->mappedRegion = mapped_region(sharedMem->sharedMemoryObj, read_write);
		sharedMem->addr = sharedMem->mappedRegion.get_address();
		sharedMem->notification = static_cast<message_notification *>(sharedMem->addr);
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}
	
	return sharedMem;
}

HRESULT SharedMemory::destroySharedMemory(const char * name)
{
	shared_memory * sharedMem = new shared_memory(name);

	try {
		shared_memory_object::remove(sharedMem->name.c_str());
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}
	return S_OK;
}

SharedMemory::SharedMemory()
{
}

SharedMemory::~SharedMemory()
{
}

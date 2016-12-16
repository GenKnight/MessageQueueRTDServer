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

shared_memory_struct * SharedMemory::createSharedMemory(const char * name)
{
	shared_memory_struct * sharedMem = new shared_memory_struct(name);

	try {
		shared_memory_object::remove(sharedMem->name.c_str());
		sharedMem->sharedMemoryObj = shared_memory_object(create_only, sharedMem->name.c_str(), read_write);
		sharedMem->sharedMemoryObj.truncate(sizeof(ipc_message_struct));
		sharedMem->mappedRegion = mapped_region(sharedMem->sharedMemoryObj, read_write);
		sharedMem->addr = sharedMem->mappedRegion.get_address();
		sharedMem->ipc_message = new (sharedMem->addr) ipc_message_struct;
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return sharedMem;
}

shared_memory_struct * SharedMemory::getSharedMemory(const char * name)
{
	shared_memory_struct * sharedMem = new shared_memory_struct(name);
	
	try 
	{
		sharedMem->sharedMemoryObj = shared_memory_object(open_only, sharedMem->name.c_str(), read_write);
		sharedMem->mappedRegion = mapped_region(sharedMem->sharedMemoryObj, read_write);
		sharedMem->addr = sharedMem->mappedRegion.get_address();
		sharedMem->ipc_message = static_cast<ipc_message_struct *>(sharedMem->addr);
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}
	
	return sharedMem;
}

HRESULT SharedMemory::destroySharedMemory(const char * name)
{
	shared_memory_struct * sharedMem = new shared_memory_struct(name);

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

#pragma once

#include "SharedMemory.h"
#include <vector>
#include <string>

using namespace std;

class Topic
{
public:
	long topicId;
	vector<string> strings;

	Topic(long id, vector<string> strings);
	~Topic();

	string getUniqueName();

private:
	string uName;
};


#include "stdafx.h"
#include "Topic.h"


Topic::Topic(long id, vector<string> strings)
{
	this->topicId = id;
	this->strings = strings;
}


Topic::~Topic()
{
}

string Topic::getUniqueName()
{
	if (uName.empty()) 
	{
		uName = "";
		for (auto str : this->strings) 
		{
			if (strlen(str.c_str()) == 0 || str.empty())
			{
				continue;
			} else
			{
				uName += str + "_";
			}
		}
		if (uName.back() == '_') 
		{
			uName = uName.substr(0, uName.size() - 1);
		}
		if (uName.length() > IPC_MSG_MAX_SIZE) {
			uName = uName.substr(0, IPC_MSG_MAX_SIZE);
		}
	}
	return uName;
}

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
			uName += str + "_";
		}
		if (uName.back() == '_') 
		{
			uName = uName.substr(0, uName.size() - 1);
		}
	}
	return uName;
}

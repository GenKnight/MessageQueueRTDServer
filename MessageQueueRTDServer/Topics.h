#pragma once

#include <map>
#include "Topic.h"

using namespace std;

class Topics
{
	static Topics *s_instance;

public:
	static Topics *instance();

	HRESULT createTopic(long topicId, SAFEARRAY **safearray);
	Topic *getTopic(long topicId);
	HRESULT removeTopic(long topicId);
	HRESULT removeTopics();
	vector<long> getTopicIds();
	size_t getTopicCount();

private:
	Topics();
	~Topics();

	map<long, Topic> m_map_topics;
};


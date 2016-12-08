#include "stdafx.h"
#include "Topics.h"


Topics *Topics::s_instance = 0;


Topics::Topics()
{
}


Topics::~Topics()
{
}

Topics * Topics::instance()
{
	if (!s_instance)
		s_instance = new Topics;
	return s_instance;
}

HRESULT Topics::createTopic(long topicId, SAFEARRAY **safearray)
{
	VARIANT * raw;
	HR(::SafeArrayAccessData(*safearray, (void **)&raw));

	const ULONG numOfElements = (*safearray)->rgsabound[0].cElements;

	vector<string> strings;

	for (ULONG i = 0; i < numOfElements; ++i)
	{
		const wchar_t* wstrVal = (const wchar_t*) raw[i].bstrVal;
		wstring ws(wstrVal);
		string str(ws.begin(), ws.end());
		strings.push_back(str);
	}

	Topic topic(topicId, strings);

	m_map_topics.insert(make_pair(topicId, topic));

	HR(::SafeArrayUnaccessData(*safearray));

	return S_OK;
}

Topic* Topics::getTopic(long topicId)
{
	map<long, Topic>::iterator it = m_map_topics.find(topicId);
	if (it != m_map_topics.end()) 
	{
		return &(it->second);
	}
	return NULL;
}

HRESULT Topics::removeTopic(long topicId)
{
	map<long, Topic>::iterator it = m_map_topics.find(topicId);
	if (it != m_map_topics.end()) 
	{
		m_map_topics.erase(it);
	}
	return S_OK;
}

HRESULT Topics::removeTopics()
{
	m_map_topics.clear();
	return S_OK;
}

vector<long> Topics::getTopicIds()
{
	vector<long> v;
	for (map<long, Topic>::iterator it = m_map_topics.begin(); it != m_map_topics.end(); ++it) 
	{
		v.push_back(it->first);
	}
	return v;
}

vector<string> Topics::getTopicNames()
{
	vector<string> v;
	for (map<long, Topic>::iterator it = m_map_topics.begin(); it != m_map_topics.end(); ++it) 
	{
		Topic t = it->second;
		v.push_back(t.getUniqueName());
	}
	return v;
}

size_t Topics::getTopicCount()
{
	return m_map_topics.size();
}

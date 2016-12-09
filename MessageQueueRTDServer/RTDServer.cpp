// RTDServer.cpp : Implementation of CSimpleRTDServer

#include "stdafx.h"
#include "RTDServer.h"


// CSimpleRTDServer

HRESULT CRTDServer::ServerStart(IRTDUpdateEvent *callback, long *pfRes)
{

	if (0 == callback || 0 == pfRes)
	{
		return E_POINTER;
	}

	m_callback = callback;

	*pfRes = 1;

	return S_OK;
}

HRESULT CRTDServer::ConnectData(long topicID, SAFEARRAY **strings, VARIANT_BOOL *getNewValues, VARIANT *pvarOut)
{
	if (0 == strings || 0 == getNewValues || 0 == pvarOut)
	{
		return E_POINTER;
	}

	HR(Topics::instance()->createTopic(topicID, strings));
	Topic * topic = Topics::instance()->getTopic(topicID);

	if (topic != nullptr)
	{
		HR(MessageQueue::instance()->MQOpen(topic->getUniqueName().c_str()));

		Observer observer(topic, m_callback, &m_updatedTopics);
		thread obs_thread(boost::move(observer));
		obs_thread.detach();
	}
	
	return GetMsg(topicID, pvarOut);
}

HRESULT CRTDServer::RefreshData(long *topicCount, SAFEARRAY **parrayOut)
{
	if (0 == topicCount || 0 == parrayOut)
	{
		return E_POINTER;
	}

	long numOfNewValues = static_cast<long> (m_updatedTopics.size());

	CComSafeArrayBound bounds[2] =
	{
		CComSafeArrayBound(2),
		CComSafeArrayBound(numOfNewValues)
	};

	CComSafeArray<VARIANT> data(bounds, _countof(bounds));
	LONG indices[2];
	
	int i = 0;
	for (list<long>::iterator it = m_updatedTopics.begin(); it != m_updatedTopics.end(); it++, i++)
	{
		indices[0] = 0;
		indices[1] = i;

		long topicId = *it;
		HR(data.MultiDimSetAt(indices, CComVariant(topicId)));

		CComVariant value;
		HR(GetMsg(topicId, &value));

		indices[0] = 1;
		indices[1] = i;
		HR(data.MultiDimSetAt(indices, value));
	}
	
	m_updatedTopics.clear();

	*parrayOut = data.Detach();

	*topicCount = numOfNewValues;

	return S_OK;
}

HRESULT CRTDServer::DisconnectData(long topicID)
{
	HR(MessageQueue::instance()->MQClose(Topics::instance()->getTopic(topicID)->getUniqueName().c_str()));
	HR(Topics::instance()->removeTopic(topicID));

	return S_OK;
}

HRESULT CRTDServer::Heartbeat(long *pfRes)
{
	if (0 == pfRes)
	{
		return E_POINTER;
	}

	*pfRes = 1;

	return S_OK;
}

HRESULT CRTDServer::ServerTerminate()
{
	vector<long> topicIds = Topics::instance()->getTopicIds();
	for (auto topicId : topicIds)
	{
		MessageQueue::instance()->MQClose(Topics::instance()->getTopic(topicId)->getUniqueName().c_str());
	}
	HR(Topics::instance()->removeTopics());

	m_callback = nullptr;

	return S_OK;
}


HRESULT CRTDServer::GetMsg(long topicID, VARIANT *value)
{
	ASSERT(0 != value);

	char *msg;
	HR(MessageQueue::instance()->MQRecv(Topics::instance()->getTopic(topicID)->getUniqueName().c_str(), &msg));

	CComBSTR string(msg);
	value->vt = VT_BSTR;
	value->bstrVal = string.Detach();

	return S_OK;
}


// Observer

Observer::Observer(Topic * pTopic, IRTDUpdateEvent * pCallback, list<long>* pNewResults)
{
	m_pTopic = pTopic;
	m_callback = pCallback;
	m_pNewResults = pNewResults;
}

Observer::~Observer()
{
	m_callback = nullptr;
}

HRESULT Observer::SetCallback(IRTDUpdateEvent * callback)
{
	m_callback = callback;
	return S_OK;
}

HRESULT Observer::operator()()
{
	try
	{
		
		message_notification * notification = SharedMemory::instance()->getSharedMemory(m_pTopic->getUniqueName().c_str())->notification;
		if (notification != nullptr) {
			bool end_loop = false;
			do
			{
				scoped_lock<interprocess_mutex> lock(notification->mutex);
				notification->cond_msg_exists.wait(lock);

				if (notification->close)
				{
					end_loop = true;
				}
				else
				{
					m_pNewResults->push_front(m_pTopic->topicId);
					m_callback->UpdateNotify();
				}

			} while (!end_loop);
		}
		
	}
	catch (interprocess_exception &ex)
	{
		cout << ex.what() << endl;
	}

	return S_OK;
}

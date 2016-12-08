// RTDServer.cpp : Implementation of CSimpleRTDServer

#include "stdafx.h"
#include "RTDServer.h"


// TimerWindow

void TimerWindow::OnTimer(UINT_PTR /*timer*/)
{
	Stop();

	if (0 != m_callback)
	{
		vector<string> topicNames = Topics::instance()->getTopicNames();
		for (auto tName : topicNames)
		{
			if (MessageQueue::instance()->MQMessageExists(tName.c_str()))
			{
				m_callback->UpdateNotify();
				break;
			}
		}
		
	}
}

TimerWindow::TimerWindow()
{
	Create(0);
	ASSERT(0 != m_hWnd);
}

TimerWindow::~TimerWindow()
{
	VERIFY(DestroyWindow());
}

void TimerWindow::SetCallback(IRTDUpdateEvent *callback)
{
	m_callback = callback;
}

void TimerWindow::Start()
{
	SetTimer(0, 2000);
}

void TimerWindow::Stop()
{
	VERIFY(KillTimer(0));
}




// CSimpleRTDServer

HRESULT CRTDServer::ServerStart(IRTDUpdateEvent *callbackObject, long *pfRes)
{

	if (0 == callbackObject || 0 == pfRes)
	{
		return E_POINTER;
	}

	m_timer.SetCallback(callbackObject);

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
	HR(MessageQueue::instance()->MQOpen(Topics::instance()->getTopic(topicID)->getUniqueName().c_str()));


	if (Topics::instance()->getTopicCount() == 1)
	{
		m_timer.Start();
	}

	return GetMsg(topicID, pvarOut);
}

HRESULT CRTDServer::RefreshData(long *topicCount, SAFEARRAY **parrayOut)
{
	if (0 == topicCount || 0 == parrayOut)
	{
		return E_POINTER;
	}

	vector<long> topicIds = Topics::instance()->getTopicIds();
	long numOfTopics = static_cast<long> (topicIds.size());
	long numOfNewValues = static_cast<long> (m_updatedTopics.size());

	CComSafeArrayBound bounds[2] =
	{
		CComSafeArrayBound(2),
		CComSafeArrayBound(numOfTopics)
	};

	CComSafeArray<VARIANT> data(bounds, _countof(bounds));
	LONG indices[2];
	
	int i = 0;
	for (vector<long>::iterator it = topicIds.begin(); it != topicIds.end(); it++, i++)
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

	*topicCount = numOfTopics;

	m_timer.Start();

	return S_OK;
}

HRESULT CRTDServer::DisconnectData(long topicID)
{
	if (Topics::instance()->getTopicCount() <= 0)
	{
		m_timer.Stop();
	}

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
	m_timer.SetCallback(0);

	vector<long> topicIds = Topics::instance()->getTopicIds();
	for (auto topicId : topicIds)
	{
		MessageQueue::instance()->MQClose(Topics::instance()->getTopic(topicId)->getUniqueName().c_str());
	}
	HR(Topics::instance()->removeTopics());

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

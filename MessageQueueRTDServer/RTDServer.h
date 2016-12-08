// RTDServer.h : Declaration of the CSimpleRTDServer

#pragma once

#include "resource.h"       // main symbols
#include "MessageQueue.h"
#include "Topics.h"
#include <vector>
#include <list>
#include "RTDServer_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace std;
using namespace boost;


// TimerWindow

class TimerWindow : public CWindowImpl<TimerWindow, CWindow, CWinTraits<>>
{
private:
	CComPtr<IRTDUpdateEvent> m_callback;

	void OnTimer(UINT_PTR /*timer*/);

public:
	BEGIN_MSG_MAP(TimerWindow)
		MSG_WM_TIMER(OnTimer)
	END_MSG_MAP()

	TimerWindow();

	~TimerWindow();

	void SetCallback(IRTDUpdateEvent *callback);

	void Start();

	void Stop();

};

class CWorkerTask
{
private:
	long topicId;
	Topic * m_pTopic;
	IRTDUpdateEvent * m_pCallback;
	list<long> * m_pNewResults;

public:
	CWorkerTask(long topicId, Topic *pTopic, IRTDUpdateEvent * pCallback, list<long> * pNewResults);
	HRESULT operator()();

};



// CRTDServer

class ATL_NO_VTABLE CRTDServer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRTDServer, &CLSID_RTDServer>,
	public IDispatchImpl<IRTDServer, &IID_IRTDServer, &LIBID_RTDServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IRtdServer, &__uuidof(IRtdServer), &LIBID_Excel, /* wMajor = */ 1, /* wMinor = */ 8>
{
public:
	CRTDServer()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_RTDSERVER)

	BEGIN_COM_MAP(CRTDServer)
		COM_INTERFACE_ENTRY(IRTDServer)
		COM_INTERFACE_ENTRY2(IDispatch, IRtdServer)
		COM_INTERFACE_ENTRY(IRtdServer)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// IRtdServer Methods
public:
	STDMETHOD(ServerStart)(IRTDUpdateEvent *callbackObject, long *pfRes);
	STDMETHOD(ConnectData)(long topicID, SAFEARRAY **strings, VARIANT_BOOL *getNewValues, VARIANT *pvarOut);
	STDMETHOD(RefreshData)(long *topicCount, SAFEARRAY **parrayOut);
	STDMETHOD(DisconnectData)(long topicID);
	STDMETHOD(Heartbeat)(long *pfRes);
	STDMETHOD(ServerTerminate)();

	STDMETHOD(GetMsg)(long topicID, VARIANT *value);

private:
	TimerWindow m_timer;
	list<long> m_updatedTopics;

};

OBJECT_ENTRY_AUTO(__uuidof(RTDServer), CRTDServer)

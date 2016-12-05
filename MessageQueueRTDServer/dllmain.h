// dllmain.h : Declaration of module class.

class CRTDServerModule : public ATL::CAtlDllModuleT< CRTDServerModule >
{
public :
	DECLARE_LIBID(LIBID_RTDServerLib)
};

extern class CRTDServerModule _AtlModule;

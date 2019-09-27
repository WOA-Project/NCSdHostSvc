#include <Windows.h>
#include "Main.h"

typedef int(__stdcall* SvcMain)();
typedef TCHAR* (__stdcall* GetServiceName)();
typedef SvcMain (__stdcall* GetServiceMain)();

typedef struct _SVC_PLUGIN_DETAILS {
	GetServiceName GetServiceName;
	GetServiceMain GetServiceMain;
} SVC_PLUGIN_DETAILS, * PSVC_PLUGIN_DETAILS, ** PPSVC_PLUGIN_DETAILS;

typedef PPSVC_PLUGIN_DETAILS(__stdcall* GET_SVC)(__int64 a1);

int _tmain(int argc, TCHAR* argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[] =
	{
		{SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{NULL, NULL}
	};

	if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
	{
		return GetLastError();
	}

	return 0;
}


VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	HINSTANCE hDLL = LoadLibrary(L"NcsdPluginSvc.dll");

	if (!hDLL)
	{
		return;
	}

	GET_SVC get_svc = (GET_SVC)GetProcAddress(hDLL, "GET_SVC");
	if (!get_svc)
	{
		return;
	}

	SVC_PLUGIN_DETAILS svcdetails = *(*get_svc(0));
	SvcMain fsvcmain = svcdetails.GetServiceMain();
	fsvcmain(argc, argv);

	return;
}
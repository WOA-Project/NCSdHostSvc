#pragma once
#include <Windows.h>
#include <tchar.h>

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);

TCHAR SERVICE_NAME[] = _T("NcsdPluginSvc");
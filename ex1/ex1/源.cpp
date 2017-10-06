
#include <comdef.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include<fstream>
#include<string>
#include <vector>
#include<iostream>
#define MAX_LINE_LEN  80

int _tmain()
{
	/* Creat a new process to execute */
	_TCHAR cmdLine[MAX_LINE_LEN]; //= _TEXT("notepad");

	/* CreateProcess parameters */
	LPCTSTR lpApplicationName = NULL;
	LPSECURITY_ATTRIBUTES processSA = NULL;		/* Default */
	LPSECURITY_ATTRIBUTES threadSA = NULL;		/* Default */
	BOOL bInheritHandles = TRUE;				/* Default ,shareRights*/
	DWORD dwCreationFlags = CREATE_NEW_CONSOLE;	/* creationMask,Window per process */
	LPVOID lpEnvironment = NULL;				/* Default */
	LPTSTR lpCurrentDirectory = NULL;			/* Default */

	STARTUPINFO lpStartupInfo;					/* Result */
	PROCESS_INFORMATION lpProcessInformation;	/* Result */

	ZeroMemory(&lpStartupInfo, sizeof(lpStartupInfo));
	lpStartupInfo.cb = sizeof(lpStartupInfo);

	/* 1.Read the command line parameters */


	std::vector<TCHAR*>applications;

	int des;
	std::cout << "选择你想要的地址读取方式 ，1为内置，2为打开batch";
	std::cin >> des;
	if(des==1)
	{
		applications.push_back( L"notepad.exe");
		applications.push_back(L"calc.exe");
		applications.push_back(L"write.exe");
	}else if(des==2)
	{
		std::ifstream ifs;
		
		ifs.open("batch");
		if(ifs.is_open())
		{
			char add[MAX_LINE_LEN];
			wchar_t trans[MAX_LINE_LEN];
			while(ifs.getline(add, MAX_LINE_LEN))
			{
				wchar_t* toSave = new wchar_t[MAX_LINE_LEN];
				mbstowcs(trans, add, MAX_LINE_LEN);
				memcpy(toSave, trans, MAX_LINE_LEN);
				applications.push_back(toSave);
				
			}
			

		
		}
		
	}
	

	for (int i = 0; i < applications.size(); i++) {

		/* 2.copy the command from command line parameters */
		wcscpy_s(cmdLine, MAX_LINE_LEN, applications[i]);


		/* Create a new Procwss */

		if (!CreateProcess(
			lpApplicationName,		/* File name of executable */
			(LPTSTR)cmdLine,		/* Command line   cmdLine */
			processSA,				/* Process inherited security */
			threadSA,				/* Thread inherited security */
			bInheritHandles,		/* Right propagation */
			dwCreationFlags,		/* Various creation flags */
			lpEnvironment,			/* Use parent's environment block */
			lpCurrentDirectory,		/* Use parent's starting directory */
			&lpStartupInfo,			/* Pointer to STARTUPINFO structure */
			&lpProcessInformation	/* Pointer to PROCESS_INFORMATION structure */
		)
			) {
			fprintf(stderr, "CreatProcess failed on error %d\n", GetLastError());
			ExitProcess(1);
		};
		fprintf(stdout, "The Child Process's PID: %d.\n", lpProcessInformation.dwProcessId);
		fprintf(stdout, "The Parent Process finish.\n");
		Sleep(500);
		CloseHandle(lpProcessInformation.hProcess);
		CloseHandle(lpProcessInformation.hThread);
	};
	Sleep(5000);
	return 0;
}


{% raw %}
/*
ARCH: Windows x86
This example attempts to query the process hirearchy from which the existing process is part of
If parent doesn't match any inside a whitelist, the test fails
The test may be improved watching for child processes that were not created by the existing code
*/

#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <string>
#include <algorithm>
#include <TlHelp32.h>
#include <map>
using namespace std;

#pragma warning (disable:4996)

typedef struct _ALK_PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    void* PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} ALK_PROCESS_BASIC_INFORMATION;

typedef NTSTATUS(* __stdcall pNtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG);

DWORD GetExplorerPIDbyShellWindow()
{
    DWORD dwProcessId = 0;

    // Get the PID of explorer by its windows handle
    GetWindowThreadProcessId(GetShellWindow(), &dwProcessId);

    return dwProcessId;
}

DWORD GetParentProcessId()
{
    auto NtQueryInfoProcess = (pNtQueryInformationProcess)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");

    NTSTATUS Status = 0;
    ALK_PROCESS_BASIC_INFORMATION pbi;
    SecureZeroMemory(&pbi, sizeof(ALK_PROCESS_BASIC_INFORMATION));

    const UINT ProcessBasicInformation = 0;

    Status = NtQueryInfoProcess(GetCurrentProcess(), ProcessBasicInformation, (PVOID)&pbi, sizeof(ALK_PROCESS_BASIC_INFORMATION), 0);

    if (Status)
        return 0;
    return (DWORD)pbi.ParentProcessId;
}

void IsParentExplorerExe(string& addition)
{
    CHAR terminals[6][20] = { "svchost.exe", "cmd.exe", "conhost.exe", "explorer.exe", "winlogon.exe", "services.exe" };
    DWORD parentPid = GetParentProcessId();

    if (parentPid)
    {
        // first check 
        HANDLE hParent = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, parentPid);
        if (hParent != INVALID_HANDLE_VALUE)
        {
            CHAR winDir[MAX_PATH];
            CHAR parentProcessPath[MAX_PATH];
            if (GetModuleFileNameEx(hParent, NULL, parentProcessPath, MAX_PATH) && GetWindowsDirectory(winDir, MAX_PATH))
            {
                CHAR* parentFname = PathFindFileName(parentProcessPath);                
                string lowparent = parentFname;
                transform(lowparent.begin(), lowparent.end(), lowparent.begin(), tolower);
                bool matchfnm = false;
                for (int i = 0; i < 6; i++)
                {                    
                    if (!strcmp(lowparent.c_str(), terminals[i]))
                    {
                        matchfnm = true;
                        break;
                    }
                }
                if (!matchfnm)
                {
                    addition += "[NtQueryInfoProcess-filename:";
                    addition += lowparent;
                    addition += "]";
                }

                PathRemoveFileSpec(parentProcessPath);
                string srcp = parentProcessPath;
                string dstp = winDir;
                transform(srcp.begin(), srcp.end(), srcp.begin(), tolower);
                transform(dstp.begin(), dstp.end(), dstp.begin(), tolower);
                if (strcmp(srcp.c_str(), dstp.c_str()))
                {
                    addition += "[NtQueryInfoProcess-path:";
                    addition += srcp;
                    addition += "]";
                }
            }
            CloseHandle(hParent);
        }

        DWORD explorerPid = GetExplorerPIDbyShellWindow();
        if (explorerPid)
        {
            if (parentPid != explorerPid)
            {
                addition += "[GetWindowThreadProcessId:";
                char ppd[10];
                sprintf(ppd, "%d", parentPid);
                addition += ppd;
                addition += "]";
            }
        }
    }

    //----------------------------------------------------------------------
    PROCESSENTRY32 processInfo;
    map<DWORD, pair<DWORD, pair<string, string>>> parent_list; //cpid -> {ppid, {cname, pname}}
    processInfo.dwSize = sizeof(processInfo);
    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return;
    Process32First(processesSnapshot, &processInfo);
    while (Process32Next(processesSnapshot, &processInfo))
    {
        DWORD parentPid = processInfo.th32ParentProcessID;
        CHAR parentProcessPath[MAX_PATH];
        HANDLE hParent = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, parentPid);
        GetProcessImageFileName(hParent, parentProcessPath, MAX_PATH);
        CloseHandle(hParent);            
        string cpname = processInfo.szExeFile;
        string ppname = parentProcessPath;
        transform(cpname.begin(), cpname.end(), cpname.begin(), tolower);
        transform(ppname.begin(), ppname.end(), ppname.begin(), tolower);
        parent_list[processInfo.th32ProcessID] = { processInfo.th32ParentProcessID, {cpname, ppname} };
    }
    CloseHandle(processesSnapshot);

    DWORD cpid = GetCurrentProcessId();
    auto current = parent_list[cpid];
    auto parent = parent_list[current.first];
    
    bool found = false;
    for (int i = 0; i < 6; i++)
    {
        if (!strcmp(parent.second.first.c_str(), terminals[i]))
        {
            found = true;
            CHAR* pfname = PathFindFileName(current.second.second.c_str());
            if (strcmp(pfname, parent.second.first.c_str()))
            {
                found = false;
                if (!current.second.second.empty() || !parent.second.first.empty())
                {
                    addition += "[GetProcessImageFileName:";
                    if (!current.second.second.empty())
                    {
                        addition += current.second.second;
                        addition += ":";
                    }
                    if (!parent.second.first.empty())
                        addition += parent.second.first;
                    addition += "]";
                }
            }
            break;
        }
    }
    if (!found)
    {
        if (!parent.second.first.empty())
        {
            addition += "[CreateToolhelp32Snapshot:";
            addition += parent.second.first;
            addition += "]";
        }
    }
}

int main()
{
    string matches;
    IsParentExplorerExe(matches);
    if (matches.empty())
        printf("PASSED");
    else
        printf("FAILED");
	return 0;
}

{% endraw %}
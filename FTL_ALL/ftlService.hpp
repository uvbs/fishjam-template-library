#ifndef FTL_SERVICE_HPP
#define FTL_SERVICE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlService.h"
#endif

namespace FTL
{
        BOOL CFService::IsServiceInstalled(LPCTSTR lpServiceName)
        {
            BOOL bRet = FALSE;

            SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

            if (hSCM != NULL)
            {
                SC_HANDLE hService = ::OpenService(hSCM, lpServiceName , SERVICE_QUERY_CONFIG);
                if (hService != NULL)
                {
                    bRet = TRUE;
                    ::CloseServiceHandle(hService);
                }
                ::CloseServiceHandle(hSCM);
            }
            return bRet;
        }

        BOOL CFService::InstallService(LPCTSTR lpBinaryPathName,LPCTSTR lpServiceName,LPCTSTR lpDependencies)
        {
            if (IsServiceInstalled(lpServiceName))
                return TRUE;
            BOOL bRet = FALSE;

            SC_HANDLE hSCM = NULL;
            API_VERIFY(( hSCM= ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS))!= NULL);
            if (hSCM == NULL)
            {
                FTLTRACE(_T("Couldn't open service manager\r\n"));
                return FALSE;
            }

            SC_HANDLE hService = NULL;
            API_VERIFY((hService = ::CreateService(
                hSCM, lpServiceName, lpServiceName,
                SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
                SERVICE_AUTO_START,// SERVICE_DEMAND_START���ֶ�������SERVICE_AUTO_START���Զ�����
                SERVICE_ERROR_NORMAL,
                lpBinaryPathName, NULL, NULL, 
                lpDependencies,   //����������ϵ, ������SQL Server����Ҫ����Ϊ _T("MSSQLSERVER\0")��
                NULL, NULL))!=NULL);

            if (hService == NULL)
            {
                ::CloseServiceHandle(hSCM);
                FTLTRACE(_T("Couldn't create service"));
                return FALSE;
            }

            API_VERIFY(::CloseServiceHandle(hService));
            API_VERIFY(::CloseServiceHandle(hSCM));
            return TRUE;
        }

        BOOL CFService::UninstallService(LPCTSTR lpServiceName)
        {
            if (!IsServiceInstalled(lpServiceName))
                return TRUE;

            BOOL bRet = FALSE;
            SC_HANDLE hSCM = NULL;
            API_VERIFY((hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS))!=NULL);
            if (hSCM == NULL)
            {
                FTLTRACE(_T("Couldn't open service manager\r\n"));
                return FALSE;
            }

            SC_HANDLE hService =NULL;
            API_VERIFY((hService = ::OpenService(hSCM, lpServiceName, SERVICE_STOP | DELETE)) != NULL);
            if (hService == NULL)
            {
                ::CloseServiceHandle(hSCM);
                FTLTRACE(_T("Couldn't open service\r\n"));
                return FALSE;
            }
            SERVICE_STATUS status;
            API_VERIFY(::ControlService(hService, SERVICE_CONTROL_STOP, &status));

            BOOL bDelete = ::DeleteService(hService);
            API_VERIFY(::CloseServiceHandle(hService));
            API_VERIFY(::CloseServiceHandle(hSCM));

            if (bDelete)
                return TRUE;

            FTLTRACE(_T("Service could not be deleted\r\n"));
            return FALSE;
        }

        BOOL CFService::CreateServiceUIProcess(LPCTSTR pszProcessPath)
        {
            BOOL bRet = FALSE; 
#if 0
            //*1* Log the client on to the local computer.
            if (!LogonUser(
                lpszUsername,
                lpszDomain,
                lpszPassword,
                LOGON32_LOGON_INTERACTIVE,
                LOGON32_PROVIDER_DEFAULT,
                &hToken) ) 
            {
                ��..
            }
            // *2* Save a handle to the caller's current window station.
            if ( (hwinstaSave = GetProcessWindowStation() ) == NULL)
                ��..;
            // *3* Get a handle to the interactive window station.
            hwinsta = OpenWindowStation(
                "winsta0",                   // the interactive window station 
                FALSE,                       
                READ_CONTROL | WRITE_DAC);   
            // *4* To get the correct default desktop, set the caller's 
            // window station to the interactive window station.
            if (!SetProcessWindowStation(hwinsta))
                ��;
            // *5* Get a handle to the interactive desktop.
            hdesk = OpenDesktop(
                "default",     // the interactive window station 
                0,             // no interaction with other desktop processes
                FALSE,         
                READ_CONTROL | // request the rights to read and write the DACL
                WRITE_DAC | 
                DESKTOP_WRITEOBJECTS | 
                DESKTOP_READOBJECTS);
            // *6* Restore the caller's window station.
            if (!SetProcessWindowStation(hwinstaSave)) 
                ����
                //*7*  Get the SID for the client's logon session.
                if (!GetLogonSID(hToken, &pSid)) 
                    ��������;
            // *8* Allow logon SID full access to interactive window station.
            if (! AddAceToWindowStation(hwinsta, pSid) ) 
                ������;
            // *9* Allow logon SID full access to interactive desktop.
            if (! AddAceToDesktop(hdesk, pSid) ) 
                goto Cleanup;
            // *10* Impersonate client to ensure access to executable file.
            if (! ImpersonateLoggedOnUser(hToken) ) 
            {

            }
            // *11* Initialize the STARTUPINFO structure.
            // Specify that the process runs in the interactive desktop.

            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb= sizeof(STARTUPINFO);
            si.lpDesktop = TEXT("winsta0\\default");

            // *12* Launch the process in the client's logon session.
            bRet = CreateProcessAsUser(
                hToken,            // client's access token
                NULL,              // file to execute
                lpCommandLine,     // command line
                NULL,              // pointer to process SECURITY_ATTRIBUTES
                NULL,              // pointer to thread SECURITY_ATTRIBUTES
                FALSE,             // handles are not inheritable
                NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,   // creation flags
                NULL,              // pointer to new environment block 
                NULL,              // name of current directory 
                &si,               // pointer to STARTUPINFO structure
                &pi                // receives information about new process
                ); 
#endif 
            return bRet;
        }

}
#endif //FTL_SERVICE_HPP
/*
 * Tail for Win32 - a Windows version of the UNIX 'tail -f' command.
 * 
 * Author: Paul Perkins (paul@objektiv.org.uk)
 * 
 * Copyright(c)
 *
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
 * Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * $Id: Tail.cpp,v 1.13 2006/01/23 14:03:09 paulperkins Exp $
 * 
 */

#include "stdafx.h"
#include "Tail.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TailDoc.h"
#include "TailView.h"
#include "Splash.h"
#include "workspacechldfrm.h"
#include "workspacedoc.h"
#include "workspaceview.h"

#include "utils.h"

#include "Plugin.h"
#include "keywordlist.h"
#include "settings.h"

#include "wkspmultidoctemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WKS_EXT   (_T(".wks"))

/////////////////////////////////////////////////////////////////////////////
// CTailApp

BEGIN_MESSAGE_MAP(CTailApp, CWinApp)
	//{{AFX_MSG_MAP(CTailApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_BEEP, OnFileBeep)
	ON_UPDATE_COMMAND_UI(ID_FILE_BEEP, OnUpdateFileBeep)
	ON_COMMAND(IDM_PAUSED, OnPaused)
	ON_UPDATE_COMMAND_UI(IDM_PAUSED, OnUpdatePaused)
	ON_COMMAND(IDM_IGNORE_STARTUP, OnIgnoreStartup)
	ON_UPDATE_COMMAND_UI(IDM_IGNORE_STARTUP, OnUpdateIgnoreStartup)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


CTailMultiDocTemplate::CTailMultiDocTemplate ( 
  UINT nIDResource, 
  CRuntimeClass* pDocClass, 
  CRuntimeClass* pFrameClass, 
  CRuntimeClass* pViewClass ) :  CMultiDocTemplate (nIDResource, pDocClass, pFrameClass, pViewClass)
{

}

CFrameWnd* CTailMultiDocTemplate::CreateNewFrame (
  CRuntimeClass *pNewViewClass,
  CDocument *pDoc, 
  CFrameWnd *pOther)
{
  CRuntimeClass* pOldViewClass = m_pViewClass;

  m_pViewClass=pNewViewClass;

  CFrameWnd* pFrame = CMultiDocTemplate::CreateNewFrame(pDoc, pOther);

  m_pViewClass=pOldViewClass;

  return pFrame;
}


/////////////////////////////////////////////////////////////////////////////
// CTailApp construction

CTailApp::CTailApp()
{
  char szVersionString[255] = "";

  // Load the settings for the app.
  m_pSettings = new CSettings;

  ::LoadString (AfxGetApp ()->m_hInstance, IDS_VERSION, szVersionString, sizeof (szVersionString) - 1);

  LogMessage ("Starting %s...", szVersionString);
  LogMessage ("Built with Developer Studio MFC version 0x%04x", _MSC_VER);

}

CTailApp::~CTailApp()
{
  LogMessage ("Terminating...");

  delete m_pSettings;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTailApp object

CTailApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTailApp initialization

BOOL CTailApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

// Enable3d... are deprecated under .NET, so guard.
#if _MSC_VER <= 0x04b0
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

  InitCommonControls ();

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Objektiv"));

  VersionCheck ();

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CTailMultiDocTemplate(
		IDR_TAILTYPE,
		RUNTIME_CLASS(CTailDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTailView));
	
  AddDocTemplate(m_pDocTemplate);

  m_pWksDocTemplate = new CWorkspaceMultiDocTemplate(
		IDR_TAIL_WORKSPACE,
		RUNTIME_CLASS(CWorkspaceDoc),
		RUNTIME_CLASS(CWorkspaceChldFrm), // custom MDI child frame
		RUNTIME_CLASS(CWorkspaceView));

	AddDocTemplate(m_pWksDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	m_pMainFrame = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

  // Prevent the framework from prompting for which type of doc to open.
  cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

  DragAcceptFiles ((HWND)(this->GetMainWnd()->GetSafeHwnd()), TRUE);

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// VersionCheck
//
//
DWORD CTailApp::VersionCheck (
	void)
{
#ifndef FULL_MONTE

	OSVERSIONINFO	osv;

  osv.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    
	if (!GetVersionEx (&osv))
        return FALSE;
  LogMessage ("Running on Windows %s %ld.%ld", 
                    (osv.dwPlatformId == VER_PLATFORM_WIN32_NT ? "NT/2000/XP" : "95/98/Me"),
                    osv.dwMajorVersion,
                    osv.dwMinorVersion);

  m_fVersion = osv.dwMajorVersion + ((float) osv.dwMinorVersion / 10);
  m_dwPlatformID = osv.dwPlatformId;

  return TRUE;

#else

// Check and log the version of Windows that this platform reports to be.
// Code for this function was lifted in its entirity from MSDN.

  #define BUFSIZE 80
  char szBuf[1024] = "";
  OSVERSIONINFOEX osvi;
  BOOL bOsVersionInfoEx;

  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  // If that fails, try using the OSVERSIONINFO structure.

  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

  if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
  {
    osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
        return FALSE;
  }

  // Save the version for later.
  m_fVersion = osvi.dwMajorVersion + ((float) osvi.dwMinorVersion / 10);
  m_dwPlatformID = osvi.dwPlatformId;

  switch (osvi.dwPlatformId)
  {
    // Test for the Windows NT product family.
    case VER_PLATFORM_WIN32_NT:

        // Test for the specific product family.
        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
          strcat (szBuf, "Microsoft Windows Server&nbsp;2003 family, ");

        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
          strcat (szBuf, "Microsoft Windows XP ");

        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
          strcat (szBuf, "Microsoft Windows 2000 ");

        if ( osvi.dwMajorVersion <= 4 )
          strcat (szBuf, "Microsoft Windows NT ");

        // Test for specific product on Windows NT 4.0 SP6 and later.
        if( bOsVersionInfoEx )
        {
          // Test for the workstation type.
          if ( osvi.wProductType == VER_NT_WORKSTATION )
          {
              if( osvi.dwMajorVersion == 4 )
                strcat (szBuf, "Workstation 4.0 " );
              else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                strcat (szBuf, "Home Edition " );
              else
                strcat (szBuf, "Professional " );
          }
          
          // Test for the server type.
          else if ( osvi.wProductType == VER_NT_SERVER )
          {
              if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
              {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    strcat (szBuf, "Datacenter Edition " );
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    strcat (szBuf, "Enterprise Edition " );
                else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                    strcat (szBuf, "Web Edition " );
                else
                    strcat (szBuf, "Standard Edition " );
              }

              else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
              {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    strcat (szBuf, "Datacenter Server " );
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    strcat (szBuf, "Advanced Server " );
                else
                    strcat (szBuf, "Server " );
              }

              else  // Windows NT 4.0 
              {
                if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    strcat (szBuf, "Server 4.0, Enterprise Edition " );
                else
                    strcat (szBuf, "Server 4.0 " );
              }
          }
        }
        else  // Test for specific product on Windows NT 4.0 SP5 and earlier
        {
          HKEY hKey;
          char szProductType[BUFSIZE];
          DWORD dwBufLen=BUFSIZE;
          LONG lRet;

          lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
              "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
              0, KEY_QUERY_VALUE, &hKey );
          if( lRet != ERROR_SUCCESS )
              return FALSE;

          lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
              (LPBYTE) szProductType, &dwBufLen);
          if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
              return FALSE;

          RegCloseKey( hKey );

          if ( lstrcmpi( "WINNT", szProductType) == 0 )
              strcat (szBuf, "Workstation " );
          if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
              strcat (szBuf, "Server " );
          if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
              strcat (szBuf, "Advanced Server " );

          sprintf (szBuf, "%s %d.%d ", szBuf, osvi.dwMajorVersion, osvi.dwMinorVersion );
        }

    // Display service pack (if any) and build number.

        if( osvi.dwMajorVersion == 4 && 
            lstrcmpi( osvi.szCSDVersion, "Service Pack 6" ) == 0 )
        {
          HKEY hKey;
          LONG lRet;

          // Test for SP6 versus SP6a.
          lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
              "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
              0, KEY_QUERY_VALUE, &hKey );
          if( lRet == ERROR_SUCCESS )
              sprintf (szBuf, "%s Service Pack 6a (Build %d) ", szBuf, osvi.dwBuildNumber & 0xFFFF );         
          else // Windows NT 4.0 prior to SP6a
          {
              sprintf (szBuf, "%s %s (Build %d) ",
                szBuf,
                osvi.szCSDVersion,
                osvi.dwBuildNumber & 0xFFFF);
          }

          RegCloseKey( hKey );
        }
        else // Windows NT 3.51 and earlier or Windows 2000 and later
        {
          sprintf (szBuf, "%s, %s (Build %d) ",
            szBuf,
              osvi.szCSDVersion,
              osvi.dwBuildNumber & 0xFFFF);
        }


        break;

    // Test for the Windows 95 product family.
    case VER_PLATFORM_WIN32_WINDOWS:

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
        {
            strcat (szBuf, "Microsoft Windows 95 ");
            if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
              strcat (szBuf, "OSR2 " );
        } 

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
        {
            strcat (szBuf, "Microsoft Windows 98 ");
            if ( osvi.szCSDVersion[1] == 'A' )
              strcat (szBuf, "SE " );
        } 

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
        {
            strcat (szBuf, "Microsoft Windows Millennium Edition ");
        } 
        break;

    case VER_PLATFORM_WIN32s:

        strcat (szBuf, "Microsoft Win32s ");
        break;
  }

  LogMessage ("Running on %s", szBuf);


  return TRUE;

#endif // #ifndef FULL_MONTE
}


///////////////////////////////////////////////////////////////////////////////
// GetSettings
//
CSettings* CTailApp::GetSettings (
	void)
{
  return m_pSettings;
}

// App command to run the dialog
void CTailApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



/////////////////////////////////////////////////////////////////////////////
// CTailApp commands
  

void CTailApp::OnFileBeep() 
{
  CSettings* pSettings = GetSettings ();

  pSettings->SetBeepOnChanges (!pSettings->GetBeepOnChanges());
}

void CTailApp::OnUpdateFileBeep(CCmdUI* pCmdUI) 
{
  CSettings* pSettings = GetSettings ();

  if (pCmdUI->m_pMenu)
    pCmdUI->SetCheck (pSettings->GetBeepOnChanges() ? MF_CHECKED : MF_UNCHECKED);		
}

BOOL CTailApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CAboutDlg::OnInitDialog() 
{
  char szVersion[255] = "";
  char szMsg[1024] = "";
  int count = 0;

	CDialog::OnInitDialog();
	
  ::LoadString (AfxGetApp ()->m_hInstance, IDS_VERSION, szVersion, sizeof (szVersion) - 1);

  GetDlgItem (IDC_VERSION) -> SetWindowText (szVersion);


  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CTailApp::OnPaused() 
{
/*  
  CSettings* pSettings = GetSettings ();

  pSettings->SetPaused (!pSettings->GetPaused()); 
  */
}

void CTailApp::OnUpdatePaused(CCmdUI* pCmdUI) 
{
  /*
  CSettings* pSettings = GetSettings ();

  if (pCmdUI->m_pMenu)
    pCmdUI->SetCheck (pSettings->GetPaused() ? MF_CHECKED : MF_UNCHECKED);		
    */
}

void CTailApp::OnIgnoreStartup() 
{
  CSettings* pSettings = GetSettings ();

  pSettings->SetIgnoreHotStartup (!pSettings->GetIgnoreHotStartup());			
}

void CTailApp::OnUpdateIgnoreStartup(CCmdUI* pCmdUI) 
{
  CSettings* pSettings = GetSettings ();

  if (pCmdUI->m_pMenu)
    pCmdUI->SetCheck (pSettings->GetIgnoreHotStartup() ? MF_CHECKED : MF_UNCHECKED);		
}


CDocument* CTailApp::OpenDocumentFile (
  LPCTSTR lpszFileName) 
{
  CDocTemplate* pDocTemplate = NULL;
  CDocument* pDoc = NULL;
  CString strExt;
  char szCaption[1024] = "";
  DWORD dwFileAttr = 0;

  // Bug #1344804 - Check file type before opening.
  // This should probably be implemented by handling
  // WM_DROPFILES, but this is a cheap (and easy) way
  // to do it!
  dwFileAttr = GetFileAttributes (lpszFileName);

  if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
  {
    AfxMessageBox ("Sorry, only files can be dropped!");
    return NULL;
  }
	pDoc = CWinApp::OpenDocumentFile(lpszFileName);

  // If this is not a workspace document, then add it to a workspace.
  pDoc->GetDocTemplate()->GetDocString (strExt, CDocTemplate::filterExt);

  if (strExt != WKS_EXT)
  {
    CTailDoc* pTailDoc = (CTailDoc*) pDoc;

    // If this file has already been opened, then OpenDocumentFile
    // will not open another frame (and view) for us - force this to happen.
    if (m_pWorkspace->FileLoaded ((char*) lpszFileName))
    {
      pTailDoc->ShowAnotherWindow ();
    }

    m_pWorkspace->AddWorkspaceFile ((char*) lpszFileName, pTailDoc, pTailDoc->GetActiveView (), NULL);

    sprintf (szCaption, "%s - %s", m_pWorkspace->m_pDefaultWorkspace->GetName (), lpszFileName);

    ((CChildFrame*)pTailDoc->GetActiveView()->GetParentFrame())->SetCaption (szCaption);
  }

  return pDoc;
}

CDocument* CTailApp::OpenDocumentFile (
  LPCTSTR     lpszFileName, 
  CWorkspace* pWksp) 
{
  CDocument* pDoc = NULL;
  CTailDoc* pTailDoc = NULL;
  char szCaption[1024] = "";
	
	pDoc = CWinApp::OpenDocumentFile(lpszFileName);

  pTailDoc = (CTailDoc*) pDoc;

  // If this file has already been opened, then OpenDocumentFile
  // will not open another frame (and view) for us - force this to happen.
  if (m_pWorkspace->FileLoaded ((char*) lpszFileName))
  {
    pTailDoc->ShowAnotherWindow ();
  }

  m_pWorkspace->AddWorkspaceFile ((char*) lpszFileName, pTailDoc, pTailDoc->GetActiveView (), pWksp);

  sprintf (szCaption, "%s - %s", pWksp->GetName (), lpszFileName);
  
  ((CChildFrame*)pTailDoc->GetActiveView()->GetParentFrame())->SetCaption (szCaption);

  return pDoc;
}

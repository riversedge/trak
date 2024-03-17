// trak.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "trak.h"

#include "MainFrm.h"
#include "trakDoc.h"
#include "trakMain.h"
#include "splash.h"
#include "preferences.h"
#include "setup.h"
#include "InstInfo.h"
#include "trakproj.h"

#include <direct.h>

SoundProject	TProj;	
CInstanceInfo	InstInfo;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrakApp

BEGIN_MESSAGE_MAP(CTrakApp, CWinApp)
	//{{AFX_MSG_MAP(CTrakApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrakApp construction

CTrakApp::CTrakApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTrakApp object

CTrakApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrakApp initialization

BOOL CTrakApp::InitInstance()
{
	CSplash Splash;
	
	Splash.DoModal();

	Loading = TRUE;

	// Get information from registry if it exists
	if (GetRegInfo()) 
		InstInfo.FilledPrefs = TRUE;
	else {
		CPreferences Prefs;
		Prefs.DoModal();
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Trak"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTrakDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTrakMain));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	InstInfo.BaseName = "Untitled";

	AlreadyLoading = FALSE;	// Not already loading a project from file|open
	
	if (m_lpCmdLine[0] != '\0') 
    {
       // Open a file passed as the first command line parameter.
	   OpenDocumentFile(m_lpCmdLine);
    }

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
		// No message handlers
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTrakApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTrakApp commands

void CTrakApp::OnFileNew() 
{
	if (Loading) {
		Loading = FALSE;
		return;
	}

	CSetup SetupScreen;

	if (SetupScreen.DoModal() != IDOK) return;

	Saved = FALSE;
	Changed = FALSE;

	CWinApp::OnFileNew();
}

void CTrakApp::OnPreferences() 
{
	CPreferences PrefScreen;
	
	PrefScreen.DoModal();
}

BOOL CTrakApp::GetRegInfo() {
   HKEY hKey;

   DWORD RecDev;
   DWORD PlayDev;
   DWORD PlayBuff;
   DWORD NumPlayBuff;
   DWORD RecBuff;
   DWORD SyncPlay;

   // Get currently selected data if it's there
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\TRAK", 0, KEY_READ, &hKey) == 0) {
      DWORD type;
      DWORD sData;
      sData = sizeof(type);
	  if (RegQueryValueEx(hKey, "RecDevNum", NULL, &type, (LPBYTE)&RecDev, &sData) != ERROR_SUCCESS) return FALSE;
      sData = sizeof(type);
   	  if (RegQueryValueEx(hKey, "PlayDevNum", NULL, &type, (LPBYTE)&PlayDev, &sData) != ERROR_SUCCESS) return FALSE;
      sData = sizeof(type);
	  if (RegQueryValueEx(hKey, "PlayBuffer", NULL, &type, (LPBYTE)&PlayBuff, &sData) != ERROR_SUCCESS) return FALSE;
      sData = sizeof(type);
	  if (RegQueryValueEx(hKey, "PlayBuffers", NULL, &type, (LPBYTE)&NumPlayBuff, &sData) != ERROR_SUCCESS) return FALSE;
      sData = sizeof(type);
	  if (RegQueryValueEx(hKey, "RecBuffer", NULL, &type, (LPBYTE)&RecBuff, &sData) != ERROR_SUCCESS) return FALSE;
      sData = sizeof(type);
	  if (RegQueryValueEx(hKey, "SyncPlay", NULL, &type, (LPBYTE)&SyncPlay, &sData) != ERROR_SUCCESS) return FALSE;
   } else return FALSE;

   InstInfo.RecDev = RecDev;
   InstInfo.PlayDev = PlayDev;
   InstInfo.PlayBuff = PlayBuff;
   InstInfo.NumPlayBuff = NumPlayBuff;
   InstInfo.RecBuff = RecBuff;
   InstInfo.SyncPlay = SyncPlay;

   return TRUE;
}

void CTrakApp::OnFileOpen() 
{
	CFileDialog fDialog(TRUE, "tpj", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
						"Trak Project Files (*.tpj)|*.tpj|All Files (*.*)|*.*|", m_pMainWnd);
	
	if (fDialog.DoModal() != IDOK) return;
	
	InstInfo.FilledStart = TRUE;

	AlreadyLoading = TRUE;

	CString FilePath = fDialog.GetPathName();
	CString FileName = fDialog.GetFileName();
	CString BaseName = fDialog.GetFileTitle();
	
	char BName[5000];
	strncpy(BName, FilePath, 4999);
	TProj.Reset();
	TProj.Load(FilePath);
	BName[strlen(FilePath)-strlen(FileName)-1] = '\0';
	chdir(BName);
	
	// Update the project control
	InstInfo.Filename = FileName;
	InstInfo.Filepath = FilePath;
	InstInfo.BaseName = TProj.GetBase();
	InstInfo.SampRate = TProj.GetSampleRate();
	InstInfo.SampBits = TProj.GetBitRate();
	InstInfo.FilledStart = TRUE;

	SaveFileName = FilePath;

	CWinApp::OpenDocumentFile(FileName);
	AlreadyLoading = FALSE;
}

void CTrakApp::OnFileSave() 
{
	if (!InstInfo.FilledStart) return;

	TProj.Save(SaveFileName);
	
	Saved = TRUE;
	Changed = FALSE;
}

CDocument* CTrakApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{

	return CWinApp::OpenDocumentFile(lpszFileName);
}

// trakDoc.cpp : implementation of the CTrakDoc class
//

#include "stdafx.h"
#include "trak.h"

#include "trakDoc.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTrakApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrakDoc

IMPLEMENT_DYNCREATE(CTrakDoc, CDocument)

BEGIN_MESSAGE_MAP(CTrakDoc, CDocument)
	//{{AFX_MSG_MAP(CTrakDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrakDoc construction/destruction

CTrakDoc::CTrakDoc()
{
	// TODO: add one-time construction code here

}

CTrakDoc::~CTrakDoc()
{
}

BOOL CTrakDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if (!strcmp(TProj.GetBase(), ""))
		SetTitle("Untitled");
	else
		SetTitle(TProj.GetBase());

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTrakDoc serialization

void CTrakDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTrakDoc diagnostics

#ifdef _DEBUG
void CTrakDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTrakDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrakDoc commands

BOOL CTrakDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (theApp.bPlaying || theApp.bRecording) {
		MessageBox(NULL, "Please stop all playback and recording before opening a file.", "Trak Multitracking System", MB_ICONWARNING);
		return FALSE;
	}

	if (theApp.Changed) {
		int rc = MessageBox(NULL, "File has changed.  Open new file anyway?", "Trak Multitracking System", MB_ICONQUESTION | MB_YESNO);
		if (rc == IDNO) return FALSE;
	}

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!theApp.AlreadyLoading) {
		char BName[5000];
		strncpy(BName, lpszPathName, 4999);
		TProj.Reset();
		TProj.Load(lpszPathName);
		char *Start = BName;
		char *LastStr = NULL;
		while (strstr(Start, "\\")) {
			LastStr = strstr(Start, "\\");
			Start = LastStr + sizeof(char);
		}
		if (LastStr != NULL) *LastStr = '\0';
		chdir(BName);
		
		// Update the project control
		InstInfo.Filename = lpszPathName;
		InstInfo.Filepath = BName;
		InstInfo.BaseName = TProj.GetBase();
		InstInfo.SampRate = TProj.GetSampleRate();
		InstInfo.SampBits = TProj.GetBitRate();
		InstInfo.FilledStart = TRUE;

		theApp.SaveFileName = lpszPathName;
	}

	SetTitle(TProj.GetBase());
	
	return TRUE;
}

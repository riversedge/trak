// trakDoc.h : interface of the CTrakDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAKDOC_H__A46CD1A3_A807_11D2_8728_00A02468635B__INCLUDED_)
#define AFX_TRAKDOC_H__A46CD1A3_A807_11D2_8728_00A02468635B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "InstInfo.h"
#include "trakproj.h"

extern SoundProject	TProj;	
extern CInstanceInfo InstInfo;

class CTrakDoc : public CDocument
{
protected: // create from serialization only
	CTrakDoc();
	DECLARE_DYNCREATE(CTrakDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrakDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrakDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTrakDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAKDOC_H__A46CD1A3_A807_11D2_8728_00A02468635B__INCLUDED_)

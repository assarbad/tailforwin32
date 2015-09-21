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
 * $Id: TailDoc.cpp,v 1.10 2004/07/30 07:35:33 paulperkins Exp $
 * 
 */

#include "stdafx.h"
#include "Tail.h"

#include "TailDoc.h"
#include "tailview.h"
#include "settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//DWORD WINAPI ReaderThread (LPVOID pParams);

/////////////////////////////////////////////////////////////////////////////
// CTailDoc

IMPLEMENT_DYNCREATE(CTailDoc, CDocument)

BEGIN_MESSAGE_MAP(CTailDoc, CDocument)
	//{{AFX_MSG_MAP(CTailDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTailDoc construction/destruction

CTailDoc::CTailDoc()
{
	// TODO: add one-time construction code here
//  m_pThread = NULL;
//  m_szEventName[0] = '\0';
//  m_pActiveView = NULL;
}

CTailDoc::~CTailDoc()
{
//  if (m_pThread)
//  {
//    m_pThread->ExitInstance ();
//  }
}


/////////////////////////////////////////////////////////////////////////////
// CTailDoc serialization

void CTailDoc::Serialize(CArchive& ar)
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
// CTailDoc diagnostics

#ifdef _DEBUG
void CTailDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTailDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTailDoc commands

BOOL CTailDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  CTailApp* pApp = (CTailApp*) AfxGetApp ();
  CSettings* pSettings = pApp->GetSettings ();
  CTailView* pView;

//  m_strTailFile = CString (lpszPathName);

  pApp = (CTailApp*) AfxGetApp ();

  // I'm pretty sure this can be removed. 
  // I believe this method is only called by the framework
  // when a real file is chosen to be loaded.
/*  if ((!m_strTailFile || (m_strTailFile == "")) && pSettings->GetAutoLoad ())
  {
    m_strTailFile = AfxGetApp()->GetProfileString (_T("Recent File List"), _T("File 1"), NULL);
  } */

  pView = GetActiveView ();

  // Interesting fix. This needs to be done, otherwise the 
  // document name gets mashed.
  SetTitle (lpszPathName);
  m_strPathName = CString (lpszPathName);
//  SetPathName (m_strTailFile, FALSE);
  SetPathName (lpszPathName, FALSE);


	return TRUE;
}

BOOL CTailDoc::OnNewDocument() 
{
  CTailApp* pApp = (CTailApp*) AfxGetApp();
  CSettings* pSettings = pApp->GetSettings ();
  CString strTailFile;

  strTailFile = GetPathName();

  if (strTailFile.IsEmpty ())
  {
    // Don't load the file.
    return FALSE;

    if (pSettings->GetAutoLoad())
    {  
      strTailFile = pApp->GetProfileString (_T("Recent File List"), _T("File1"), NULL);
    }
    else
    {
      return FALSE;
    } 
  }

  // Interesting fix. 
  SetTitle (strTailFile);
  m_strPathName = strTailFile;
  SetPathName (strTailFile, FALSE);

	return CDocument::OnNewDocument();
}

void CTailDoc::ShowAnotherWindow()
{
  CTailMultiDocTemplate* pTempl = (CTailMultiDocTemplate*)  GetDocTemplate();

  CFrameWnd* pFrame = pTempl->CreateNewFrame (RUNTIME_CLASS (CTailView), this, NULL);

  // Initialize & show the new window
  pTempl->InitialUpdateFrame(pFrame, this);
}

/////////////////////////////////////////////////////////////////////////////
// CTailDoc::GetActiveView
//
// Gets the active view.
// Only reliable is called when the view has *just* been created 
// i.e. immediately after OpenDocumentFile.
//
CTailView* CTailDoc::GetActiveView (void)
{
  CTailView* pView = NULL;
  POSITION pos = 0;

  pos = GetFirstViewPosition ();

  // Loop over all views 'till we get to the
  // end. The last view encountered will be the
  // active one when it is first created.
  while (pos != NULL)
  {
    pView = (CTailView*) GetNextView (pos);
  }

  return pView;
}

BOOL CTailDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
  // Bug #968230: Do not save document!	
  return FALSE;
}

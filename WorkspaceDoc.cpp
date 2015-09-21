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
 * $Id: WorkspaceDoc.cpp,v 1.3 2004/04/07 18:11:14 paulperkins Exp $
 * 
 */

// WorkspaceDoc.cpp : implementation file
//

#include "stdafx.h"
#include "tail.h"
#include "WorkspaceDoc.h"
#include "workspacedlg.h"
#include "workspace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTailApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc

IMPLEMENT_DYNCREATE(CWorkspaceDoc, CDocument)

CWorkspaceDoc::CWorkspaceDoc()
{
  // Note: We don't own this workspace object.
  // It's ours on loan. Don't delete.
  m_pWorkspace = NULL;
}

BOOL CWorkspaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CWorkspaceDoc::~CWorkspaceDoc()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceDoc, CDocument)
	//{{AFX_MSG_MAP(CWorkspaceDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc diagnostics

#ifdef _DEBUG
void CWorkspaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorkspaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc serialization

void CWorkspaceDoc::Serialize(CArchive& ar)
{
  CWorkspaceDlg* pWkspDlg = NULL;
  CWorkspaceItem* pWkspItem = NULL;
  char* pszName = NULL;
  long l = 0;

  pWkspDlg = theApp.m_pWorkspace;

	if (ar.IsStoring())
	{
		ar.WriteString (m_pWorkspace->GetName ());
    ar.WriteString (_T("\n"));

    while (pWkspItem = m_pWorkspace->FindItem (l++))
    {
      ar.WriteString (pWkspItem->GetFilename ());
      ar.WriteString (_T("\n"));
    }
	}
	else
	{
    CString str;

    // Get the workspace name.
    if (ar.ReadString (str))
    {
      m_pWorkspace = pWkspDlg->AddWorkspace (str.GetBuffer (10), this);

      // Get the file names.
      while (ar.ReadString (str))
      {
        pWkspDlg->AddWorkspaceFile (str.GetBuffer (10), m_pWorkspace);
      }
    }
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc commands

BOOL CWorkspaceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc::GetActiveView
//
// Gets the active view.
//
CWorkspaceView* CWorkspaceDoc::GetActiveView (void)
{
  CWorkspaceView* pView = NULL;
  POSITION pos = 0;

  pos = GetFirstViewPosition ();

  // Loop over all views 'till we get to the
  // end. The last view encountered will be the
  // active one when it is first created.
  while (pos != NULL)
  {
    pView = (CWorkspaceView*) GetNextView (pos);
  }

  return pView;
}

void CWorkspaceDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnCloseDocument();
}

BOOL CWorkspaceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

void CWorkspaceDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

void CWorkspaceDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	
}

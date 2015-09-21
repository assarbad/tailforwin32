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
 * $Id: WorkspaceDlg.cpp,v 1.5 2004/04/07 18:11:14 paulperkins Exp $
 * 
 */

// WorkspaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tail.h"
#include "taildoc.h"
#include "WorkspaceDlg.h"
#include "workspacedoc.h"
#include "workspacelistdlg.h"
#include "xtreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORKSPACE_ICON_TOP        (0)
#define WORKSPACE_ICON_WORKSPACE  (1)
#define WORKSPACE_ICON_FOLDER     (2)
#define WORKSPACE_ICON_FILE       (3)
#define WORKSPACE_ICON_DOWN_FILE  (4)

extern CTailApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg dialog


CWorkspaceDlg::CWorkspaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkspaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorkspaceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

//  m_pTree = NULL;

  m_htiRoot = 0;
  m_htiNonWorkspace = 0;
  m_lAnonymousWorkspaceCount = 0;

  m_pDefaultWorkspace = new CWorkspace;

  strcpy (m_pDefaultWorkspace->m_szName, "Non-Workspace Files");

  m_pWorkspaces = new CObList;

//  AddWorkspace (m_pDefaultWorkspace->m_szName);

  m_pWorkspaces->AddTail ((CObject*) m_pDefaultWorkspace);
}

CWorkspaceDlg::~CWorkspaceDlg (void)
{
  POSITION stPos;

  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    delete (CWorkspace*) m_pWorkspaces->GetNext (stPos);
  } 

  delete m_pWorkspaces;
}

void CWorkspaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkspaceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkspaceDlg, CDialog)
	//{{AFX_MSG_MAP(CWorkspaceDlg)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(IDM_TN_RENAMEWORKSPACE, OnWorkspaceRenameWorkspace)
	ON_COMMAND(IDM_TI_ADD_TO_WORKSPACE, OnFileAddToWorkspace)
	ON_COMMAND(IDM_TN_ADDFILETOWORKSPACE, OnWorkspaceAddFileToWorkspace)
	ON_COMMAND(IDM_TN_CLOSEWORKSPACE, OnWorkspaceCloseWorkspace)
	ON_COMMAND(IDM_TN_SAVEWORKSPACE, OnWorkspaceSaveWorkspace)
	ON_COMMAND(IDM_TN_SAVEWORKSPACEAS, OnWorkspaceSaveWorkspaceAs)
	ON_COMMAND(IDM_TI_DEL_FROM_WORKSPACE, OnFileDeleleteFromWorkspace)
	ON_COMMAND(IDM_TI_ADDNEWWORKSPACE, OnTopLevelAddNewWorkspace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::Create (
  LPCTSTR lpszClassName, 
  LPCTSTR lpszWindowName, 
  DWORD dwStyle, 
  const RECT& rect, 
  CWnd* pParentWnd, 
  UINT nID, 
  CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::OnInitDialog (
  void) 
{
  HTREEITEM htiWS1 = 0;

  CDialog::OnInitDialog();

	m_stImageList.Create (IDB_TREE_BUTTONS, 16, 5, RGB(255, 0, 255));

  m_Tree.SetImageList (&m_stImageList, TVSIL_NORMAL);

  m_htiRoot         = m_Tree.AddTreeItem (NULL, _T("Workspaces"), WORKSPACE_ICON_TOP, WORKSPACE_ICON_TOP, NULL);
  m_htiNonWorkspace = m_Tree.AddTreeItem (m_htiRoot, _T("Non-Workspace Files"), WORKSPACE_ICON_WORKSPACE, WORKSPACE_ICON_WORKSPACE, (void*) m_pDefaultWorkspace);

  m_Tree.Expand (m_htiRoot, TVE_EXPAND);
  m_Tree.Expand (m_htiNonWorkspace, TVE_EXPAND);

  m_Tree.m_pWorkspaceDlg = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnSize (
  UINT nType, 
  int cx, 
  int cy) 
{
	CRect rc;

	CDialog::OnSize(nType, cx, cy);

	GetClientRect (rc);
	rc.DeflateRect (5,5);

  m_Tree.MoveWindow (rc);	
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
int CWorkspaceDlg::OnCreate (
  LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_Tree.Create (WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT, // | TVS_EDITLABELS,
		                  CRect(0, 0, 0, 0), 
                      this, 
                      100))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;		// fail to create
	}

	m_Tree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::DblClick (
  void)
{
  CObject* pObject = NULL;

  HTREEITEM hItem = m_Tree.GetSelectedItem ();

	if (hItem) 
  {
		pObject = (CObject*) m_Tree.GetItemData (hItem);

    if (pObject)
    {
      if (pObject->IsKindOf (RUNTIME_CLASS (CWorkspaceItem)))
      {
        CWorkspaceItem* pItem = NULL;

  		  if (pItem = (CWorkspaceItem*) m_Tree.GetItemData (hItem))
        {
          pItem->Activate ();
        }
      }

      if (pObject->IsKindOf (RUNTIME_CLASS (CWorkspace)))
      {
        CWorkspace* pItem = NULL;

  		  if (pItem = (CWorkspace*) m_Tree.GetItemData (hItem))
        {
          pItem->Activate ();
        }
      }

    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::RButton (
  void)
{
  CWorkspaceItem* pWorkspaceItem = NULL;
  CWorkspace*     pWorkspace = NULL;
  CMenu           menu;
  CMenu*          pPopup = NULL;
  CPoint          point;
  DWORD           dwSel = 0;
  HTREEITEM       hItem = 0;
  CObject*        pObject = NULL;

  menu.LoadMenu(IDR_TREE_MENU);

  GetCursorPos (&point);

  // Work out which sub-menu to load.
  // This is a *real* bodge, but I can't be bothered to work out a 
  // more appropriate way at the moment.
  if (hItem = m_Tree.GetSelectedItem ())
  {		
    pObject = (CObject*) m_Tree.GetItemData (hItem);

    if (pObject)
    {
      if (pObject->IsKindOf (RUNTIME_CLASS (CWorkspaceItem)))
      {
        pWorkspaceItem = (CWorkspaceItem*) m_Tree.GetItemData (hItem);

        if (FindWorkspaceItem (pWorkspaceItem))
        {
          pPopup = menu.GetSubMenu(0);
        }
      }

      if (pObject->IsKindOf (RUNTIME_CLASS (CWorkspace)))
      {
		    pWorkspace = (CWorkspace*) m_Tree.GetItemData (hItem);

        if (FindWorkspace (pWorkspace))
        {
          pPopup = menu.GetSubMenu(1);
        }
      }
    }

    // Must be the top-level of the tree.
    if (!pObject)
    {
      pPopup = menu.GetSubMenu(2);
    }

    if (pPopup)
    {
      dwSel = pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, 
                                      point.x, 
                                      point.y,
                                      this);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::AddWorkspaceFile (
  char*       pszFilename, 
  CWorkspace* pWorkspace)
{
  CDocument* pDoc = NULL;

  if (!FindFile (pWorkspace, pszFilename))
  {
    pDoc = theApp.OpenDocumentFile (pszFilename, pWorkspace);    
  }

  return (pDoc != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::AddWorkspaceFile (
  char*       pszFilename, 
  CTailDoc*   pDoc,
  CTailView*  pView,
  CWorkspace* pWorkspace)
{
  CWorkspaceItem* pNewItem = NULL;

  // Check if the file has already ben added to this workspace.
  pNewItem = FindFile (pWorkspace, pszFilename);

  // Brand-spanker - not been seen before.
  if (!pNewItem)
  {
    pNewItem = new CWorkspaceItem;

    pNewItem->m_pDoc  = pDoc;
    pNewItem->m_pView = pView;

    strcpy (pNewItem->m_szTailFile, pszFilename);

    // Workspace not set - add to the default workspace.
    if (!pWorkspace)
    {
      pNewItem->m_hti = m_Tree.AddTreeItem (m_htiNonWorkspace, pszFilename, WORKSPACE_ICON_FILE, WORKSPACE_ICON_FILE, (void*) pNewItem);

      m_pDefaultWorkspace->m_pItems->AddTail ((CObject*) pNewItem);
    
      m_Tree.Expand (m_htiNonWorkspace, TVE_EXPAND);
    }
    else
    {
      pNewItem->m_hti = m_Tree.AddTreeItem (pWorkspace->m_hti, pszFilename, WORKSPACE_ICON_FILE, WORKSPACE_ICON_FILE, (void*) pNewItem);

      pWorkspace->AddItem (pNewItem);
    
      m_Tree.Expand (pWorkspace->m_hti, TVE_EXPAND);
    }

    return TRUE;
  }
  
  if (pNewItem->m_pView)
  {
    // It's an existing item, and it's active.
    // Just say no.
    return FALSE;
  }

  // This must be the case where a view has been 
  // closed and it wasn't within the default workspace.
  // Here, we just update the existing tree item with the 
  // new view.
  pNewItem->m_pView = pView;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::AddWorkspaceFile (
  char*       pszFilename, 
  CTailView*  pView,
  CWorkspace* pWorkspace)
{
  CWorkspaceItem* pNewItem = NULL;

  // Check if the file has already ben added to this workspace.
  if (FindFile (pWorkspace, pszFilename))
  {
    return FALSE;
  }

  pNewItem = new CWorkspaceItem;

  pNewItem->m_pView = pView;

  strcpy (pNewItem->m_szTailFile, pszFilename);

  if (!pWorkspace)
  {
    pNewItem->m_hti = m_Tree.AddTreeItem (m_htiNonWorkspace, pszFilename, WORKSPACE_ICON_FILE, WORKSPACE_ICON_FILE, (void*) pNewItem);

    m_pDefaultWorkspace->m_pItems->AddTail ((CObject*) pNewItem);
    
    m_Tree.Expand (m_htiNonWorkspace, TVE_EXPAND);
  }
  else
  {
    pNewItem->m_hti = m_Tree.AddTreeItem (pWorkspace->m_hti, pszFilename, WORKSPACE_ICON_FILE, WORKSPACE_ICON_FILE, (void*) pNewItem);

    pWorkspace->AddItem (pNewItem);
    
    m_Tree.Expand (pWorkspace->m_hti, TVE_EXPAND);
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::DeleteWorkspaceFile (
  CWorkspaceItem* pItem)
{
  POSITION stPos;
  CWorkspaceItem* pFoundItem = NULL;
  CWorkspace* pWksp = NULL;

  // Close the item's view.
  pItem->Close ();

  // Loop over all workspaces to find the view.
  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pWksp = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    if (pFoundItem = pWksp->FindView (pItem->m_pView))
    {
      m_Tree.DelTreeItem (pItem->m_hti);

      pWksp->RemoveItem (pItem);

      delete (CWorkspaceItem*) pItem;

      // If it's not in the default workspace, mark it as modified.
      if (pWksp != m_pDefaultWorkspace)
      {
        pWksp->SetModified (TRUE);
      }

      return TRUE;
    }
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::RemoveWorkspaceFile (
  CTailView* pView)
{
  POSITION stPos;
  CWorkspaceItem* pItem = NULL;
  CWorkspace* pWksp = NULL;

  // Loop over all workspaces to find the view.
  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pWksp = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    if (pItem = pWksp->FindView (pView))
    {
      // If the item belongs to the default workspace,
      // then delete it from the workspace and the tree.
      if (pWksp == m_pDefaultWorkspace)
      {
        m_Tree.DelTreeItem (pItem->m_hti);

        pWksp->RemoveItem (pItem);

        delete (CWorkspaceItem*) pItem;
      }
      else 
      {
        // Otherwise, just close the file. It can then
        // be activated later.
        pItem->m_pView = NULL;
      }

      return TRUE;
    }
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::RemoveWorkspaceFile (
  CTailView*  pView,
  CWorkspace* pWorkspace)
{
  CWorkspaceItem* pItem = NULL;
  CWorkspace* pSearch = NULL;

  pSearch = pWorkspace;

  if (!pWorkspace)
  {
    pSearch = m_pDefaultWorkspace;
  }

  if (pItem = pSearch->FindView (pView))
  {
    m_Tree.DelTreeItem (pItem->m_hti);

    pSearch->RemoveItem (pItem);

    delete (CWorkspaceItem*) pItem;
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::RemoveWorkspaceFile (
  CTailDoc*   pDoc,
  CWorkspace* pWorkspace)
{
  CWorkspaceItem* pItem = NULL;
  CWorkspace* pSearch = NULL;

  pSearch = pWorkspace;

  if (!pWorkspace)
  {
    pSearch = m_pDefaultWorkspace;
  }

  if (pItem = pSearch->FindDocument (pDoc))
  {
    m_Tree.DelTreeItem (pItem->m_hti);

    pSearch->RemoveItem (pItem);

    delete (CWorkspaceItem*) pItem;
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
CWorkspace* CWorkspaceDlg::AddWorkspace (
  char*       pszName)
{
  CWorkspace* pNewItem = NULL;

  pNewItem = new CWorkspace;

  strcpy (pNewItem->m_szName, pszName);

  pNewItem->m_hti = m_Tree.AddTreeItem (m_htiRoot, 
                                pszName, 
                                WORKSPACE_ICON_WORKSPACE, 
                                WORKSPACE_ICON_WORKSPACE, 
                                (void*) pNewItem);

//  m_DefaultWorkspace.m_pItems->AddTail ((CObject*) pNewItem);
  
  m_Tree.Expand (m_htiRoot, TVE_EXPAND);

  m_pWorkspaces->AddTail ((CObject*) pNewItem);

  return pNewItem;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
CWorkspace* CWorkspaceDlg::AddWorkspace (
  char*          pszName,
  CWorkspaceDoc* pDoc)
{
  CWorkspace* pNewItem = NULL;

  pNewItem = new CWorkspace;

  strcpy (pNewItem->m_szName, pszName);

  pNewItem->m_pWkspDoc = pDoc;

  pNewItem->m_hti = m_Tree.AddTreeItem (m_htiRoot, 
                                pszName, 
                                WORKSPACE_ICON_WORKSPACE, 
                                WORKSPACE_ICON_WORKSPACE, 
                                (void*) pNewItem);

//  m_DefaultWorkspace.m_pItems->AddTail ((CObject*) pNewItem);
  
  m_Tree.Expand (m_htiRoot, TVE_EXPAND);

  m_pWorkspaces->AddTail ((CObject*) pNewItem);

  return pNewItem;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::AddNewWorkspace (
  void)
{
  char szWkspName[1024] = "";

  sprintf (szWkspName, "Workspace %ld", ++m_lAnonymousWorkspaceCount);

  AddWorkspace (szWkspName);

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::RemoveWorkspace (
  CWorkspace* pWorkspace)
{
  CWorkspaceItem* pItem = NULL;
  CWorkspace* pSearch = NULL;
  POSITION stPos;
  CWorkspace* pFound = NULL;
  long lIndex = 0;

  pSearch = pWorkspace;

  if (!pWorkspace)
  {
    pSearch = m_pDefaultWorkspace;
  }

  // Can't close the default workspace.
  if (pSearch != m_pDefaultWorkspace)
  {
    // Find the workspace.
    while ((stPos = m_pWorkspaces->FindIndex (lIndex++)) != NULL)
    {
      pFound = (CWorkspace*) m_pWorkspaces->GetAt (stPos);

      if (pFound == pWorkspace)
      {
        m_Tree.DelTreeItem (pFound->m_hti);

        m_pWorkspaces->RemoveAt (stPos);

        delete (CWorkspace*) pFound;

        return TRUE;
      }
    }
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
// See if this file already exists in the given workspace.
CWorkspaceItem* CWorkspaceDlg::FindFile (
  CWorkspace* pWorkspace,
  char*       pszFilename)
{
  POSITION stPos;
  CWorkspace* pSearch = NULL;
  CWorkspaceItem* pFound = NULL;

  pSearch = pWorkspace;

  if (!pWorkspace)
  {
    pSearch = m_pDefaultWorkspace;
  }

  for (stPos = pSearch->m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspaceItem*) pSearch->m_pItems->GetNext (stPos);

    if (strcmp (pFound->GetFilename (), pszFilename) == 0)
    {
      return pFound;
    }
  }

  return NULL;

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
CWorkspace* CWorkspaceDlg::FindWorkspace (
  CWorkspace* pWorkspace)
{
  POSITION stPos;
  CWorkspace* pFound = NULL;

  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    if (pFound == pWorkspace)
    {
      return pWorkspace;
    }
  }

  return NULL;

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
// Find the workspace containing the given workspace item.
//
CWorkspace* CWorkspaceDlg::GetWorkspace (
  CWorkspaceItem* pWorkspaceItem)
{
  POSITION        stPos;
  CWorkspace*     pFoundWksp = NULL;
  CWorkspaceItem* pFoundItem = NULL;

  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pFoundWksp = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    if (pFoundWksp->FindItem (pWorkspaceItem))
    {
      return pFoundWksp;
    }
  }

  return NULL;

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg::FileLoaded 
//
// Determines whether the given file is loaded (and active) within any
// workspace.
// Currently used to determine whether a new frame window should be created
// for this file.
//
BOOL CWorkspaceDlg::FileLoaded (
  char* pszFilename)
{
  POSITION stPos;
  CWorkspace* pFound = NULL;
  CWorkspaceItem* pFoundItem = NULL;

  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    // If any of the files are active, then we should load another frame.
    if ((pFoundItem = pFound->FindItem (pszFilename)) && (pFoundItem->m_pView))
    {
      return TRUE;
    }
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
CWorkspaceItem* CWorkspaceDlg::FindWorkspaceItem (
  CWorkspaceItem* pWorkspaceItem)
{
  POSITION stPos;
  CWorkspace* pFound = NULL;
  CWorkspaceItem* pFoundItem = NULL;

  for (stPos = m_pWorkspaces->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspace*) m_pWorkspaces->GetNext (stPos);

    if (pFound->FindItem (pWorkspaceItem))
    {
      return pWorkspaceItem;
    }
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
CWorkspace* CWorkspaceDlg::FindWorkspace (
  long lIndex)
{
  POSITION stPos;

  if ((stPos = m_pWorkspaces->FindIndex (lIndex)) != NULL)
  {
      return (CWorkspace*) m_pWorkspaces->GetAt (stPos);
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnWorkspaceRenameWorkspace() 
{
  ::AfxMessageBox ("Rename workspace");	

	m_Tree.ModifyStyleEx (0, TVS_EDITLABELS);
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnWorkspaceAddFileToWorkspace() 
{
  ::AfxMessageBox ("Add file to workspace");	
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnWorkspaceCloseWorkspace() 
{
  CWorkspace* pWksp = NULL;

  // Close the windows of the workspace.
  HTREEITEM hItem = m_Tree.GetSelectedItem ();

  if (hItem && (pWksp = (CWorkspace*) m_Tree.GetItemData (hItem)))
  {
    // This will close all subwindows.
    pWksp->Close ();

    RemoveWorkspace (pWksp);
  }  
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnWorkspaceSaveWorkspace() 
{
  CWorkspace* pWksp = NULL;

  // Close the windows of the workspace.
  HTREEITEM hItem = m_Tree.GetSelectedItem ();

  if (hItem && (pWksp = (CWorkspace*) m_Tree.GetItemData (hItem)))
  {
//    pWksp->m_pWkspDoc->DoSaveFile ();
  }  
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnWorkspaceSaveWorkspaceAs() 
{
  ::AfxMessageBox ("Save workspace as...");	
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnFileAddToWorkspace() 
{
  CWorkspaceListDlg stDlg;
  CWorkspaceItem* pItem = NULL;
  CWorkspace* pWorkspace = NULL;
  int nItem = 0;

  HTREEITEM hItem = m_Tree.GetSelectedItem ();

  if (hItem && (pItem = (CWorkspaceItem*) m_Tree.GetItemData (hItem)))
  {
    nItem = stDlg.DoModal ();

    if (nItem != -1)
    {
      if (pWorkspace = FindWorkspace (nItem))
      {
        AddWorkspaceFile ((char*) pItem->GetFilename (), pWorkspace);

        pWorkspace->SetModified (TRUE);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
void CWorkspaceDlg::OnFileDeleleteFromWorkspace() 
{
  CWorkspaceItem* pItem = NULL;
  HTREEITEM       hItem = NULL;

  hItem = m_Tree.GetSelectedItem ();

  if (hItem && (pItem = (CWorkspaceItem*) m_Tree.GetItemData (hItem)))
  {
    DeleteWorkspaceFile (pItem);
  }
}

void CWorkspaceDlg::OnTopLevelAddNewWorkspace() 
{
  AddNewWorkspace ();
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDlg 
//
BOOL CWorkspaceDlg::UpdateWorkspace (
  CWorkspace* pWksp)
{
  if (FindWorkspace (pWksp))
  {
    return m_Tree.SetItemText (pWksp->m_hti, pWksp->GetName ());
  }

  return FALSE;
}


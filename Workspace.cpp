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
 * $Id: Workspace.cpp,v 1.7 2006/01/23 14:04:38 paulperkins Exp $
 * 
 */

#include "stdafx.h"

#include "tail.h"
#include "taildoc.h"
#include "workspacedoc.h"
#include "workspaceview.h"
#include "tailview.h"
#include "workspace.h"

IMPLEMENT_DYNAMIC(CWorkspaceItem, CObject)

extern CTailApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceItem
//
CWorkspaceItem::CWorkspaceItem (void)
{
  m_hti = 0;
  m_pWksp = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceItem 
//
CWorkspaceItem::~CWorkspaceItem (void)
{
  m_pView = NULL;
  m_pDoc = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceItem 
//
const char* CWorkspaceItem::GetFilename (
  void)
{
  return &m_szTailFile[0];
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceItem:: Close
//
// Close the frame object. This should simulate the user clicking the 
// close button, and hence should do the necessary with the GUI.
//
void CWorkspaceItem::Close (
  void)
{
  CFrameWnd* pFrame= m_pView->GetParentFrame();
  
  ASSERT_VALID(pFrame);

  pFrame->PostMessage(WM_CLOSE);

//  m_pDoc->OnCloseDocument ();
//  m_pView->OnDestroy ();
}


IMPLEMENT_DYNAMIC(CWorkspace, CObject)

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
CWorkspace::CWorkspace (void)
{
  m_pItems = new CObList;
  m_hti = 0;
  m_pWkspDoc = NULL;

  memset (&m_szName[0], 0, sizeof (m_szName));
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
CWorkspace::~CWorkspace (void)
{
  POSITION stPos;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    delete (CWorkspaceItem*) m_pItems->GetNext (stPos);
  } 

  delete m_pItems;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceItem::Activate
//
void CWorkspaceItem::Activate (
  void)
{
  if (m_pView)
  {
    m_pView->GetParentFrame()->ActivateFrame ();

    if (m_pView->GetParentFrame()->IsIconic ())
    {
      m_pView->GetParentFrame()->ActivateFrame (SW_RESTORE);
    }
  }   
  else
  {
    // No view - re-open the file.
    theApp.OpenDocumentFile (&m_szTailFile[0], m_pWksp);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::Close
//
void CWorkspace::Close (
  void)
{
  POSITION stPos;
  CWorkspaceItem* pFound = NULL;

  // Close each attached workspace item.
  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    pFound->Close ();
  }

  // Bug #1337505 - Check that member is valid before proceeding. 
  if (m_pWkspDoc)
  {
    CFrameWnd* pFrame = m_pWkspDoc->GetActiveView()->GetParentFrame();

    ASSERT_VALID(pFrame);

    pFrame->PostMessage(WM_CLOSE);
  }
}


/////////////////////////////////////////////////////////////////////////////
// CWorkspace::FindItem
//
CWorkspaceItem* CWorkspace::FindItem (
  char* pszFilename)
{
  POSITION stPos;
  CWorkspaceItem* pFound = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    if (strcmp (pFound->GetFilename (), pszFilename) == 0)
    {
      return pFound;
    }
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::FindItem
//
CWorkspaceItem* CWorkspace::FindItem (
  CWorkspaceItem* pItem)
{
  POSITION stPos;
  CWorkspaceItem* pFound = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    if (pFound == pItem)
    {
      return pItem;
    }
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::FindItem
//
CWorkspaceItem* CWorkspace::FindItem (
  long lIndex)
{
  POSITION stPos;

  if ((stPos = m_pItems->FindIndex (lIndex)) != NULL)
  {
      return (CWorkspaceItem*) m_pItems->GetAt (stPos);
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::FindDocument
//
CWorkspaceItem* CWorkspace::FindDocument (
  CTailDoc* pDoc)
{
  POSITION stPos;
  CWorkspaceItem* pItem = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pItem = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    if (pItem->m_pDoc == pDoc)
    {
      return pItem;
    }
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::FindView
//
CWorkspaceItem* CWorkspace::FindView (
  CTailView* pView)
{
  POSITION stPos;
  CWorkspaceItem* pItem = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pItem = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    if (pItem->m_pView == pView)
    {
      return pItem;
    }
  }

  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
CWorkspaceItem* CWorkspace::AddDocument (
  CTailDoc* pDoc)
{
  CWorkspaceItem* pItem = NULL;

  if (!(pItem = FindDocument (pDoc)))
  {    
    pItem = new CWorkspaceItem;

    pItem->m_pWksp = this;

//    pItem->m_pDoc = pDoc;
//    pDoc->m_pWorkspace = this;

    m_pItems->AddTail ((CObject*) pItem);
  }

  return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
CWorkspaceItem* CWorkspace::AddItem (
  CWorkspaceItem* pItem)
{
  if (!FindItem (pItem))
  {    
    pItem->m_pWksp = this;

    m_pItems->AddTail ((CObject*) pItem);
  }

  return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
BOOL CWorkspace::RemoveItem (
  CWorkspaceItem* pItem)
{
  POSITION stPos;
  CWorkspaceItem* pFound = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pFound = (CWorkspaceItem*) m_pItems->GetAt (stPos);

    if (pItem == pFound)
    {
      m_pItems->RemoveAt (stPos);

      return TRUE;
    }
  
    m_pItems->GetNext (stPos);
  }

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace 
//
// Activate all windows belonging to the workspace.
//
void CWorkspace::Activate (
  void)
{
  POSITION stPos;
  CWorkspaceItem* pItem = NULL;

  for (stPos = m_pItems->GetHeadPosition (); stPos != NULL; ) 
  {
    pItem = (CWorkspaceItem*) m_pItems->GetNext (stPos);

    pItem->Activate ();
  }

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::GetName
//
// Returns name of this workspace.
//
const char* CWorkspace::GetName (
  void)
{
  return &m_szName[0];
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::SetName
//
// Sets name of this workspace.
//
void CWorkspace::SetName (
  const char* pszName)
{
  strcpy (m_szName, pszName);
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspace::SetModified
//
// Sets the modified flag in the owning document.
//
void CWorkspace::SetModified (
  BOOL bModified)
{
  if (m_pWkspDoc)
  {
    m_pWkspDoc->SetModifiedFlag (bModified);
  }
}
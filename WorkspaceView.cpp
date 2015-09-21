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
 * $Id: WorkspaceView.cpp,v 1.1 2003/11/25 17:14:20 paulperkins Exp $
 * 
 */

// WorkspaceView.cpp : implementation file
//

#include "stdafx.h"
#include "tail.h"
#include "WorkspaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView

IMPLEMENT_DYNCREATE(CWorkspaceView, CView)

CWorkspaceView::CWorkspaceView()
{
}

CWorkspaceView::~CWorkspaceView()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceView, CView)
	//{{AFX_MSG_MAP(CWorkspaceView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView drawing

void CWorkspaceView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceView diagnostics

#ifdef _DEBUG
void CWorkspaceView::AssertValid() const
{
	CView::AssertValid();
}

void CWorkspaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


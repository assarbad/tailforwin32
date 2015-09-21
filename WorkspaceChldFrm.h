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
 * $Id: WorkspaceChldFrm.h,v 1.1 2003/11/25 17:14:20 paulperkins Exp $
 * 
 */

#if !defined(AFX_WORKSPACECHLDFRM_H__F7279673_1C23_4540_9F5C_C5FA90DD8889__INCLUDED_)
#define AFX_WORKSPACECHLDFRM_H__F7279673_1C23_4540_9F5C_C5FA90DD8889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// WorkspaceChldFrm.h : header file
//
// The only purpose for this sub-class is to prevent automatic display of 
// the document.
// The document is used to manage the contents of the Tail 'workspace' 
// files.
//

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceChldFrm frame

class CWorkspaceChldFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWorkspaceChldFrm)
protected:
	CWorkspaceChldFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceChldFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWorkspaceChldFrm();

	// Generated message map functions
	//{{AFX_MSG(CWorkspaceChldFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACECHLDFRM_H__F7279673_1C23_4540_9F5C_C5FA90DD8889__INCLUDED_)

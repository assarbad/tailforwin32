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
 * $Id: WorkspaceDoc.h,v 1.3 2004/04/07 18:11:14 paulperkins Exp $
 * 
 */

#if !defined(AFX_WORKSPACEDOC_H__CF8AD98A_C1C9_481E_817A_6A12C92E1CC6__INCLUDED_)
#define AFX_WORKSPACEDOC_H__CF8AD98A_C1C9_481E_817A_6A12C92E1CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkspaceDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceDoc document

class CWorkspace;
class CWorkspaceView;

class CWorkspaceDoc : public CDocument
{
protected:
	CWorkspaceDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWorkspaceDoc)

// Attributes
public:
  CWorkspaceView* GetActiveView (void);


private:
  CWorkspace* m_pWorkspace;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEDOC_H__CF8AD98A_C1C9_481E_817A_6A12C92E1CC6__INCLUDED_)

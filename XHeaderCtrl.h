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
 * $Id: XHeaderCtrl.h,v 1.1 2003/11/11 07:31:45 paulperkins Exp $
 * 
 */

// XHeaderCtrl.h
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XHEADERCTRL_H
#define XHEADERCTRL_H

#include <tchar.h>
#include "MemDC.h"

#define FLATHEADER_TEXT_MAX	80

#define XHEADERCTRL_NO_IMAGE		0
#define XHEADERCTRL_UNCHECKED_IMAGE	1
#define XHEADERCTRL_CHECKED_IMAGE	2

///////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl window

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5


class CXHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNCREATE(CXHeaderCtrl)

// Construction
public:
	CXHeaderCtrl();
	virtual ~CXHeaderCtrl();

// Attributes
public:
	BOOL ModifyProperty(WPARAM wParam, LPARAM lParam);
	int GetSpacing() { return m_iSpacing; }
	void SetSpacing(int nSpacing) { m_iSpacing = nSpacing; }

// Overrides
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bDoubleBuffer;
	int m_iSpacing;
	SIZE m_sizeImage;
	SIZE m_sizeArrow;
	BOOL m_bStaticBorder;
	UINT m_nDontDropCursor;
	BOOL m_bResizing;
	UINT m_nClickFlags;
	CPoint m_ptClickPoint;

	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;

	void DrawCtrl(CDC* pDC);
	int DrawImage(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	int DrawBitmap(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, 
		BITMAP* pBitmapInfo, BOOL bRight);
	int DrawText (CDC* pDC, CRect rect, LPHDITEM lphdi);

// Generated message map functions
protected:
	//{{AFX_MSG(CXHeaderCtrl)
	afx_msg LRESULT OnDeleteItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnInsertItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetImageList(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XHEADERCTRL_H

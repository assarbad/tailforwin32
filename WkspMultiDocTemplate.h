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
 * $Id: WkspMultiDocTemplate.h,v 1.1 2003/11/25 17:18:32 paulperkins Exp $
 * 
 */

#ifndef __WORKSPACE_DOC_TEMPLATE__
#define __WORKSPACE_DOC_TEMPLATE__

class CWorkspaceMultiDocTemplate : public CMultiDocTemplate
{
public:
  CWorkspaceMultiDocTemplate ( UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass );

//  CFrameWnd* CreateNewFrame (CRuntimeClass *pNewViewClass, CDocument *pDoc, CFrameWnd *pOther);

};

#endif // #ifndef __WORKSPACE_DOC_TEMPLATE__

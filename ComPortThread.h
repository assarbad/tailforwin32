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
 * $Id: ComPortThread.h,v 1.1 2006/01/23 08:22:27 paulperkins Exp $
 * 
 */

#ifndef __COMPORT_THREAD_H__
#define __COMPORT_THREAD_H__

//#include "tailview.h"

class CTailView;

typedef struct _COM_PORT_THREAD_PARAMS
{
//  HANDLE hDie;
  char szFileName[_MAX_FNAME];
  char szEvent[_MAX_FNAME];
  char szDieEvent[_MAX_FNAME];
  HWND hEdit;
  BOOL bUseTimeout;
  BOOL bIgnoreHotStartup;
  BOOL bDebug;
  int  nTimeout;
  CTailView* pView;
  BOOL bPaused;

} COM_PORT_THREAD_PARAMS;

UINT EventLogThread (
  LPVOID pParams);


#endif // #ifdef __COMPORT_THREAD_H__

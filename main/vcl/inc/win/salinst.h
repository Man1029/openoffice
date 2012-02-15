/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _SV_SALINST_H
#define _SV_SALINST_H

#include <salinst.hxx>

namespace vos { class OMutex; }

// -------------------
// - SalInstanceData -
// -------------------

class SalYieldMutex;

class WinSalInstance : public SalInstance
{
public:
	HINSTANCE			mhInst; 				// Instance Handle
	HWND				mhComWnd;				// window, for communication (between threads and the main thread)
	SalYieldMutex*		mpSalYieldMutex;		// Sal-Yield-Mutex
	vos::OMutex*		mpSalWaitMutex; 		// Sal-Wait-Mutex
	sal_uInt16				mnYieldWaitCount;		// Wait-Count
public:
    WinSalInstance();
    virtual ~WinSalInstance();

    virtual SalFrame*      	CreateChildFrame( SystemParentData* pParent, sal_uIntPtr nStyle );
    virtual SalFrame*      	CreateFrame( SalFrame* pParent, sal_uIntPtr nStyle );
    virtual void			DestroyFrame( SalFrame* pFrame );
    virtual SalObject*		CreateObject( SalFrame* pParent, SystemWindowData* pWindowData, sal_Bool bShow = sal_True );
    virtual void			DestroyObject( SalObject* pObject );
    virtual SalVirtualDevice*	CreateVirtualDevice( SalGraphics* pGraphics,
                                                     long nDX, long nDY,
                                                     sal_uInt16 nBitCount, const SystemGraphicsData *pData );
    virtual void			DestroyVirtualDevice( SalVirtualDevice* pDevice );

    virtual SalInfoPrinter*	CreateInfoPrinter( SalPrinterQueueInfo* pQueueInfo,
                                               ImplJobSetup* pSetupData );
    virtual void			DestroyInfoPrinter( SalInfoPrinter* pPrinter );
    virtual SalPrinter*		CreatePrinter( SalInfoPrinter* pInfoPrinter );
    virtual void			DestroyPrinter( SalPrinter* pPrinter );
    virtual void			GetPrinterQueueInfo( ImplPrnQueueList* pList );
    virtual void			GetPrinterQueueState( SalPrinterQueueInfo* pInfo );
    virtual void			DeletePrinterQueueInfo( SalPrinterQueueInfo* pInfo );
    virtual String             GetDefaultPrinter();
    virtual SalTimer*			CreateSalTimer();
    virtual SalI18NImeStatus*	CreateI18NImeStatus();
    virtual SalSystem*			CreateSalSystem();
    virtual SalBitmap*			CreateSalBitmap();
    virtual vos::IMutex*		GetYieldMutex();
    virtual sal_uIntPtr			ReleaseYieldMutex();
    virtual void				AcquireYieldMutex( sal_uIntPtr nCount );
    virtual bool                CheckYieldMutex();

    virtual void				Yield( bool bWait, bool bHandleAllCurrentEvents );
    virtual bool				AnyInput( sal_uInt16 nType );
    virtual SalMenu*			CreateMenu( sal_Bool bMenuBar, Menu* );
    virtual void				DestroyMenu( SalMenu* );
    virtual SalMenuItem*		CreateMenuItem( const SalItemParams* pItemData );
    virtual void				DestroyMenuItem( SalMenuItem* );
    virtual SalSession*                         CreateSalSession();
    virtual void*				GetConnectionIdentifier( ConnectionIdentifierType& rReturnedType, int& rReturnedBytes );
    virtual void                AddToRecentDocumentList(const rtl::OUString& rFileUrl, const rtl::OUString& rMimeType);

    static int WorkaroundExceptionHandlingInUSER32Lib(int nExcept, LPEXCEPTION_POINTERS pExceptionInfo);
};

// --------------
// - Prototypen -
// --------------

SalFrame* ImplSalCreateFrame( WinSalInstance* pInst, HWND hWndParent, sal_uIntPtr nSalFrameStyle );
SalObject* ImplSalCreateObject( WinSalInstance* pInst, WinSalFrame* pParent );
HWND ImplSalReCreateHWND( HWND hWndParent, HWND oldhWnd, sal_Bool bAsChild );
void ImplSalStartTimer( sal_uIntPtr nMS, sal_Bool bMutex = sal_False );
void ImplSalPrinterAbortJobAsync( HDC hPrnDC );

#endif // _SV_SALINST_H

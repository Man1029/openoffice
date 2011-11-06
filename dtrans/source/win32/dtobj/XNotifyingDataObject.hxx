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




#ifndef _XNOTIFYINGDATAOBJECT_HXX_
#define _XNOTIFYINGDATAOBJECT_HXX_


//------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------

#include <com/sun/star/datatransfer/XTransferable.hpp>
#include <com/sun/star/datatransfer/clipboard/XClipboardOwner.hpp>

#if defined _MSC_VER
#pragma warning(push,1)
#endif
#include <objidl.h>
#include <windows.h>
#if defined _MSC_VER
#pragma warning(pop)
#endif

#include <systools/win32/comtools.hxx>

/*--------------------------------------------------------------------------
	To implement the lostOwnership mechanism cleanly we need this wrapper
	object
----------------------------------------------------------------------------*/

// forward
class CWinClipbImpl;

class CXNotifyingDataObject : public IDataObject
{
public:
	CXNotifyingDataObject( 
		const IDataObjectPtr& aIDataObject,		
		const com::sun::star::uno::Reference< com::sun::star::datatransfer::XTransferable >& aXTransferable,
		const com::sun::star::uno::Reference< com::sun::star::datatransfer::clipboard::XClipboardOwner >& aXClipOwner,
		CWinClipbImpl* theWinClipImpl );

    virtual ~CXNotifyingDataObject() {}
    
	//-----------------------------------------------------------------
	// ole interface implementation
	//-----------------------------------------------------------------

    //IUnknown interface methods
    STDMETHODIMP           QueryInterface(REFIID iid, LPVOID* ppvObject);
    STDMETHODIMP_( ULONG ) AddRef( );
    STDMETHODIMP_( ULONG ) Release( );

    // IDataObject interface methods
    STDMETHODIMP GetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP GetDataHere( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP QueryGetData( LPFORMATETC pFormatetc );
    STDMETHODIMP GetCanonicalFormatEtc( LPFORMATETC pFormatectIn, LPFORMATETC pFormatetcOut );
    STDMETHODIMP SetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium, BOOL fRelease );
    STDMETHODIMP EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC** ppenumFormatetc );
    STDMETHODIMP DAdvise( LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink, DWORD* pdwConnection );
    STDMETHODIMP DUnadvise( DWORD dwConnection );
    STDMETHODIMP EnumDAdvise( LPENUMSTATDATA* ppenumAdvise );

	operator IDataObject*( );

private:
	void SAL_CALL lostOwnership( );

private:	
	sal_Int32																							m_nRefCnt;
	IDataObjectPtr																						m_aIDataObject;	
	const com::sun::star::uno::Reference< com::sun::star::datatransfer::XTransferable >					m_XTransferable;
	const com::sun::star::uno::Reference< com::sun::star::datatransfer::clipboard::XClipboardOwner >	m_XClipboardOwner;
	CWinClipbImpl*																						m_pWinClipImpl;

	friend class CWinClipbImpl;
};

#endif

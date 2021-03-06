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



#undef UNICODE
#undef _UNICODE

#define _WIN32_WINDOWS 0x0410

#ifdef _MSC_VER
#pragma warning(push, 1) /* disable warnings within system headers */
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <msiquery.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <malloc.h>
#include <assert.h>

#include <tchar.h>
#include <string>
#include <systools/win32/uwinapi.h>

#include <../tools/seterror.hxx>

using namespace std;

namespace
{
    string GetMsiProperty(MSIHANDLE handle, const string& sProperty)
    {
        string  result;
        TCHAR   szDummy[1] = TEXT("");
        DWORD   nChars = 0;

        if (MsiGetProperty(handle, sProperty.c_str(), szDummy, &nChars) == ERROR_MORE_DATA)
        {
            DWORD nBytes = ++nChars * sizeof(TCHAR);
            LPTSTR buffer = reinterpret_cast<LPTSTR>(_alloca(nBytes));
            ZeroMemory( buffer, nBytes );
            MsiGetProperty(handle, sProperty.c_str(), buffer, &nChars);
            result = buffer;
        }
        return result;
    }

    inline bool IsSetMsiProperty(MSIHANDLE handle, const string& sProperty)
    {
        return (GetMsiProperty(handle, sProperty).length() > 0);
    }

    inline void UnsetMsiProperty(MSIHANDLE handle, const string& sProperty)
    {
        MsiSetProperty(handle, sProperty.c_str(), NULL);
    }

    inline void SetMsiProperty(MSIHANDLE handle, const string& sProperty, const string&)
    {
        MsiSetProperty(handle, sProperty.c_str(), TEXT("1"));
    }

    void stripFinalBackslash(std::string * path) {
        std::string::size_type i = path->size();
        if (i > 1) {
            --i;
            if ((*path)[i] == '\\') {
                path->erase(i);
            }
        }
    }
} // namespace

extern "C" UINT __stdcall CreateLayerLinks(MSIHANDLE handle)
{
    string sInstallPath = GetMsiProperty(handle, TEXT("INSTALLLOCATION"));

    string sOfficeInstallPath = sInstallPath;
    string sBasisInstallPath = sInstallPath + TEXT("Basis\\");
    string sUreInstallPath = sInstallPath + TEXT("URE\\");

    string sBasisLinkPath = sInstallPath + TEXT("basis-link");
    string sUreLinkPath = sInstallPath + TEXT("Basis\\ure-link");

    if ( IsSetMsiProperty(handle, TEXT("ADMININSTALL")) )
    {
        sBasisInstallPath = TEXT("Basis");
        sUreInstallPath = TEXT("..\\URE");
    }

    stripFinalBackslash(&sBasisInstallPath);
    stripFinalBackslash(&sUreInstallPath);

    // string myText1 = TEXT("Creating Basis-Link: ") + sBasisLinkPath;
    // string myText2 = TEXT("Creating Ure-Link: ") + sUreLinkPath;
    // MessageBox(NULL, myText1.c_str(), "DEBUG", MB_OK);
    // MessageBox(NULL, myText2.c_str(), "DEBUG", MB_OK);

    // creating basis-link in brand layer

    HANDLE h1file = CreateFile(
        sBasisLinkPath.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (IsValidHandle(h1file))
    {
        DWORD dummy;
        
        // Converting string into UTF-8 encoding and writing into file "basis-link"

        int nCharsRequired = MultiByteToWideChar( CP_ACP, 0, sBasisInstallPath.c_str(), -1, NULL, 0 );
        if ( nCharsRequired )
        {
            LPWSTR	lpPathW = new WCHAR[nCharsRequired];
            if ( MultiByteToWideChar( CP_ACP, 0, sBasisInstallPath.c_str(), -1, lpPathW, nCharsRequired ) )
            {
                nCharsRequired = WideCharToMultiByte( CP_UTF8, 0, lpPathW, -1, NULL, 0, NULL, NULL );
                if ( nCharsRequired )
                {
                    LPSTR	lpPathUTF8 = new CHAR[nCharsRequired];
                    WideCharToMultiByte( CP_UTF8, 0, lpPathW, -1, lpPathUTF8, nCharsRequired, NULL, NULL );

                    // WriteFile( h1file, sBasisInstallPath.c_str(), sBasisInstallPath.size() ,&dummy, 0 );
                    WriteFile( h1file, lpPathUTF8, strlen(lpPathUTF8) ,&dummy, 0 );

                    delete lpPathUTF8;
                }
            }
            
            delete lpPathW;
        }

        CloseHandle(h1file);
    }

    // creating ure-link in basis layer

    HANDLE h2file = CreateFile(
        sUreLinkPath.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (IsValidHandle(h2file))
    {
        DWORD dummy;

        // Converting string into UTF-8 encoding and writing into file "basis-link"

        int nCharsRequired = MultiByteToWideChar( CP_ACP, 0, sUreInstallPath.c_str(), -1, NULL, 0 );
        if ( nCharsRequired )
        {
            LPWSTR	lpPathW = new WCHAR[nCharsRequired];
            if ( MultiByteToWideChar( CP_ACP, 0, sUreInstallPath.c_str(), -1, lpPathW, nCharsRequired ) )
            {
                nCharsRequired = WideCharToMultiByte( CP_UTF8, 0, lpPathW, -1, NULL, 0, NULL, NULL );
                if ( nCharsRequired )
                {
                    LPSTR	lpPathUTF8 = new CHAR[nCharsRequired];
                    WideCharToMultiByte( CP_UTF8, 0, lpPathW, -1, lpPathUTF8, nCharsRequired, NULL, NULL );

                    // WriteFile( h2file, sUreInstallPath.c_str(), sUreInstallPath.size() ,&dummy, 0 );
                    WriteFile( h2file, lpPathUTF8, strlen(lpPathUTF8) ,&dummy, 0 );

                    delete lpPathUTF8;
                }
            }
            
            delete lpPathW;
        }

        CloseHandle(h2file);
    }

    return ERROR_SUCCESS;
}

extern "C" UINT __stdcall RemoveLayerLinks(MSIHANDLE handle)
{
    string sInstallPath = GetMsiProperty(handle, TEXT("INSTALLLOCATION"));

    string sOfficeInstallPath = sInstallPath;
    string sBasisInstallPath = sInstallPath + TEXT("Basis\\");
    string sUreInstallPath = sInstallPath + TEXT("URE\\");
	
    string sBasisLinkPath = sOfficeInstallPath + TEXT("basis-link");
    string sUreLinkPath = sBasisInstallPath + TEXT("ure-link");
    string sUreDirName = sUreInstallPath + TEXT("bin");

    // string myText2 = TEXT("Deleting Ure-Link: ") + sUreLinkPath;
    // MessageBox(NULL, myText2.c_str(), "DEBUG", MB_OK);

    // Deleting link to basis layer
    // string myText1 = TEXT("Deleting Basis-Link: ") + sBasisLinkPath;
    // MessageBox(NULL, myText1.c_str(), "DEBUG", MB_OK);
    DeleteFile(sBasisLinkPath.c_str());

    // Check, if URE is still installed
    bool ureDirExists = true;
    WIN32_FIND_DATA aFindData;
    HANDLE hFindContent = FindFirstFile( sUreDirName.c_str(), &aFindData );
    if ( hFindContent == INVALID_HANDLE_VALUE ) { ureDirExists = false; }
    FindClose( hFindContent );

    // if ( ureDirExists )
    // {
    //     string myText3 = TEXT("URE directory still exists: ") + sUreDirName;
    //     MessageBox(NULL, myText3.c_str(), "DEBUG", MB_OK);
    //     string myText4 = TEXT("URE link NOT removed: ") + sUreLinkPath;
    //     MessageBox(NULL, myText4.c_str(), "DEBUG", MB_OK);
    // }

    // Deleting link to URE layer, if URE dir no longer exists
    if ( ! ureDirExists )
    {
    //     string myText5 = TEXT("URE directory does not exist: ") + sUreDirName;
    //     MessageBox(NULL, myText5.c_str(), "DEBUG", MB_OK);
    	DeleteFile(sUreLinkPath.c_str());
    //     string myText6 = TEXT("URE link removed: ") + sUreLinkPath;
    //     MessageBox(NULL, myText6.c_str(), "DEBUG", MB_OK);
    }

    return ERROR_SUCCESS;
}

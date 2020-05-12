
#include "Stdafx.h"
#include "Common.hpp"

std::string GetInfo()
{

    TCHAR szVersionFile[MAX_PATH];
    GetModuleFileName(NULL, szVersionFile, MAX_PATH);

    DWORD  verHandle = 0;
    UINT   size      = 0;
    LPBYTE lpBuffer  = NULL;
    DWORD  verSize   = GetFileVersionInfoSize( szVersionFile, &verHandle);
    char Buffer[64];

    if (verSize != 0)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfo( szVersionFile, verHandle, verSize, verData))
        {
            if (VerQueryValue(verData,"\\",(VOID FAR* FAR*)&lpBuffer,&size))
            {
                if (size)
                {
                    VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        sprintf(Buffer, "%d.%d.%d",
                        int( verInfo->dwFileVersionMS >> 16 ) & 0xffff,
                        int( verInfo->dwFileVersionMS >>  0 ) & 0xffff,
                        int( verInfo->dwFileVersionLS >> 16 ) & 0xffff
                        );
                    }
                }
            }
        }

        delete[] verData;
    }

    return std::string(Buffer);
}

std::string FileOpen( HWND hWnd, char *Filter, char *InitPath, char *DefaultExt)
{

    OPENFILENAME ofn;
    char szFile[260];

    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = Filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrDefExt = DefaultExt;
    ofn.lpstrInitialDir = InitPath;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if( GetOpenFileName( &ofn ) == TRUE ) {
        return std::string( ofn.lpstrFile );
    } else {
        return "";
    }
}

std::string FileSave( HWND hWnd, char *Filter, char *InitPath, char *DefaultExt )
{
    OPENFILENAME ofn;
    char szFile[260];

    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = Filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrDefExt = DefaultExt;
    ofn.lpstrInitialDir = InitPath;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if( GetSaveFileName( &ofn ) == TRUE ) {
        return std::string( ofn.lpstrFile );
    } else {
        return "";
    }
}

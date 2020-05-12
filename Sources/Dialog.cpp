
#include "Stdafx.h"
#include "Dialog.hpp"
#include "Resource.h"

CAppDialog::CAppDialog(UINT nResID) : CDialog(nResID)
{

}

CAppDialog::~CAppDialog()
{

}

BOOL CAppDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch (nID)
    {
        case IDI_BROWSEIN:
            FileIn = FileOpen( NULL, (char*)("Obj Files\0*.obj\0Bin Files\0*.bin\0All Files\0*.*\0"), &std::string("\0")[0], &std::string(".obj")[0] );
            if( FileIn.size() != 0 ) {

                SetDlgItemTextA(IDI_INPUT, FileIn.c_str());

                if (strcasecmp(FileIn.substr(FileIn.find_last_of("."), 4).c_str(), ".bin") == 0) {
                    SetDlgItemTextA(IDI_OUTPUT, std::string(FileIn.substr(0, FileIn.find_last_of(".")) + ".obj").c_str());
                } else if (strcasecmp(FileIn.substr(FileIn.find_last_of("."), 4).c_str(), ".obj") == 0) {
                    SetDlgItemTextA(IDI_OUTPUT, std::string(FileIn.substr(0, FileIn.find_last_of(".")) + ".bin").c_str());
                }

            }
            break;

        case IDI_BROWSEOUT:
            FileOut = FileSave( NULL, (char*)("Bin Files\0*.bin\0Obj Files\0*.obj\0All Files\0*.*\0"), &std::string("\0")[0], &std::string(".bin")[0] );
            if( FileOut.size() != 0 ) {
                SetDlgItemTextA(IDI_OUTPUT, FileOut.c_str());

//                if (strcasecmp(FileOut.substr(FileOut.find_last_of("."), 4).c_str(), ".obj") == 0) {
//                    SetDlgItemTextA(IDI_INPUT, std::string(FileOut.substr(0, FileOut.find_last_of(".")) + ".bin").c_str());
//                } else if (strcasecmp(FileOut.substr(FileOut.find_last_of("."), 4).c_str(), ".bin") == 0) {
//                    SetDlgItemTextA(IDI_INPUT, std::string(FileOut.substr(0, FileOut.find_last_of(".")) + ".obj").c_str());
//                }

            }
            break;

        case IDI_ABOUT:
                MessageBox(std::string("Version - v" + GetInfo() + "\nMonvert © 2019, Natas8\nwww.Natas8.com\n\nMonvert Is A Simple Utility To Convert Between Monster Truck Madness Bin Models And Alias Wavefront Object Models.").c_str(), "About...", MB_OK | MB_ICONINFORMATION);
            break;

        case IDI_OK:
            if (GetDlgItemTextA(IDI_INPUT) != "" && GetDlgItemTextA(IDI_OUTPUT) != "") {

                FileIn = GetDlgItemTextA(IDI_INPUT);
                FileOut = GetDlgItemTextA(IDI_OUTPUT);

                Invert = IsDlgButtonChecked(IDI_INVERT);
                Scale = IsDlgButtonChecked(IDI_SCALE);
                Faces = SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_GETCURSEL, 0, 0);

                if (strcasecmp(FileIn.substr(FileIn.find_last_of("."), 4).c_str(), ".bin") == 0) {
                    Bin.Load(FileIn);
                } else if (strcasecmp(FileIn.substr(FileIn.find_last_of("."), 4).c_str(), ".obj") == 0) {
                    Obj.Load(FileIn);
                }

                if (strcasecmp(FileOut.substr(FileOut.find_last_of("."), 4).c_str(), ".bin") == 0) {
                    Obj.Export(FileOut, Invert, Scale, Faces);
                } else if (strcasecmp(FileOut.substr(FileOut.find_last_of("."), 4).c_str(), ".obj") == 0) {
                    Bin.Export(FileOut, Invert, Scale);
                }

                MessageBox("Converted Model!", "Success", MB_OK | MB_ICONINFORMATION);
            } else {
                MessageBox("Please Ensure Input & Output Filenames Are Valid", "Error", MB_OK | MB_ICONERROR);
            }

            break;

    }

    return FALSE;
}


INT_PTR CAppDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    }

    return DialogProcDefault(uMsg, wParam, lParam);
}

void CAppDialog::OnDestroy()
{

    char Result[ MAX_PATH ];
    std::string ConfigFile = std::string(Result, GetModuleFileName(NULL, Result, MAX_PATH));
    ConfigFile = ConfigFile.substr(0, ConfigFile.find_last_of("\\") + 1) + "Config.cfg";

    std::ofstream StreamOut(ConfigFile.c_str(), std::ios::out);

    if (!StreamOut.fail()) {

        FileIn = GetDlgItemTextA(IDI_INPUT);
        FileOut = GetDlgItemTextA(IDI_OUTPUT);
        Invert = IsDlgButtonChecked(IDI_INVERT);
        Scale = IsDlgButtonChecked(IDI_SCALE);
        Faces = SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_GETCURSEL, 0, 0);

        StreamOut << FileIn << std::endl;
        StreamOut << FileOut << std::endl;
        StreamOut << Invert << std::endl;
        StreamOut << Scale << std::endl;
        StreamOut << Faces << std::endl;

    }

    StreamOut.close();


    ::PostQuitMessage(0);
}

BOOL CAppDialog::OnInitDialog()
{
    SetIconLarge(IDI_ICON);
    SetIconSmall(IDI_ICON);

    SetWindowText(std::string("Monvert - v" + GetInfo()).c_str());

    SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_ADDSTRING, 0, (LPARAM) _T("0x19 Ignore Texture Coordinates (MTM)"));
    SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_ADDSTRING, 0, (LPARAM) _T("0x18 Normal Texture (MTM)"));
    SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_ADDSTRING, 0, (LPARAM) _T("0x29 Shiny Texture (MTM2)"));
    SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_ADDSTRING, 0, (LPARAM) _T("0x11 Transparent Color #0 (MTM)"));
    SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_ADDSTRING, 0, (LPARAM) _T("0x33 Transparent Color #0 (MTM2)"));

    char Result[ MAX_PATH ];
    std::string Path = std::string(Result, GetModuleFileName(NULL, Result, MAX_PATH));
    Path = Path.substr(0, Path.find_last_of("\\") + 1);

    std::ifstream StreamIn(std::string(Path + "Config.cfg").c_str(), std::ios::in);

    if (!StreamIn.fail()) {

        std::getline(StreamIn, FileIn);
        SetDlgItemTextA(IDI_INPUT, FileIn.c_str());

        std::getline(StreamIn, FileOut);
        SetDlgItemTextA(IDI_OUTPUT, FileOut.c_str());

        StreamIn >> Invert;
        CheckDlgButton(IDI_INVERT, Invert);

        StreamIn >> Scale;
        CheckDlgButton(IDI_SCALE, Scale);

        StreamIn >> Faces;
        SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_SETCURSEL, Faces, 0);

    } else {

        SetDlgItemTextA(IDI_INPUT, std::string(Path + "Models\\").c_str());
        SetDlgItemTextA(IDI_OUTPUT, std::string(Path + "Models\\").c_str());
        CheckDlgButton(IDI_INVERT, true);
        CheckDlgButton(IDI_SCALE, true);
        SendMessage(::GetDlgItem(GetHwnd(), IDI_FACETYPE), CB_SETCURSEL, 2, 0);

    }

    StreamIn.close();

    return TRUE;
}

CApp::CApp() : Dialog(IDD_DIALOG)
{
}

CApp::~CApp()
{
}

BOOL CApp::InitInstance()
{
    Dialog.Create();

    return TRUE;
}

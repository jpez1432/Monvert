

#include "Stdafx.h"
#include "Dialog.hpp"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{

    try
    {
        CApp App;

        return App.Run();
    }

    catch (const CException &e)
    {
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return EXIT_FAILURE;
    }
}


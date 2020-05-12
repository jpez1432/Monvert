
#ifndef DIALOG_HPP
#define DIALOG_HPP

#include "Common.hpp"
#include "Resource.h"

#include "CBin.hpp"
#include "CObj.hpp"

class CAppDialog : public CDialog
{
public:
    CAppDialog(UINT nResID);
    virtual ~CAppDialog();

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:

    CBin Bin;
    CObj Obj;

    std::string FileIn;
    std::string FileOut;
    bool Invert;
    bool Scale;
    int Faces;

};

class CApp : public CWinApp
{
public:
    CApp();
    virtual ~CApp();
    virtual BOOL InitInstance();
    CAppDialog& GetDialog() {return Dialog;}

private:
    CAppDialog Dialog;
};

inline CApp& GetDlgApp() { return static_cast<CApp&>(GetApp()); }



#endif

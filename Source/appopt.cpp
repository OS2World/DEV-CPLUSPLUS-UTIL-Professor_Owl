/*
  (c) Copyright 2005, Edward J. March Jr.
  under the terms of the GNU/GPL License
  see license.txt file for legal details.
  Professor Owl was compiled under OS/2
  Warp 3.0 using Borland C++ Version 2.0
*/
#define     NULL 0
#include    <os2.h>
#include    <owl\checkbox.h>
#include    <owl\edit.h>
#include    "owl\dialog.h"
#include    <stdio.h>
#include    "resource.h"
#include    "appopt.h"
#include    "create1.h"
#include    "warnbeep.h"
#include    "startapp.h"
#include    "def.h"


//----------------------------------
// MESSAGE MAP FOR AppOpt
//----------------------------------
DEFINE_RESPONSE_TABLE1(AppOpt,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_OPTDIALOG,EvClickOptmaindialog),
    EV_BN_CLICKED(ID_OPTMENU,EvClickOptmenu),
    EV_BN_CLICKED(ID_OPTMDI,EvClickOptmdi),
    EV_BN_CLICKED(ID_OPTSTATUSBAR,EvClickOptstatusbar),
    EV_BN_CLICKED(ID_OPTSDI,EvClickSdi),
    EV_EN_CHANGE(ID_FILE_APP,EvChangeFileApp),
    EV_EN_CHANGE(ID_FILE_WINDOW,EvChangeFileWindow),
    EV_EN_CHANGE(ID_FILE_ABOUT,EvChangeFileAbout),
    EV_EN_CHANGE(ID_FILE_RES,EvChangeFileRes),
    EV_BN_CLICKED(ID_DEFAULT,EvClickDefault),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


extern char MyProjectPath[MAXPATHSIZE];


//----------------------------------
// Constructor
//----------------------------------
AppOpt::AppOpt(TWindow *parent) : TDialog(parent, ID_APPOPT)
{
    m_FileRes = new TEdit(this,ID_FILE_RES);
    m_FileAbout = new TEdit(this,ID_FILE_ABOUT);
    m_FileWindow = new TEdit(this,ID_FILE_WINDOW);
    m_FileApp = new TEdit(this,ID_FILE_APP);
}





//----------------------------------
// Destructor
//----------------------------------
AppOpt::~AppOpt()
{
    delete m_FileRes;
    delete m_FileAbout;
    delete m_FileWindow;
    delete m_FileApp;
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL AppOpt::EvInitDialog(HWND hWndFocus)
{
    BOOL r = TDialog::EvInitDialog(hWndFocus);

    char s[MAXPATHSIZE+12];
    sprintf(s,"Path: %s",MyProjectPath);
    SetDlgItemText(ID_INFO,s);

    CheckDlgButton(ID_OPTSDI,TRUE);
   EvClickDefault();
   //
   return r;
}

//----------------------------------
// Cancel Button
//----------------------------------
void AppOpt::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void AppOpt::EvClickOk(void)
{
    int Flags = 0;
    if(IsDlgButtonChecked(ID_OPTDIALOG) )
        Flags |= NEWAPPFLAG_DIALOG;
    else if(IsDlgButtonChecked(ID_OPTSDI) )
        Flags |= NEWAPPFLAG_SDI;
    else if(IsDlgButtonChecked(ID_OPTMDI) )
        Flags |= NEWAPPFLAG_MDI;

    if(IsDlgButtonChecked(ID_OPTMENU) )
        Flags |= NEWAPPFLAG_MENU;

    if(IsDlgButtonChecked(ID_OPTCONTROLBAR) )
        Flags |= NEWAPPFLAG_CONTROLBAR;

    if(IsDlgButtonChecked(ID_OPTSTATUSBAR) )
        Flags |= NEWAPPFLAG_STATUSBAR;

    char FRes[32+1];
    char FAbout[32+1];
    char FMain[32+1];
    char FApp[32+1];

    m_FileRes->GetText(FRes,sizeof(FRes));
    m_FileAbout->GetText(FAbout,sizeof(FAbout));
    m_FileWindow->GetText(FMain,sizeof(FMain));
    m_FileApp->GetText(FApp,sizeof(FApp));

    CreateApplication(this,Flags,FApp,FMain,FAbout,FRes);
    CmOk();
}


void AppOpt::EnableChildWithId(int Id, int enable)
{
    ::WinEnableWindow(GetDlgItem(Id), enable);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_OPTMAINDIALOG
//----------------------------------
void AppOpt::EvClickOptmaindialog(void)
{
}


//----------------------------------
// Event EV_BN_CLICKED for ID_OPTMENU
//----------------------------------
void AppOpt::EvClickOptmenu(void)
{
}


//----------------------------------
// Event EV_BN_CLICKED for ID_OPTMDI
//----------------------------------
void AppOpt::EvClickOptmdi(void)
{
}


//----------------------------------
// Event EV_BN_CLICKED for ID_OPTSTATUSBAR
//----------------------------------
void AppOpt::EvClickOptstatusbar(void)
{
    // INSERT>> Your Code Here!
}


//----------------------------------
// Event EV_BN_CLICKED for ID_SDI
//----------------------------------
void AppOpt::EvClickSdi(void)
{
}



//----------------------------------
// Event EV_EN_CHANGE for ID_FILE_APP
//----------------------------------
void AppOpt::EvChangeFileApp(void)
{
}


//----------------------------------
// Event EV_EN_CHANGE for ID_FILE_WINDOW
//----------------------------------
void AppOpt::EvChangeFileWindow(void)
{
}


//----------------------------------
// Event EV_EN_CHANGE for ID_FILE_ABOUT
//----------------------------------
void AppOpt::EvChangeFileAbout(void)
{
}


//----------------------------------
// Event EV_EN_CHANGE for ID_FILE_RES
//----------------------------------
void AppOpt::EvChangeFileRes(void)
{
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DEFAULT
//----------------------------------
void AppOpt::EvClickDefault(void)
{
    m_FileRes->SetText("RESOURCE.H");
    m_FileAbout->SetText("ABOUT.CPP");
    m_FileWindow->SetText("MAINWIN.CPP");
    m_FileApp->SetText("MYAPP.CPP");
}





//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void AppOpt::EvClickHelp(void)
{
    Help("Create New Application");
}


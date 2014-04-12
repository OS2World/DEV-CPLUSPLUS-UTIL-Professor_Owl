/*
  (c) Copyright 2005, Edward J. March Jr.
  under the terms of the GNU/GPL License
  see license.txt file for legal details.
  Professor Owl was compiled under OS/2
  Warp 3.0 using Borland C++ Version 2.0
*/
#include    "def.h"
#define INCL_WINSHELLDATA
#define INCL_WINSYS
#include <os2.h>
//
#include    <owl\owlpch.h>
#include    <owl\applicat.h>
#include    <owl\framewin.h>
#include    <owl\owlpch.h>
#include    "resource.h"
#include    <owl\dialog.h>
#include    <owl\menu.h>
#include    <owl\edit.h>
#include    <owl\listbox.h>
#include    <owl\editfile.h>
#include    <owl\combobox.h>
#include    <owl\static.h>
#pragma hdrstop
#include    "main.h"
#include    "dlgmsg.h"
#include    <owl\framewin.h>
#include    <owl\decframe.h>    // decorated
#include    <owl\controlb.h>    // decorated frame control bar
#include    <owl\buttonga.h>    // decorated frame control bar
#include    <owl\statusba.h>    // decorated frame control bar
#include    <stdio.h>
#include    "prjpath.h"
#include    <owl\point.h>       // Desktop Window size
#include    <owl\window.h>      // Desktop Window size

char MyHomePath[MAXPATHSIZE]; // as c:\\dir1\dir2
char MyName[MAXPATHSIZE]; // as "POWL.EXE"


TStatusBar *Sbar;

void StatusBar(char *s)
{
    static char prev[128+1];
    static char current[128+1];
    if(s == NULL)
    {
        Sbar->SetText(prev);
    }
    else
    {
        strcpy(prev,current);
        Sbar->SetText(s);
        strcpy(current,s);
    }
}

TDecoratedFrame *MainFramePtr;


void ShowStatusBar(void)
{
    static BOOL last = 0xABCD;
    BOOL sh = MainFramePtr->IsIconic();
    if(sh)
    {
        sh = SW_HIDE;
    }
    else
    {
        sh = SW_SHOW;
    }
    ////
    if(last != sh)
    {
        Sbar->ShowWindow(sh);
        last = sh;
    }
}



class TMyApp : public TApplication
{
  public:
    TMyApp() : TApplication() {}
    void InitMainWindow();
protected:
    void InitInstance(void);
};



void TMyApp::InitMainWindow()
{
    int ResIdNum = ID_DLGMSG1000;   // Hi Res Dialog
    int h,w;
    GetDefaultWH(w,h);
    if(h <= 480)
    {
        ResIdNum = ID_DLGMSG640x480;
    }
    ///
    DlgMsg *Dw = new DlgMsg((TWindow*)0, ResIdNum);
    MainFramePtr = new TDecoratedFrame(0, ".", Dw );
    const int small = 6;
    const big = 20;
    TControlBar *Cbar = new TControlBar(MainFramePtr);
    Cbar->Insert(*new TSeparatorGadget(big) ); // left margin on tool bar
    Cbar->Insert(*new TButtonGadget(CM_BM_RESCAN,ID_RESCAN,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(big) );
    Cbar->Insert(*new TButtonGadget(CM_BM_SETUP,ID_SETUP,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(small) );
    Cbar->Insert(*new TButtonGadget(CM_BM_PROJECTWIN,CM_OPENPROJECTWIN,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(small) );
    Cbar->Insert(*new TButtonGadget(CM_BM_PRJPATH,ID_PATH,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(big) );
    Cbar->Insert(*new TButtonGadget(CM_BM_NEWCLASS,ID_ADD_CLASS,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(big) );
    Cbar->Insert(*new TButtonGadget(CM_BM_VARIABLE,ID_VARIABLE,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(big) );
    Cbar->Insert(*new TButtonGadget(CM_BM_ADDFUNCTION,ID_ADD_EVENT,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(small) );
    Cbar->Insert(*new TButtonGadget(CM_BM_DELFUNCTION,ID_DEL_MEMBER,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(big) );
    Cbar->Insert(*new TButtonGadget(CM_BM_TOOLS,ID_TOOLS,TButtonGadget::Command) );
    Cbar->Insert(*new TSeparatorGadget(small) );
    Cbar->Insert(*new TButtonGadget(CM_BM_ABOUT,CM_ABOUT,TButtonGadget::Command) );
    MainFramePtr->Insert(*Cbar, TDecoratedFrame::Top);

    Sbar = new TStatusBar(MainFramePtr,TGadget::Embossed, TStatusBar::CapsLock | TStatusBar::NumLock | TStatusBar::Overtype);
    MainFramePtr->Insert(*Sbar, TDecoratedFrame::Bottom);

    SetMainWindow( MainFramePtr );

    MainWindow->AssignMenu(ID_MAINMENU);
    MainWindow->SetIcon(this,ICON2);
    MainWindow->Attr.Style &= ~WS_MAXIMIZEBOX;

    RecallWindowSizePos();
    RecallProjectPath(NULL,TRUE);
}



void RecallWindowSizePos(void)
{
    int deskw,deskh;
    GetDefaultWH(deskw,deskh);
    char ws[32+1];
    char hs[32+1];
    sprintf(ws,"%d",deskw);
    sprintf(hs,"%d",deskh);
    ///
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    char s[MAXPATHSIZE];
    s[0] = NULL;
    PrfQueryProfileString(hini,"profsowl","X","16",s,32);
    int x = atoi(s);
    PrfQueryProfileString(hini,"profsowl","Y","3",s,32);
    int y = atoi(s);
    PrfQueryProfileString(hini,"profsowl","Width",ws,s,32);
    int w = atoi(s);
    PrfQueryProfileString(hini,"profsowl","Height",hs,s,32);
    int h = atoi(s);
    PrfCloseProfile(hini);
    //
    // IF Something Valid To Recall !
    //
    if(x >= 0 && x < 1000 && y >= 0 && y < 1000)
    {
        MainFramePtr->MoveWindow(x,y,w,h,TRUE);
    }
}

void GetDefaultWH(int &w,int &h)
{
    RECTL Tr;
    WinQueryWindowRect(HWND_DESKTOP,&Tr);
    int dw = Tr.xRight - Tr.xLeft;
    int dh = Tr.yTop - Tr.yBottom;

    if(dw <= 640)
    {
        w = 585;
        h = 480;
    }
    else if(dw == 800)
    {
        w = 580;
        h = 550;
    }
    else if(dw == 1024)
    {
        w = 780;    //OK
        h = 665;
    }
    else if(dw == 1152)
    {
        w = 870;    //OK
        h = 705;
    }
    else if(dw == 1280)
    {
        w = 870;    //OK
        h = 705;
    }
    else
    {
        w = dw;
        h = dh;
    }
}


void SaveWindowSizePos(void)
{
    int x = MainFramePtr->Attr.X;
    int y = MainFramePtr->Attr.Y;

    SWP swp;
    WinQueryWindowPos(MainFramePtr->HWindow, &swp);
    int w = swp.cx;
    int h = swp.cy;

    static int oldx, oldy, oldw, oldh;
    ///
    if(x == oldx && y == oldy && w == oldw && h == oldh)
    {
        return;
    }
    ////
    oldx = x;
    oldy = y;
    oldw = w;
    oldh = h;
    ////
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    if(!hini)
        return;

    char s[32+1];
    s[0] = NULL;
    sprintf(s,"%d",x);
    PrfWriteProfileString(hini,"profsowl","X",s);
    sprintf(s,"%d",y);
    PrfWriteProfileString(hini,"profsowl","Y",s);
    sprintf(s,"%d",w);
    PrfWriteProfileString(hini,"profsowl","Width",s);
    sprintf(s,"%d",h);
    PrfWriteProfileString(hini,"profsowl","Height",s);
    PrfCloseProfile(hini);
}



int OwlMain(int argc, char *argv[])
{
    strcpy(MyHomePath,argv[0]);
    char *p = strchr(MyHomePath,'\\');
    char *q = p;;
    while( p && *p)
    {
        q = p;  // last found backslash
        p = p+1;
        if(*q)
            p = strchr(p,'\\');
    }
    *q = NULL;  // MyHomePath = "C:\thisdir\andthahtdir\sub1" // cut off "\powl.exe"
    strcpy(MyName,q+1);
    return TMyApp().Run();
}


//----------------------------------
// Virtual Function
//----------------------------------
void TMyApp::InitInstance(void)
{
    TApplication::InitInstance();
}




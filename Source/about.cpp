/*
  (c) Copyright 2005, Edward J. March Jr.
  under the terms of the GNU/GPL License
  see license.txt file for legal details.
  Professor Owl was compiled under OS/2
  Warp 3.0 using Borland C++ Version 2.0
*/

#include    "def.h"
#include    <owl\dialog.h>
#include    <owl\buttonga.h>
#include    <owl\static.h>
#include    "resource.h"
#include    "version.h"
#include    "about.h"
#include    "owl\edit.h"



//----------------------------------
// MESSAGE MAP FOR About
//----------------------------------
DEFINE_RESPONSE_TABLE1(About,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
About::About(TWindow *parent) : TDialog(parent, ID_ABOUT)
{
    m_HomeDir = new TStatic(this,ID_HOMEDIR);
    m_Version = new TStatic(this,ID_VERSION);
}


//----------------------------------
// Destructor
//----------------------------------
About::~About()
{
    delete m_HomeDir;
    delete m_Version;
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL About::EvInitDialog(HWND hWndFocus)
{
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
    extern char MyHomePath[];
    m_HomeDir->SetText(MyHomePath);
    m_Version->SetText(PROFESSOR_OWL_VERSION);
    return RetValue;
}


//----------------------------------
// Cancel Button Pressed
//----------------------------------
void About::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void About::EvClickOk(void)
{
    CmOk();
}





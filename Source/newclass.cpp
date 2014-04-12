#include    "def.h"
#define INCL_WINSTDFILE         // FILEDLG FileOpen Window Standard File Functions
#include    <os2.h>

#include    "resource.h"
#include    <owl\dialog.h>
#include    <owl\combobox.h>
#include    <owl\edit.h>
#pragma hdrstop
#include    <process.h>
#include    "newclass.h"
#include    <stdio.h>
#include    <dir.h>
#include    "cl.h"
#include    "dlgid.h"
#include    "create1.h"
#include    "parse.h"
#include    "warnbeep.h"
#include    "startapp.h"
#include    "projFile.h"
#include    "projWin.h"


DEFINE_RESPONSE_TABLE1(NewClass, TDialog)
    EV_BN_CLICKED(IDOK, EvClickOk),
    EV_BN_CLICKED(IDCANCEL, EvClickCancel),
    EV_EN_CHANGE(ID_CLASS_NAME,EvClassChg),
    EV_EN_CHANGE(ID_HFILE,EvHfileChg),
    EV_EN_CHANGE(ID_CFILE,EvCfileChg),
    EV_CBN_SELCHANGE(ID_BASE_LIST,EvChgBaseList),
    EV_WM_PAINT,
    EV_BN_CLICKED(ID_RESCAN,EvClickRescan),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


NewClass::NewClass(TWindow *parent) : TDialog(parent, ID_DLGNEWCLASS)
{
    m_Base      = new TComboBox(this,ID_BASE_LIST);
    m_Class     = new TEdit(this,ID_CLASS_NAME);
    m_Hfile     = new TEdit(this,ID_HFILE);
    m_Cfile     = new TEdit(this,ID_CFILE);
    m_Ident     = new TComboBox(this,ID_IDENT);
}

NewClass::~NewClass(void)
{
    delete m_Base;
    delete m_Class;
    delete m_Hfile;
    delete m_Cfile;
    delete m_Ident;
}


BOOL NewClass::EvInitDialog(HWND hWndFocus)
{
    m_Scan = TRUE;
    OutStr[0] = NULL;
    TDialog::EvInitDialog(hWndFocus);
    return 0;
}

//----------------------------------
// Event EV_WM_PAINT for ID_DLGNEWCLASS
//----------------------------------
void NewClass::EvPaint(void)
{
    TDialog::EvPaint();
    if(m_Scan)
    {
        m_Scan = FALSE;
        Rescan();
        for(int j=0; j<m_Ident->GetCount(); j++)
        {
            char s[MAXPATHSIZE];
            m_Ident->GetString(s,j);
            if(strstr(s," ?") )
            {
                m_Ident->SetSelIndex(j);
                break;
            }
        }
    }
}

void NewClass::Rescan(void)
{
    char *bc[] ={
        "TButton",
        "TCheckBox",
        "TComboBox",
        "TControl",
        "TControlBar",
        "TDialog",          // #5
        "TDocManager",      // 1.03a
        "TDocument",
        "TEdit",
        "TEditFile",
        "TEditSearch",
        "TEditView",
        "TFileDocument",
        "TFloatingFrame",
        "TGadgetWindow",
        "TGauge",
        "TListBox",
        "TLayoutWindow",
        "TListView",
        "TMDIChild",
        "TMessageBar",
        "TRadioButton",
        "TScrollBar",
        "TStatic",
        "TStatusBar",
        "TTinyCaption",
        "TToolBox",
        "TView",
        "TWindow",
        "TWindowView",
        NULL
    };
    SetCursor(0,IDC_WAIT);
    //
    // Add Unused Dialog ID_'s to list
    //
    m_Ident->ClearList();
    m_Ident->ShowList(TRUE);    // WHILE BUSY...
    //
    DlgId X;
    //
    int use_wildmask = TRUE;
    ProjFile Project;
    while( ! Project.IsEof() )
    {
        char fpath[MAXPATHSIZE];
        Project.GetFile(fpath);
        char ThePath[MAXPATHSIZE];
        char TheFile[MAXPATHSIZE];
        PathSplit(ThePath,TheFile,fpath);
        if(strstr(TheFile, ".rc" ) != NULL)
        {
            X.Scanmask(ThePath, TheFile, m_Ident);
            use_wildmask = FALSE;
        }
    }
    Project.Close();

    if(use_wildmask)
    {
        X.Scanmask(".\\", "*.rc",m_Ident);
    }

    m_Ident->SetSelIndex(0);
    m_Base->ClearList();
    for(int i=0; bc[i]; i++)
    {
        m_Base->InsertString((const char far *)bc[i], -2);
    }
    m_Base->SetSelIndex(m_Base->FindString("TDialog",-1));
    m_Ident->ShowList(FALSE);
    SetCursor(0,IDC_ARROW);
}


void NewClass::EvClassChg(void)
{
    int bad = FALSE;
    char s[128+1];
    m_Class->GetText(s,sizeof(s));
    s[32]=NULL;

    for(int i=0; s[i]; i++)
    {
        if(
            (i == 0 && !isalpha(s[0]) && s[i] != '_' )
            ||
            (i > 0 && !isalpha(s[i]) && !isdigit(s[i]) && s[i] != '_')
            )
        { // illegal char found in class name - remove it
            for(int j=i; s[j]; j++)
                s[j] = s[j+1];
            bad = TRUE;
        }
    }
    if(bad)
        m_Class->SetText(s);

    char *p = strchr(s,'.');
    if(p)
        *p = NULL;

    strlwr(s);
    strcat(s,".h");
    m_Hfile->SetText(s);
}


void NewClass::EvHfileChg(void)
{
    char s[MAXPATHSIZE];
    char ss[MAXPATHSIZE];
    m_Hfile->GetText(s,sizeof(s));
    strcpy(ss,s);
    char *p = strchr(s,'.');
    if(p)
        *p = NULL;
    //
    strcat(s,".h");
    if(strcmp(ss,s) != 0)
    {
        m_Hfile->SetText(s);
    }
    if(p)
        *p = NULL;
    strcat(s,".cpp");
    m_Cfile->SetText(s);
}

void NewClass::EvCfileChg(void)
{
    char s[MAXPATHSIZE];
    char ss[MAXPATHSIZE];
    m_Cfile->GetText(s,sizeof(s));
    strcpy(ss,s);
    char *p = strchr(s,'.');
    if(p)
        *p = NULL;
    strcat(s,".cpp");
    if(strcmp(ss,s) != 0)
    {
        m_Cfile->SetText(s);
    }
}

extern ProjWin *ProjectWindowPtr;


void NewClass::EvClickOk(void)
{
    Parser();
    //
    // CREATE CLASS BUTTON IF ALL IS WELL
    //
    char s[MAXPATHSIZE];
    int er = 0;
    m_Cfile->GetText(s,sizeof(s));
    if(strstr(s,".c") == 0 || strlen(s) < 3)
        er = 1;

    m_Hfile->GetText(s,sizeof(s));
    if(strstr(s,".h") == 0 || strlen(s) < 3)
       er = 2;

    m_Class->GetText(s,sizeof(s));
    if(strlen(s) < 2)
        er = 3;
    if( ValidateSymbol(s) )
        er = 3;
    //
    if( strcmp(BaseClass,"TDialog") == 0 && MyId[0] == NULL)
        er = 4;

    char *msg = NULL;
    switch(er)
    {
        case 1: msg = "Bad C++ Ffile Name"; break;
        case 2: msg = "Bad Header File Name";   break;
        case 3: msg = "Bad Class Name";     break;
        case 4: msg = "Invalid ID_ constant for Dialog Box\n"
                      "Select a valid ID or Use Resoure Workshop\n"
                      "To create a New DLGTEMPLATE with a new ID";
        break;
    }

    if(msg)
    {
        WarnBeep();
        MessageBox(msg,"CAN NOT CREATE CLASS",MB_CANCEL);
        return;
    }
    PrettyPathFile(Hfile);
    PrettyPathFile(Cfile);

    SetCursor(0,IDC_WAIT);
    if( strcmp(BaseClass,"TDialog") == 0)
    {
        CreateClassTDialog(Hfile,Cfile,MyClass,MyId,IdHfile);
    }
    else if( strcmp(BaseClass,"TWindow") == 0)
    {
        CreateClassTWindow(Hfile,Cfile,MyClass,IdHfile);
    }
    else
    {
        CreateClassTgeneric(Hfile,Cfile,MyClass,BaseClass,MyId,IdHfile);
    }
    sprintf(OutStr,"%s : %s ( %s %s )",MyClass,BaseClass,Hfile,Cfile);
    SetCursor(0,IDC_ARROW); // 1.07

    ProjFile Project;
    if( ! Project.IsEof() ) // If there is a project.owl file !!
    {
        if( MessageBox("Would you Like To Append "
                   "These Files To The Project.Owl File?",
                    "UPDATE PROJECT.OWL",MB_YESNO) == IDYES)
        {
            // If HFILE or Cfile has no path prefix with CWD
            char thisdir[MAXPATHSIZE];
            getcwd(thisdir,sizeof(thisdir));        // save directory we are in
            char hfilename[MAXPATHSIZE];
            char cfilename[MAXPATHSIZE];
            strcpy(hfilename,Hfile);
            strcpy(cfilename,Cfile);
            if(strstr(Hfile,"\\") == NULL)
                sprintf(hfilename,"%s\\%s",thisdir,Hfile);
            if(strstr(Cfile,"\\") == NULL)
                sprintf(cfilename,"%s\\%s",thisdir,Cfile);
            ///
            PrettyPathFile(hfilename);
            PrettyPathFile(cfilename);
            Project.AppendString(hfilename,cfilename);
            //
            if( ProjectWindowPtr )  // Project Window is Open - reload it!
            {
                ProjectWindowPtr->ReloadFile();
            }
        }
    }
    CmOk();
}



void NewClass::EvClickCancel(void)
{
    CmCancel();
}

void NewClass::EvChgBaseList(void)
{
    char s[128+1];
    s[0] = NULL;
    m_Base->GetSelString(s,sizeof(s)-1);
    int t = strcmp(s,"TDialog")==0;
    m_Ident->EnableWindow(t);
}



void NewClass::Parser(void)
{
    MyId[0] = IdHfile[0] = IdCfile[0] = NULL;
    //
    char s[sizeof(MyId)];
    m_Base->GetSelString(BaseClass,sizeof(BaseClass)-1);
    m_Class->GetText(MyClass,sizeof(MyClass)-1);
    m_Hfile->GetText(Hfile,sizeof(Hfile)-1);
    m_Cfile->GetText(Cfile,sizeof(Cfile)-1);
    m_Ident->GetSelString(s,sizeof(s)-1);
    sscanf(s,"%s ( %s %s )",MyId,IdHfile,IdCfile);
}


void NewClass::Output(char *s)
{
    strcpy(s,OutStr);   // return "MyClass : Base ( hfile.h cfile.cpp )"
}


//----------------------------------
// Event EV_BN_CLICKED for ID_RESCAN
//----------------------------------
void NewClass::EvClickRescan(void)
{
    Rescan();
}




//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void NewClass::EvClickHelp(void)
{
    Help("Create New Class");
}




#define INCL_WINSHELLDATA
#define     NULL    0
#include    <os2.h>

#include    <io.h>
#include    <stdio.h>
#include    "def.h"
#include    "resource.h"
#include    "Comment.h"
#include    "setupdat.h"
#include    "startapp.h"
#include    "warnbeep.h"
#include    "parse.h"
#include    "startapp.h"

extern SETUPDATA    UserSetup;

//----------------------------------
// MESSAGE MAP FOR DlgComment
//----------------------------------
DEFINE_RESPONSE_TABLE1(DlgComment,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_EXAMPLE,EvClickExample),
    EV_BN_CLICKED(ID_EDITH,EvClickEdith),
    EV_BN_CLICKED(ID_EDITC,EvClickEditc),
    EV_BN_CLICKED(ID_SAMPLEHEAD,EvClickSamplehead),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
DlgComment::DlgComment(TWindow *parent) : TDialog(parent, ID_DLGCOMMENT)
{
    // INSERT>> Your Code Here
    m_FuncDelimeter = new TEdit(this,ID_FUNC_DELIMETER);
    m_CfileHead = new TEdit(this,ID_CFILE_HEAD);
    m_HfileHead = new TEdit(this,ID_HFILE_HEAD);
    m_Commentflag = new TEdit(this,ID_COMMENTFLAG);
}



//----------------------------------
// Destructor
//----------------------------------
DlgComment::~DlgComment()
{
    // INSERT>> Your Code Here
    delete m_FuncDelimeter;
    delete m_CfileHead;
    delete m_HfileHead;
    delete m_Commentflag;
}




//----------------------------------
// Init Dialog
//----------------------------------
BOOL DlgComment::EvInitDialog(HWND hWndFocus)
{
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
    //
    SetDlgItemText(ID_COMMENTFLAG,UserSetup.CommentFlag);
    SetDlgItemText(ID_FUNC_DELIMETER,UserSetup.FuncDelimeter);
    SetDlgItemText(ID_HFILE_HEAD,UserSetup.HFileHead);
    SetDlgItemText(ID_CFILE_HEAD,UserSetup.CFileHead);
    //
    return(RetValue);
}




//----------------------------------
// Cancel Button Pressed
//----------------------------------
void DlgComment::EvClickCancel(void)
{
    CmCancel();
}

#define BIG_DASH    "//-------------------------------------------------------------------------"


//----------------------------------
// OK Button Pressed
//----------------------------------
void DlgComment::EvClickOk(void)
{
    GetDlgItemText(ID_COMMENTFLAG,UserSetup.CommentFlag,sizeof(UserSetup.CommentFlag));
    GetDlgItemText(ID_FUNC_DELIMETER,UserSetup.FuncDelimeter,sizeof(UserSetup.FuncDelimeter));
    //
    GetDlgItemText(ID_HFILE_HEAD,UserSetup.HFileHead,sizeof(UserSetup.HFileHead));
    GetDlgItemText(ID_CFILE_HEAD,UserSetup.CFileHead,sizeof(UserSetup.CFileHead));
    //
    SaveCommentSettings();
    CmOk();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_EXAMPLE
//----------------------------------
void DlgComment::EvClickExample(void)
{
    SetDlgItemText(ID_COMMENTFLAG,"// INSERT>> Your Code Here");
    SetDlgItemText(ID_FUNC_DELIMETER,BIG_DASH);
    SetDlgItemText(ID_HFILE_HEAD,".\\_head_.h");
    SetDlgItemText(ID_CFILE_HEAD,".\\_head_.cpp");
}


extern char MyHomePath[];


//----------------------------------
//  LoadCommentSettings
//----------------------------------
void LoadCommentSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    //
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    ///
    PrfQueryProfileString(hini,"profsowl","CommentFlag","// INSERT>> Your Code Here",UserSetup.CommentFlag,sizeof(UserSetup.CommentFlag));

    PrfQueryProfileString(hini,"profsowl","FuncDelimeter",BIG_DASH,UserSetup.FuncDelimeter,sizeof(UserSetup.FuncDelimeter));

    PrfQueryProfileString(hini,"profsowl","HFileHead",
    ".\\_head_.h",UserSetup.HFileHead,sizeof(UserSetup.HFileHead));

    PrfQueryProfileString(hini,"profsowl","CFileHead",
    ".\\_head_.cpp",UserSetup.CFileHead,sizeof(UserSetup.CFileHead));

    PrfCloseProfile(hini);
}


//----------------------------------
//  SaveCommentSettings
//----------------------------------
void SaveCommentSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    //
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    //
    PrfWriteProfileString(hini,"profsowl","CommentFlag",UserSetup.CommentFlag);
    PrfWriteProfileString(hini,"profsowl","FuncDelimeter",UserSetup.FuncDelimeter);
    PrfWriteProfileString(hini,"profsowl","HFileHead",UserSetup.HFileHead);
    PrfWriteProfileString(hini,"profsowl","CFileHead",UserSetup.CFileHead);
    PrfCloseProfile(hini);
}



//----------------------------------
// Event EV_BN_CLICKED for ID_EDITH
//----------------------------------
void DlgComment::EvClickEdith(void)
{
    char HFile[MAXPATHSIZE];
    GetDlgItemText(ID_HFILE_HEAD,HFile,sizeof(HFile));
    EditFile(HFile);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_EDITC
//----------------------------------
void DlgComment::EvClickEditc(void)
{
    char CFile[MAXPATHSIZE];
    GetDlgItemText(ID_CFILE_HEAD,CFile,sizeof(CFile));
    EditFile(CFile);
}

void EditFile(char *fname)
{
    char title[300+1];
    sprintf(title,"%s (Professor-Owl)",fname);
    char dir_parm[400+1];
    strcpy(dir_parm,fname);
    // find last back slash in path ... and NULL it out
    // to make "D:\dir\subdir\file.txt" into  two strings "D:\dir\subdir" "file.txt"
    char Dir[MAXPATHSIZE];
    char Parm[MAXPATHSIZE];
    strcpy(Dir,".\\");
    //
    char *p = dir_parm;
    char *p1;
    while( *p && (p1=strchr(p,'\\')) != NULL)
    {
        p = ++p1;
    }
    if(*p == '\\')
    {
        *p++= NULL;
    }
    strcpy(Parm,p);
    if(p != dir_parm)
    {
        strcpy(Dir,dir_parm);
    }
    StartApp(0, title, Dir, "E.exe", Parm);
}



//----------------------------------
// Event EV_BN_CLICKED for ID_SAMPLEHEAD
//----------------------------------
void DlgComment::EvClickSamplehead(void)
{
    char HFile[MAXPATHSIZE];
    char CFile[MAXPATHSIZE];
    GetDlgItemText(ID_HFILE_HEAD,HFile,sizeof(HFile));
    GetDlgItemText(ID_CFILE_HEAD,CFile,sizeof(CFile));
    /////
    char bkufile[MAXPATHSIZE];
    //////
    char msg[MAXPATHSIZE];
    sprintf(msg,"Create Sample %s File ?",HFile);
    if( MessageBox(msg,"MAKE SAMPLE HEADER FILE",MB_YESNOno) == IDYES)
    {
        strcpy(bkufile,HFile);
        ForceExt(bkufile,UserSetup.EdBak);
        rename(HFile,bkufile);
        FILE *fo = fopen(HFile,"wt");
        if(!fo)
        {
            WarnBeep();
            sprintf(msg,"Error %d creating %s",HFile,errno);
            MessageBox(msg,"CAN NOT CREATE FILE");
        }
        else
        {
            fprintf(fo,"//===============================================\n");
            fprintf(fo,"// This header file was created on $Time\n");
            fprintf(fo,"// Using $product\n");
            fprintf(fo,"// This file has prototypes for functions\n");
            fprintf(fo,"// contained in $File.CPP\n");
            fprintf(fo,"// $file.h and $file.cpp are in $path\n");
            fprintf(fo,"// The class is $class and the base class is $base\n");
            fprintf(fo,"//===============================================\n\n\n");
            fclose(fo);
        }
    }
    ////--------------
    sprintf(msg,"Create Sample %s File ?",CFile);
    if( MessageBox(msg,"MAKE SAMPLE HEADER FILE",MB_YESNOno) == IDYES)
    {
        strcpy(bkufile,CFile);
        ForceExt(bkufile,UserSetup.EdBak);
        rename(CFile,bkufile);
        FILE *fo = fopen(CFile,"wt");
        if(!fo)
        {
            WarnBeep();
            sprintf(msg,"Error %d creating %s",CFile,errno);
            MessageBox(msg,"CAN NOT CREATE FILE");
        }
        else
        {
            fprintf(fo,"//===============================================\n");
            fprintf(fo,"// $path\\$file.cpp created $TIME\n");
            fprintf(fo,"//\n");
            fprintf(fo,"// Using $product\n");
            fprintf(fo,"// Class Name $class derived from $base\n");
            fprintf(fo,"// Prototypes are in $file.h \n");
            fprintf(fo,"//===============================================\n");
            fprintf(fo,"//\n");
            fprintf(fo,"//\n");
            fprintf(fo,"//\n");
            fclose(fo);
        }
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void DlgComment::EvClickHelp(void)
{
    Help("Comments");
}


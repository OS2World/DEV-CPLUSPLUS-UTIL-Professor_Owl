//===============================================
//
// This Header File Was Created On mon jan 29 22:30:02 1996
// This File Has Prototypes For Functions
// Contained In projwin.Cpp
//
// The Class is ProjWin and the base class is TDialog
// Author Edward J. March Jr.
//===============================================
#include    "def.h"
#define     INCL_WINSTDFILE         // FILEDLG FileOpen Window Standard File Functions
#define     INCL_WINMENUS           // for popup menu
#define     INCL_WINFRAMEMGR        // for wm_querytrackinfo
#include    <os2.h>
//
#include    <stdio.h>
#include    "dlgmsg.h"
#include    "resource.h"
#include    "projwin.h"
#include    "startapp.h"
#include    "parse.h"
#include    "tools.h"
#include    "setupdat.h"
#include    "makedlg.h"
#include    "parse.h"
#include    "warnbeep.h"
#include    "projfile.h"
#include    "settool.h"


extern SETUPDATA    UserSetup;
extern char MyProjectPath[];


static int LastRunTool;

//
//----------------------------------
// MESSAGE MAP FOR ProjWin
//----------------------------------
DEFINE_RESPONSE_TABLE1(ProjWin,TDialog)
    EV_LBN_DBLCLK(ID_PROJECTFILES,EvDblClickProjectfiles),
    EV_BN_CLICKED(ID_EDIT,EvClickEdit),
    EV_BN_CLICKED(ID_ADD,EvClickAdd),
    EV_BN_CLICKED(ID_DELETE,EvClickDelete),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
    EV_BN_CLICKED(ID_SAVEINI,EvClickSave),
    EV_BN_CLICKED(ID_CREATEMAKEFILE,EvClickCreatemakefile),
    EV_BN_CLICKED(ID_UNDELETE,EvClickUndelete),
    EV_WM_RBUTTONUP,
    EV_COMMAND(CM_RUNTOOL1,EvCmRuntool1),
    EV_COMMAND(CM_RUNTOOL2,EvCmRuntool2),
    EV_COMMAND(CM_RUNTOOL3,EvCmRuntool3),
    EV_COMMAND(CM_RUNTOOL4,EvCmRuntool4),
    EV_COMMAND(CM_RUNTOOL5,EvCmRuntool5),
    EV_COMMAND(CM_RUNTOOL6,EvCmRuntool6),
    EV_COMMAND(CM_RUNTOOL7,EvCmRuntool7),
    EV_COMMAND(CM_RUNTOOL8,EvCmRuntool8),
    EV_COMMAND(CM_TOOLSETUP,EvCmToolsetup),
END_RESPONSE_TABLE;

extern DlgMsg *TheDlgMsg;

//----------------------------------
// Constructor
//----------------------------------
ProjWin::ProjWin(TWindow *parent) : TDialog(parent, ID_PROJECTWINDOW)
{
    m_Projectfiles = new TListBox(this,ID_PROJECTFILES);
    for(int i=0; i<MAXUNDELETE; i++)
    {
        Undelete[i] = NULL;
    }
    UndeleteIndex = 0;
    RightMenu = new TPopupMenu;
    if(LastRunTool < 1 || LastRunTool > 8)
    {
        LastRunTool = 1;
    }
}


//----------------------------------
// Destructor
//----------------------------------
ProjWin::~ProjWin()
{
    for(int i=0; i<MAXUNDELETE; i++)
    {
        delete Undelete[i];
    }
    UndeleteIndex = 0;
    delete m_Projectfiles;
    delete RightMenu;
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL ProjWin::EvInitDialog(HWND hWndFocus)
{
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
    WinSetParent(HWindow, HWND_DESKTOP,0);  //Must Be Here too

    ReloadFile();
    return(RetValue);
}

//---------------------------------------------------------------
// RE-LOAD FILE -- called by NewClass When Files are created !
//---------------------------------------------------------------
void ProjWin::ReloadFile(void)
{
    ProjFile Project;
    m_Projectfiles->ClearList();
    char s[MAXPATHSIZE];

    while( ! Project.IsEof() )
    {
        Project.GetFile(s);
        if(s[0] && s[1] == ':')
        {
            if(m_Projectfiles->FindString(s,-1) < 0)
            {
                m_Projectfiles->AddString(s);
            }
        }
    }
    m_Projectfiles->AddString("{end-of-list}");
    //
    // Title Of Project File
    //
    sprintf(s,"%s\\Project.OWL",MyProjectPath);
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    dirty = FALSE;
    // MAKE BUTTON TEXT NUMBER LAST RUN TOOL #
    char ButtonText[128+1];
    sprintf(ButtonText,"Run ~Tool #%d",LastRunTool);
    SetDlgItemText(ID_EDIT,ButtonText);
}



//----------------------------------
// Save Button Pressed
//----------------------------------
void ProjWin::EvClickSave(void)
{
    FILE *fo = fopen("PROJECT.OWL","wt");
    if(fo)
    {
        fprintf(fo,"// Professor Owl Project File -- Edit With Care\n");
        fprintf(fo,"// Each Line Must Have A Drive:\(path | file)\n");
        for(int i=0; i<m_Projectfiles->GetCount(); i++)
        {
            char s[MAXPATHSIZE];
            m_Projectfiles->GetString(s,i);
            fprintf(fo,"%s\n",s);
        }
        fclose(fo);
        dirty = FALSE;
        TheDlgMsg->EvClickRescan();
    }
}



//----------------------------------
// Event EV_LBN_DBLCLK for ID_PROJECTFILES
//----------------------------------
void ProjWin::EvDblClickProjectfiles(void)
{
    EvClickEdit();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_EDIT
//----------------------------------
void ProjWin::EvClickEdit(void)
{
    RunTool(LastRunTool);
}


//----------------------------------------------------------------------------
// Event EV_BN_CLICKED for ID_ADD - Add one or many files from FileOpen Dialog
//----------------------------------------------------------------------------
void ProjWin::EvClickAdd(void)
{
    PAPSZ pp;
    PAPSZ pp1;
    PSZ s;
    pp = FileOpen();
    pp1 = pp;
    while(pp && (s = *pp[0]) != NULL)
    {
        PrettyPathFile(s);
        pp++;
        if(m_Projectfiles->FindString(s,-1) < 0)
        {
            int i = m_Projectfiles->GetSelIndex();
            if(i < 0)
                i = m_Projectfiles->GetCount()-1;

            if(IsDlgButtonChecked(ID_SORTED) )
                m_Projectfiles->InsertString(s,-2);
            else
                m_Projectfiles->InsertString(s,i);

            dirty = TRUE;
        }
    }
    if(pp1)
    {
        WinFreeFileDlgList(pp1);
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DELETE
//----------------------------------
void ProjWin::EvClickDelete(void)
{
    int i = m_Projectfiles->GetSelIndex();
    char s[MAXPATHSIZE];
    s[0] = NULL;
    m_Projectfiles->GetString(s,i);
    //
    if(i >= 0 && s[0] != '{')
    {
        if(UndeleteIndex < MAXUNDELETE )
        {
            Undelete[UndeleteIndex] = new char[strlen(s)+1];
            strcpy(Undelete[UndeleteIndex++],s);
        }
        m_Projectfiles->DeleteString(i);
        m_Projectfiles->SetSelIndex(i);
        dirty = TRUE;
    }
    else
    {
        WarnBeep();
        MessageBox("First Select An Entry To Delete","Warning");
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void ProjWin::EvClickHelp(void)
{
    Help("Project Window");
}


//----------------------------------
// Virtual Function
//----------------------------------
void ProjWin::Destroy(int retVal)
{
    if(dirty)
    {
        int r = MessageBox( "Save Changes to PROJECT.OWL File?",
                            "CONFIRM SAVE/DISCARD CHANGES",MB_ICONSTOP|MB_YESNO);
        if(r == IDYES)
        {
            EvClickSave();
        }
        dirty = FALSE;
    }
    TDialog::Destroy(retVal);
    TheDlgMsg->PostMessage(WM_COMMAND,CM_NOTIFY_PRJWIN_CLOSE);
}




//----------------------------------
//  'User Function' private: FileOpen
//----------------------------------
PAPSZ ProjWin::FileOpen(void)
{
    static char Directory[MAXPATHSIZE];
    /*****************************************************************/
    /* Initially set all fields to 0                                 */
    /*****************************************************************/
    static FILEDLG fild;                                    // File dialog info structure
    char pszFullFile[CCHMAXPATH];
    strcpy(pszFullFile,Directory);
    strcat(pszFullFile,"*.c*;*.h*;*.rc;*.inc");             // File filter string
    memset(&fild, 0, sizeof(FILEDLG));

    /*****************************************************************/
    /* Initialize those fields in the FILEDLG structure that are     */
    /* used by the application                                       */
    /*****************************************************************/
    fild.cbSize = sizeof(FILEDLG);            // Size of structure
    fild.fl = FDS_CENTER | FDS_OPEN_DIALOG |FDS_MULTIPLESEL;

                                                    // FDS_* flags
    fild.pszTitle = "Add Path/File To Project List";// Dialog title string
    strcpy(fild.szFullFile, pszFullFile);           // Initial path,
                                                    // file name, or
                                                    // file filter

    /*****************************************************************/
    /* Display the dialog and get the file                           */
    /*****************************************************************/
    HWND  hwndDlg = WinFileDlg(HWND_DESKTOP, HWindow, &fild);
    if (hwndDlg && (fild.lReturn == DID_OK))
    {
        char dummy[MAXPATHSIZE];
        PathSplit(Directory,dummy,fild.szFullFile);   // remember directory
        return( fild.papszFQFilename );
    }
    return(NULL);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_CREATEMAKEFILE
//----------------------------------
void ProjWin::EvClickCreatemakefile(void)
{
    MakeDlg Md(this,m_Projectfiles);
    Md.Execute();
}

//----------------------------------
// Event EV_BN_CLICKED for ID_UNDELETE
//----------------------------------
void ProjWin::EvClickUndelete(void)
{
    if(UndeleteIndex <= 0)
    {
        WarnBeep();
        MessageBox("Nothing To Un-Delete","Warning");
        return;
    }
    int i = m_Projectfiles->GetSelIndex();
    if(i < 0)
        i = m_Projectfiles->GetCount()-1;

    if(IsDlgButtonChecked(ID_SORTED) )
        i = -2;
    if(UndeleteIndex > 0 && Undelete[UndeleteIndex-1] != NULL)
    {
        UndeleteIndex--;
        m_Projectfiles->InsertString(Undelete[UndeleteIndex],i);
        delete Undelete[UndeleteIndex];
        Undelete[UndeleteIndex] = NULL;
    }
    dirty = TRUE;
}

extern TOOLNAMEPATH UserTools[8+1];

//---------------------------------------------------------------
// Event EV_WM_RBUTTONUP for ID_PROJECTWINDOW
//---------------------------------------------------------------
void ProjWin::EvRButtonUp(UINT modKeys, TPoint& point)
{
    TDialog::EvRButtonUp(modKeys, point);

    if(!RightMenu)
        return;

    int i = m_Projectfiles->GetSelIndex();
    char s[MAXPATHSIZE];
    s[0] = NULL;
    m_Projectfiles->GetString(s,i);
    //
    if(i < 0 || s[0] == '{')
    {
        s[0] = NULL;  // Still allow tool to run - might be a simple OS/2 Shell!
    }

    //////////////
    int Cm[] = { 0,
                 CM_RUNTOOL1,CM_RUNTOOL2,CM_RUNTOOL3,
                 CM_RUNTOOL4,CM_RUNTOOL5,CM_RUNTOOL6,
                 CM_RUNTOOL7,CM_RUNTOOL8 };

    for(int t=1; t<=8; t++)
    {
        if(UserTools[t].Name[0] && UserTools[t].Path[0])
        {
            int j = Cm[t];
            RightMenu->DeleteMenu(j,MF_BYCOMMAND);
            char outs[512+1];
            ExpandToolDollar(UserTools[t].Name, outs, s,s, 1);
            //////
            int chk = MF_UNCHECKED;
            if(t == LastRunTool)
            {
                chk = MF_CHECKED;
            }
            RightMenu->InsertMenu(-1,chk | MF_BYPOSITION,j, outs);
        }
    }
    RightMenu->DeleteMenu(CM_TOOLSETUP,MF_BYCOMMAND);
    RightMenu->AppendMenu(MF_BYPOSITION,CM_TOOLSETUP,"Tools Setup...");
    RightMenu->TrackPopupMenu(TPM_LEFTALIGN | PU_KEYBOARD | PU_MOUSEBUTTON1,point,HWindow);
}



//---------------------------------------------------------------
//  'User Function' private: RunTool
//---------------------------------------------------------------
void ProjWin::RunTool(int N)
{
    int i = m_Projectfiles->GetSelIndex();
    char s[MAXPATHSIZE];
    s[0] = NULL;
    m_Projectfiles->GetString(s,i);
    //
    if(i >= 0 && s[0] != '{')
    {
        // MAKE BUTTON TEXT NUMBER LAST RUN TOOL #
        char ButtonText[128+1];
        sprintf(ButtonText,"Run ~Tool #%d",LastRunTool = N);
        SetDlgItemText(ID_EDIT,ButtonText);
        ///
        TheDlgMsg->SaveMLEfile();
        Tools X(this,N);
        X.Set(s,s,N);
        if( X.Execute() == IDOK && UserSetup.RescanTool )
        {
            TheDlgMsg->EvClickRescan();
        }
    }
    else
    {
        WarnBeep();
        MessageBox("First Select An Entry","Warning");
    }
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL1
//---------------------------------------------------------------
void ProjWin::EvCmRuntool1(void)
{
    RunTool(1);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL2
//---------------------------------------------------------------
void ProjWin::EvCmRuntool2(void)
{
    RunTool(2);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL3
//---------------------------------------------------------------
void ProjWin::EvCmRuntool3(void)
{
    RunTool(3);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL4
//---------------------------------------------------------------
void ProjWin::EvCmRuntool4(void)
{
    RunTool(4);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL5
//---------------------------------------------------------------
void ProjWin::EvCmRuntool5(void)
{
    RunTool(5);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL6
//---------------------------------------------------------------
void ProjWin::EvCmRuntool6(void)
{
    RunTool(6);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL7
//---------------------------------------------------------------
void ProjWin::EvCmRuntool7(void)
{
    RunTool(7);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_RUNTOOL8
//---------------------------------------------------------------
void ProjWin::EvCmRuntool8(void)
{
    RunTool(8);
}


//---------------------------------------------------------------
// Event EV_COMMAND for CM_TOOLSETUP
//---------------------------------------------------------------
void ProjWin::EvCmToolsetup(void)
{
    SetTool S(this);
    S.Execute();
}




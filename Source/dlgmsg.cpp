#include    "def.h"
#define     INCL_WINSHELLDATA
#define     INCL_WINMESSAGEMGR
#define     INCL_WINMENUS
#define     INCL_WINFRAMEMGR    // for wm_querytrackinfo
#define     INCL_WINSTDFILE     // FILEDLG FileOpen Window Standard File Functions#define NULL 0
#include    <os2.h>
#include    "dlgmsg.h"
#include    "resource.h"
#include    <process.h>
#include    <stdio.h>
#include    <sys\stat.h>
#include    "newclass.h"
#include    "ob.h"
#include    "cl.h"
#include    "reskid.h"
#include    "bigmess.h"
#include    "edfunc.h"
#include    "msgscan.h"
#include    "parse.h"
#include    "vardlg.h"
#include    "create1.h"
#include    "appopt.h"
#include    "tools.h"
#include    "setup.h"
#include    "misc.h"
#include    "startapp.h"
#include    "setupdat.h"
#include    "prjpath.h"
#include    "vfun.h"
#include    <dir.h>
#include    <owl\buttonga.h>
#include    <owl\decframe.h>    // decorated frame ptr
#include    "about.h"
#include    "userfunc.h"
#include    "version.h"
#include    "warnbeep.h"
#include    "main.h"
#include    "input.h"
#include    "projwin.h"
#include    "ProjFile.h"
#include    "Browse.h"

extern SETUPDATA    UserSetup;
extern TDecoratedFrame *MainFramePtr;

#define USERFUNCTION    "User Function"

#define MAXHISTORY  20
#define MAXSTRLEN   80
static char *HistoryUserData[MAXHISTORY];
static char HistoryUserDataBuffer[MAXHISTORY][MAXSTRLEN+1];

char MyProjectPath[MAXPATHSIZE];

DlgMsg *TheDlgMsg;

//
DlgMsg::DlgMsg(TWindow *parent, int ResIdNum) : TDialog(parent, ResIdNum)
{
    TheDlgMsg = this;
    m_HasBegun = 0;
    m_Class     = new TComboBox(this,ID_CLASS);     // list of files
    m_Objects   = new TListBox(this,ID_OBJECTS);    // Called 'Items'
    m_Event     = new TListBox(this,ID_EVENT);      // Functions/Events
    m_Member    = new TEdit(this,ID_MEMBER);        // Editor
    m_Member->isMulti = TRUE;
    m_EdFile    = new TStatic(this,ID_FILENAME);    // Name of file
    m_IgnoreEventChange = FALSE;
    m_Member->EmptyUndoBuffer();
    m_Member->ClearModify();
    m_MleFile[0] = NULL;
    m_ModeFun = TRUE;
    for(int i=0; i<MAXHISTORY; i++)
    {
        HistoryUserData[i] = HistoryUserDataBuffer[i];
    }
}



DlgMsg::~DlgMsg(void)
{
    delete m_Class;
    delete m_Objects;
    delete m_Event;
    delete m_Member;
    delete m_EdFile;
}



DEFINE_RESPONSE_TABLE1(DlgMsg, TDialog)
    EV_BN_CLICKED(IDCANCEL, EvClickCancel),
    EV_BN_CLICKED(ID_ADD_CLASS, EvClickAddClass),
    EV_BN_CLICKED(ID_TOOLS, EvClickTools),
    EV_BN_CLICKED(ID_RESCAN, EvClickRescan),
    EV_BN_CLICKED(ID_ADD_EVENT, EvClickAddEvent),
    EV_CBN_SELCHANGE(ID_CLASS,EvClass),
    EV_LBN_SELCHANGE(ID_OBJECTS,EvObject),
    EV_LBN_SELCHANGE(ID_EVENT,EvEvent),
    EV_BN_CLICKED(ID_DEL_MEMBER,EvClickDelEvent),
    EV_BN_CLICKED(ID_VARIABLE,EvClickVariable),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
    EV_WM_PAINT,
    EV_BN_CLICKED(ID_SETUP,EvClickSetup),
    EV_WM_RBUTTONUP,
//right menu
    EV_COMMAND(CM_SCAN,EvClickRescan),
    EV_COMMAND(CM_ADDCLASS,EvClickAddClass),
    EV_COMMAND(CM_VARIABLES,EvClickVariable),
    EV_COMMAND(CM_ADDEVENT,EvClickAddEvent),
    EV_COMMAND(CM_DELEVENT,EvClickDelEvent),
    EV_COMMAND(CM_FINDOBJECTS,EvCmFindobjects),
    EV_COMMAND(CM_FINDEVENT,EvCmFindevent),
    EV_COMMAND(CM_FINDMEMBER,EvCmFindmember),
    EV_COMMAND(CM_GOTOEVENT,EvCmGotoevent),
    EV_COMMAND(CM_TOOLS,EvClickTools),
    EV_COMMAND(CM_SAVEMLEFILE,EvClickSaveMLEfile),
    EV_LBN_DBLCLK(ID_EVENT,EvDblClickEvent),
    EV_BN_CLICKED(ID_PATH,EvClickPath),
    EV_COMMAND(CM_CUT,EvCmCut),
    EV_COMMAND(CM_COPY,EvCmCopy),
    EV_COMMAND(CM_PASTE,EvCmPaste),
    EV_COMMAND(CM_DEL,EvCmDel),
    EV_COMMAND(CM_UNDO,EvCmUndo),
// main menu
    EV_COMMAND(CM_PATH,EvClickPath),
    EV_COMMAND(CM_SETUP,EvClickSetup),
    EV_COMMAND(CM_ABOUT,EvCmAbout),
    EV_COMMAND_ENABLE(CM_SAVEMLEFILE,EvCeSavemlefile),
    EV_COMMAND(CM_CREATEAPPLICATION,EvCmCreateapplication),
    EV_COMMAND(CM_HELPINDEX,EvCmHelpindex),
    EV_COMMAND(CM_HELPGENERAL,EvCmHelpgeneral),
    EV_COMMAND(CM_FINDDELETED,EvCmFinddeleted),
    EV_COMMAND(CM_FINDCOMMENTFLAG,EvCmFindcommentflag),
    EV_BN_CLICKED(ID_SHOWCONST,EvClickShowconst),
    EV_COMMAND(CM_WINDOWSAVESIZEPOS,EvCmSavesizepos),
    EV_COMMAND(CM_WINDOWDEFAULTSIZEPOS,EvCmWindowdefault),
    EV_COMMAND(CM_WINDOWRECALLSIZEPOS,EvCmWindowrecallsizepos),
    EV_COMMAND(CM_OPENPROJECTWIN,EvCmOpenprojectwin),
    EV_COMMAND_ENABLE(CM_OPENPROJECTWIN,EvCeOpenprojectwin),
    EV_COMMAND(CM_NOTIFY_PRJWIN_CLOSE,EvCmNotify_prjwin_close),
    EV_COMMAND(CM_BROWSE,EvCmBrowse),
    EV_WM_CLOSE,
END_RESPONSE_TABLE;

BOOL DlgMsg::EvInitDialog(HWND hWndFocus)
{
    BOOL b = TDialog::EvInitDialog(hWndFocus);
    RightMenu.AppendMenu(MF_BYPOSITION,CM_ADDCLASS,"~New Class...");
    RightMenu.AppendMenu(MF_BYPOSITION,CM_VARIABLES,"~Variables...");
    RightMenu.AppendMenu(MF_BYPOSITION,CM_ADDEVENT,"~Add Event/Function");
    RightMenu.AppendMenu(MF_BYPOSITION,CM_DELEVENT,"~Del Event/Function...");
    RightMenu.AppendMenu(MF_BYPOSITION,CM_TOOLS,"User ~Tools...");
    RightMenu.AppendMenu(MF_SEPARATOR);
    //
    EdMenu.AppendMenu(MF_BYPOSITION,CM_ADDCLASS,"~New Class...");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_VARIABLES,"~Variables...");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_ADDEVENT,"~Add Event/Function");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_DELEVENT,"~Del Event/Function...");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_TOOLS,"User ~Tools...");
    EdMenu.AppendMenu(MF_SEPARATOR);
    EdMenu.AppendMenu(MF_BYPOSITION,CM_FINDMEMBER,"~Search...");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_FINDDELETED,"Fin~d ' //DELETE_BEGIN'");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_FINDCOMMENTFLAG,"~Find Comment Flag");

    EdMenu.AppendMenu(MF_BYPOSITION,CM_GOTOEVENT,"~Go To Reference...");
    EdMenu.AppendMenu(MF_SEPARATOR);
    EdMenu.AppendMenu(MF_BYPOSITION,CM_COPY,"~Copy\tShift+Del");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_CUT,"Cu~t\tCtrl+Ins");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_DEL,"C~lear\tCtrl+Del");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_PASTE,"~Paste\tShift+Ins");
    EdMenu.AppendMenu(MF_BYPOSITION,CM_UNDO,"U~ndo");
    EdMenu.AppendMenu(MF_SEPARATOR);
    //
    m_Member->EmptyUndoBuffer();
    m_Member->ClearModify();
    //
    return b;
}


//----------------------------------
// Event EV_WM_PAINT for ID_DLGMSG1000
//----------------------------------
void DlgMsg::EvPaint(void)
{
    TDialog::EvPaint();
    if(m_HasBegun == 0)
    {
        m_HasBegun = 1;
    }
    ShowStatusBar();
}


void DlgMsg::Begin(void)
{
    if(m_HasBegun != 1)
        return;

    m_HasBegun = 2;

    DoTitle();

    if( NeedSetup() )
    {
        WarnBeep();
        MessageBox( "Setup Has Not Been\n"
                    "Run With This Version\n"
                    "Of Professor OWL\n\n"
                    "You Will Be Put Into\n"
                    "The Setup Options Screen\n"
                    "Press Save When Done\n\n",
        "SETUP OPTIONS NEEDED");
        EvClickSetup();
    }
    LoadSettings();
    CheckDlgButton(ID_SHOWCONST,UserSetup.ShowConstants);
    ///
    EvClickRescan();

    ////
    char cfs[128+1];
    RecallClassFile(cfs);
    int ci = 0;
    ci = m_Class->FindString(cfs,-1);
    if(ci < 0)
        ci = 0;
    m_Class->SetSelIndex(ci);
    /////

    char line[MAXPATHSIZE];
    line[MAXPATHSIZE];
    strcpy(line,TestNewAppNeeded());
    if(line[0]==NULL && !NeedSetup() )
    {
        AppOpt X(this);
        X.Execute();
        EvClickRescan();
    }
}


//----------------------------------
// Event For RESCAN BUTTON
//----------------------------------
void DlgMsg::EvClickRescan(void)
{
    SaveMLEfile();  // save changes for rescan
    //
    SetCursor(0,IDC_WAIT);

    int ci = m_Class->GetSelIndex();
    char bookmark[MAXPATHSIZE];
    bookmark[0] = NULL;
    if(ci >= 0)
    {
        m_Class->GetSelString(bookmark,sizeof(bookmark));
    }
    m_Class->SetSelIndex(-1);   // so next setsel index will cause change event
    m_Class->ClearList();
    m_Class->Clear();   // clear item in window too!
    //
    m_Objects->ClearList();
    m_Event->ClearList();
    //
    m_Member->Clear();
    m_Member->ClearModify();
    //
    m_Class->ShowList(TRUE);
    Ob X;

    // if PROJECT.OWL file exists -- USE IT! else use WILD MASKS!
    int use_wildmask = TRUE;
    ProjFile Project;
    while( ! Project.IsEof() )
    {
        char fpath[MAXPATHSIZE];
        Project.GetFile(fpath);
        if(fpath[0])
        {
            StatusBar(fpath);
            char ThePath[MAXPATHSIZE];
            char TheFile[MAXPATHSIZE];
            PathSplit(ThePath,TheFile,fpath);
            X.Scanmask(ThePath,TheFile, m_Class);
            use_wildmask = FALSE;
        }
    }
    Project.Close();

    if(use_wildmask)
    {
        StatusBar("Working... (Scanning Files)");
        X.Scanmask(".\\", "*.h",m_Class);
        X.Scanmask(".\\", "*.rh",m_Class);
        X.Scanmask(".\\", "*.rch",m_Class);
        X.Scanmask(".\\", "*.cpp",m_Class);
        X.Scanmask(".\\", "*.c",m_Class);
        X.Scanmask(".\\", "*.hpp",m_Class);
        X.Scanmask(".\\", "*.inl",m_Class);
        X.Scanmask(".\\", "*.inc",m_Class);
    }

    Ready();
    m_Class->ShowList(FALSE);

    // Remember last selected index
    ci = 0;
    if(bookmark[0])
    {
        ci = m_Class->FindString(bookmark,-1);
        if(ci < 0)
            ci = 0;
        m_Class->SetSelIndex(ci);
    }
    SetCursor(0,IDC_ARROW);
}


//----------------------------------
// Different Class Selected From DropDown List
//----------------------------------
void DlgMsg::EvClass(void)
{
    m_IgnoreEventChange = FALSE;
    //
    parMyClass[0] = parBaseClass[0]  = NULL;
    parHfile[0] = parCfile[0] = parID[0] = NULL;
    parChildId[0] = parCtrlType[0] = NULL;
    //
    char s[128+1];
    s[0] = NULL;
    m_Class->GetSelString(s,sizeof(s)-1);
    sscanf(s,"%s : %s ( %s %s ) id=%s",parMyClass,parBaseClass,parHfile,parCfile,parID);
    if(parID[0])
    {
        sprintf(s,"%s %s",parID,parBaseClass);
    }
    else
    {
        sprintf(s,"%s %s",parMyClass,parBaseClass);
    }
    m_Objects->ClearList();     // 2nd string must be base class for parser
    m_Objects->AddString("Virtual Functions");       // 1st show virtual functions then events
    m_Objects->AddString(s);    // ID_NAME BASECLASS or MyClass BaseClass
    //
    // add child controls if DLG BOX
    //
    SaveMLEfile();  // a modifed class my be in here!! for next scan below
    //
    UserSetup.ShowConstants = IsDlgButtonChecked(ID_SHOWCONST);
    ResKid X;
    // if PROJECT.OWL file exists -- USE IT! else use WILD MASKS!
    int use_wildmask = TRUE;
    ProjFile Project;
    while( ! Project.IsEof() )
    {
        char fpath[MAXPATHSIZE];
        Project.GetFile(fpath);
        if(fpath[0])
        {
            use_wildmask = FALSE;
            StatusBar(s);
            char ThePath[MAXPATHSIZE];
            char TheFile[MAXPATHSIZE];
            PathSplit(ThePath,TheFile,fpath);
            if(strstr(TheFile,".rc") != NULL)
                X.Scanmask(ThePath,TheFile,parID,m_Objects, UserSetup.ShowConstants );
            else
                X.ScanCommands(ThePath,TheFile,m_Objects);
        }
    }
    Project.Close();

    if(use_wildmask)
    {
        StatusBar("Scanning for ID_'s constants in *.RC Files...");
        X.Scanmask(".\\", "*.rc",parID,m_Objects, UserSetup.ShowConstants );
        // SCAN FOR MENU COMMANDS in this .C file and all .H files
        StatusBar("Scanning for CM_'s");
        X.ScanCommands(".\\",parCfile,m_Objects);
        X.ScanCommands(".\\","*.h",m_Objects);
        X.ScanCommands(".\\","*.hpp",m_Objects);
        X.ScanCommands(".\\","*.rh",m_Objects);
    }

    // Borland PreDefined CM_'s too!
    X.ScanCommands(UserSetup.IncludePath,"pmstd*.h",m_Objects);
    sprintf(s,"%s\\owl",UserSetup.IncludePath);
    X.ScanCommands(s,"*.rh",m_Objects);
    //
    m_Objects->SetSelIndex(0);
    //
    LoadMLEfile(parCfile);
}


//----------------------------------
// Object's List Changed
//----------------------------------
void DlgMsg::EvObject(void)
{
    char s[128+1];
    s[0] = NULL;
    m_Objects->GetSelString(s,sizeof(s)-1);
    //
    m_ModeFun = FALSE;
    m_Event->ClearList();
    if(m_Objects->GetSelIndex() == 0 && strncmp(s,"Virtual Functions",9) == 0)
    {
        m_ModeFun = TRUE;
        StatusBar("Looking Up Virtual Functions...");
        m_Event->AddString(USERFUNCTION);
        AddVirtualFunctionMess(parBaseClass,m_Event);
    }
    else if(s[0] != ' ') // skip over " <path> "
    {
        StatusBar("Looking Up Events...");
        sscanf(s,"%s %s",parChildId,parCtrlType);
        AddEventTypeMess(parCtrlType,m_Event);
    }
    Ready();
}


//----------------------------------
// Event/Function Selection changed
// See If It In Member Text
//----------------------------------
void DlgMsg::EvEvent(void)
{
    if(m_IgnoreEventChange)
        return;
    //
    char fname2[MAXPATHSIZE];
    char s[MAXPATHSIZE];
    char s1[MAXPATHSIZE];
    char m[MAXPATHSIZE];
    char line[MAXPATHSIZE+2];
    m_Event->GetSelString(s,sizeof(s)-1);
    ///
    if(m_ModeFun)
    {   // --- FUNCTION MODE ---
        if(s[0] == ' ') // COMMENT?
            return;
        //
        char *p;
        if(strcmp(s,USERFUNCTION) == 0)
        {
            strcpy(m," 'User Function'"); // Find one of these !
            if( PositionEditorSearch(m,TRUE) < 0 )
            {
                sprintf(s,"NOT IN TEXT: %s",m);
                StatusBar(s);
                return;
            }
            Ready();
            return;
        }
        else
        {
            p = strchr(s,' ');
            if(!p)
                return;
            char sRet[80+1];
            char sFun[MAXPATHSIZE];
            char sArg[MAXPATHSIZE];
            ParseRetFunArg(p, sRet, sFun, sArg);
            sprintf(m,":%s(%s)",sFun,sArg);
        }
        int funfound = GrepFiles(TRUE,NULL,NULL,parMyClass,"::",m,parCfile,fname2,line);
        if(!funfound)
        {
            sprintf(s,"NOT IN FILE %s",m);
            StatusBar(s);
            return;
        }
        if( PositionEditorSearch(line) < 0 )
        {
            sprintf(s,"NOT In Editor Text: %s",line);
            StatusBar(s);
        }
        else
        {
            Ready();
        }
        return;
    }
    ///
    ///   EVENTS
    ///
    if(strncmp(s,"EV_WM_",6) != 0)  // Need to build String With CHILD LD_name
    {   //  --- EVENT MODE ----
        char sym[80+1];
        GetThisSymbol(s,sym);
        char sym2[80+1];
        strcpy(sym2,sym);
        strcat(sym2,"(");   // macroname(
        if(!GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",sym2,",",parChildId,parCfile,fname2,line))
        {
            strcpy(sym2,sym);
            strcat(sym2," (");  // check for space paren!
            if( GrepFiles(FALSE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",sym2,",",parChildId,parCfile,fname2,line) )
            {
                WarnBeep();
                MessageBox("Error 5003 Should Never Happen","IMPOSSIBLE");
            }
        }

        if(line[0] == NULL)
        {
            char *altId = NULL;
            if(strcmp(parChildId,"DID_CANCEL") == 0)
                altId = "IDCANCEL";
            else if(strcmp(parChildId,"DID_OK") == 0)
                altId = "IDOK";
            // LOOK ALTERNATE ID name  DID_CANCEL / DID_OK -->> IDCACEL, IDOK
            if(altId)
            {
                GrepFiles(FALSE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",sym,",",altId,parCfile,fname2,line);
            }
            // Found text in code as "EV_BN_CLICKED(ID_BUT1,EvClickBut1),"
        }
    }
    else
    {
        strcpy(line,s);
    }
    //
    StatusBar("Searching For Function...");
    FindFuncMemberForEvent(parCfile,parMyClass,line,m);
    if(m[0])
    {
        if( PositionEditorSearch(m) < 0)
        {
            StatusBar("Can't Find Function For Event");
        }
        else
        {
            Ready();
        }
    }
    else
    {
        StatusBar("Event Function Not In File");
    }
}

//----------------------------------
// Ignore Escape Key
//----------------------------------
void DlgMsg::EvClickCancel(void)
{
}

//----------------------------------
// Save File before Closing
//----------------------------------
BOOL DlgMsg::CanClose(void)
{
    char cfs[128+1];
    m_Class->GetSelString(cfs,sizeof(cfs));
    RememberClassFileName(cfs);
    SaveMLEfile();
    TDialog::CanClose();
    return TRUE;
}

//----------------------------------
// ADD NEW CLASS
//----------------------------------
void DlgMsg::EvClickAddClass(void)
{
    SaveMLEfile();
    StatusBar("Add New Class To Project");
    NewClass X(this);
    if( X.Execute() == IDOK)
    {
        char s[128+1];
        X.Output(s);
        m_Class->SetSelIndex(-1);
        EvClickRescan();
        int i = m_Class->FindString(s,-1);
        m_Class->SetSelIndex(i);
    }
    StatusBar(NULL);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_USERTOOL
//----------------------------------
void DlgMsg::EvClickTools(void)
{
    ToolDialog(0);
}

void DlgMsg::ToolDialog(int toolnum)
{
    SaveMLEfile();
    UINT sPos, ePos;
    m_Member->GetSelection(sPos, ePos);
    UINT curLine = 1 + m_Member->GetLineFromPos(ePos);  // 1.07 fix
    if(curLine < 1)
        curLine = 1;
    //
    Tools X(this,toolnum);
    X.Set(parHfile,parCfile,curLine);
    if( X.Execute() == IDOK && UserSetup.RescanTool )
    {
        EvClickRescan();
    }
}

//----------------------------------
// Event EV_BN_CLICKED for ID_ADDEVENT
//----------------------------------
void DlgMsg::EvClickAddEvent(void)
{
    AddEvent(TRUE);
}

//----------------------------------
// Event EV_LBN_DBLCLK for ID_EVENT
//----------------------------------
void DlgMsg::EvDblClickEvent(void)
{
    AddEvent(FALSE);
}

//----------------------------------
// ADD EVENT OR FUNCTION
//----------------------------------
void DlgMsg::AddEvent(int warning)
{
    char s[400+1];
    s[0] = NULL;
    m_Event->GetSelString(s,sizeof(s)-1);

    char fname2[MAXPATHSIZE];
    char func[MAXPATHSIZE];
    char mac[80+1];
    char ret[80+1];
    char arg[80+1];
    char id[80+1];
    char line[MAXPATHSIZE];
    //
    //  ***** ADD USER FUNCTION *****
    //
    if(strcmp(s,USERFUNCTION) == 0 )
    {
        SaveMLEfile();  // SAVE FILE !!!
        UserFunc Uf(this);
        Uf.SetHCfiles(parHfile,parCfile,parMyClass);
        if( Uf.Execute() == IDOK)
        {
            LoadMLEfile(parCfile);
            char key[MAXPATHSIZE];
            Uf.GetResult(key);
            PositionEditorSearch(key);
            return;
        }
        else
        {
            return; // canceled user func
        }
    }

    if(s[0] == ' ')
        return; // Comment! non selectable text !
    //
    if( m_Event->GetSelIndex() < 0 || s[0] == NULL)
    {
        if(warning)
        {
            WarnBeep();
            MessageBox("No Event Selected","WARNING", MB_ICONSTOP|MB_CANCEL);
        }
        return;
    }
    SaveMLEfile();  // SAVE FILE !!!
    SetCursor(0,IDC_WAIT);

    //
    // ***** ADD FUNCTION *****
    //
    if(m_ModeFun)
    {
        ParseRetFunArg(s,ret,func,arg);
        char undonearg[MAXPATHSIZE];
        Cproto X;
        X.UnDoDefaultParms(arg,undonearg);
        char f1[MAXPATHSIZE];
        sprintf(f1,":%s(%s)",func,undonearg);
        int funfound = GrepFiles(TRUE,NULL,NULL,parMyClass,"::",f1,parCfile,fname2,line);
        if(funfound)
        {
            if(warning)
            {
                WarnBeep();
                MessageBox("Function Already In Code","WARNING", MB_ICONSTOP|MB_CANCEL);
            }
            SetCursor(0,IDC_ARROW);
            return;
        }
        SetCursor(0,IDC_WAIT);
        X.AddFunction(parHfile,parCfile,parMyClass,"protected",
                        NULL,ret,func,arg,"Virtual Function");

        ///
        /// Add code to call same function in base class
        ///
        char code[80+1];
        char param[80+1];
        ConvArgParam(undonearg,param);
        char rrr[80+1];
        rrr[0] = NULL;
        if(strcmp(ret,"void") != 0)
        {
            sprintf(rrr,"%s RetValue =",ret);
        }

        sprintf(code,"%s%s%s::%s(%s);",UserSetup.Tabs,rrr,parBaseClass,func,param);
        X.WriteCode(parCfile, parMyClass, func, code, FALSE);

        if(rrr[0])
        {
            sprintf(code,"%sreturn(RetValue);",UserSetup.Tabs);
            X.WriteCode(parCfile, parMyClass, func, code, TRUE);
        }
        ///
        LoadMLEfile(parCfile);
        char key[MAXPATHSIZE];
        sprintf(key,"%s %s::%s(",ret,parMyClass,func);
        PositionEditorSearch(key);
        SetCursor(0,IDC_ARROW);
        return;
    }


    ///
    ///  ***** ADD EVENTS *****
    ///
    // arg can have spaces
    GetThisSymbol(s,mac);
    char *ps = strchr(s,' ');
    if(!ps)
    {
        return;
    }
    SetCursor(0,IDC_WAIT);
    *ps = NULL; // macro name in 'mac'
    ps++; // point to 1st char of return string

    memset(arg,0,sizeof(arg));
    ParseRetFunArg(ps,ret,func,arg);

    m_Objects->GetSelString(s,sizeof(s)-1);
    sscanf(s,"%s",id);

    //
    // ***** USER DEFINED NAME OF EVENT *****
    //
    if(func[0]== '?')
    {
        char mac2[MAXPATHSIZE];
        StatusBar("Scanning RESPONSE TABLE...");
        strcpy(mac2,mac);
        strcat(mac2,"(");   // MACRONAME PAREN
        strcat(mac2,id);
        strcat(mac2,",");   // string is like "EV_COMMAND(CM_COPY,"
                            // comma added so "EV_COMMAND(CM_COPYS," is NOT found!
        int found = GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",mac2,"",parChildId,parCfile,fname2,line);
        if(found)
        {
            char msg[MAXPATHSIZE];
            sprintf(msg,"Event '%s' Is Already In Table!",mac2);
            StatusBar(msg);
            if(warning)
            {
                WarnBeep();
                MessageBox(msg,"WARNING", MB_ICONSTOP|MB_CANCEL);
                Ready();
            }
            SetCursor(0,IDC_ARROW);
            return;
        }

        SetCursor(0,IDC_WAIT);
        char var[80+1];
        strcpy(var,id);
        char *v = var;
        if(var[0] == 'I' && var[1] == 'D')
        {
            v = &var[2];
            if(*v == 'D' || *v == '_')  // ID_NAME | IDD_NAME --> NAME
                v++;
            strlwr(var);
            *v = toupper(*v);   // NAME --> Name
        }
        else if(var[0] == 'C' && var[1] == 'M' && var[2] == '_')
        {
            v = &var[3];
            strlwr(var);
            *v = toupper(*v);   // CM_NAME --> Name
        }

        sprintf(s,"%s%s",&func[1],v);
        strcpy(func,s);

        EdFunc E(this);
        E.Input(parHfile,parCfile,parMyClass,func);
        if( E.Execute() != IDOK)
        {
            SetCursor(0,IDC_ARROW);
            return;
        }
        E.Output(func);
    }
    else
    {
        StatusBar("Scanning RESPONSE TABLE...");
        char fname2[MAXPATHSIZE];
        char mac2[MAXPATHSIZE];
        strcpy(mac2,mac);
        strcat(mac2,",");   // MACRONAME COMMA
        int found = GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",mac2,"","",parCfile,fname2,line);

        if(found)
        {
            char msg[MAXPATHSIZE];
            sprintf(msg,"Event '%s' Is Already In Table!",mac);
            StatusBar(msg);
            if(warning)
            {
                WarnBeep();
                MessageBox(msg,"WARNING", MB_ICONSTOP|MB_CANCEL);
            }
            SetCursor(0,IDC_ARROW);
            return;
        }
    }
    //
    SetCursor(0,IDC_ARROW);
    SaveMLEfile();  // SAVE FILE !!!
    SetCursor(0,IDC_WAIT);
    //
    StatusBar("Adding Event To Response Table...");
    Cproto X;
    X.AddEvent(parCfile,parMyClass,mac,id,func);

    char fname[MAXPATHSIZE];
    char funckey[MAXPATHSIZE];
    strcpy(funckey,func);
    strcat(funckey,"(");
    int funfound = GrepFiles(TRUE,NULL,NULL,parMyClass,"::",funckey,parCfile,fname,s);
    if(!funfound)   // not found
    {
        StatusBar("Adding Function To Files...");
        // FOUND DOESNT EXIST - SO ADD IT !
        char comment[80+1];
        sprintf(comment,"Event %s for %s",mac,id);
        X.AddFunction(parHfile,parCfile,parMyClass,"protected",
                        NULL,ret,func,arg,comment);

        // Version 1.03
        if(strcmp(mac,"EV_COMMAND_ENABLE") == 0)
        {
            char code[80+1];
            sprintf(code,"%sCommandEnabler.Enable(FALSE);",UserSetup.Tabs);
            X.WriteCode(parCfile, parMyClass, func, code, FALSE);
        }

        if(strncmp(mac,"EV_WM_",6) == 0)
        {
            // add code to call same function in base class
            char code[80+1];
            char param[80+1];
            ConvArgParam(arg,param);
            sprintf(code,"%s%s::%s(%s);",UserSetup.Tabs,parBaseClass,func,param);
            X.WriteCode(parCfile, parMyClass, func, code, FALSE);
        }
        LoadMLEfile(parCfile);
        char key[MAXPATHSIZE];
        sprintf(key,"%s %s::%s(",ret,parMyClass,func);
        PositionEditorSearch(key);
    }
    SetCursor(0,IDC_ARROW);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DELEVENT
//----------------------------------
void DlgMsg::EvClickDelEvent(void)
{
    int warning = TRUE;
    char s[MAXPATHSIZE];
    s[0] = NULL;
    m_Event->GetSelString(s,sizeof(s)-1);
    if(s[0] == ' ')
        return; // Comment! non selectable text !
    //
    if( m_Event->GetSelIndex() < 0 || s[0] == NULL)
    {
        if(warning)
        {
            WarnBeep();
            MessageBox("No Event Selected","WARNING", MB_ICONSTOP|MB_CANCEL);
        }
        return;
    }
    // arg can have spaces
    char func[MAXPATHSIZE];
    char mac[80+1];
    char ret[80+1];
    char arg[80+1];
    char msg[512];
    char fname2[MAXPATHSIZE];
    char line[MAXPATHSIZE];
    SaveMLEfile();
    Cproto X;
    //
    if(m_ModeFun)
    {
        if(strcmp(s,USERFUNCTION) == 0 )
        {
            WarnBeep();
            MessageBox("User Function Must Be Deleted Manually","NOTE!", MB_ICONSTOP|MB_CANCEL);
            return;
        }

        ParseRetFunArg(s,ret,func,arg);
        char f1[MAXPATHSIZE];
        sprintf(f1,"%s(%s)",func,arg);
        int funfound = GrepFiles(TRUE,NULL,NULL,parMyClass,"::",f1,parCfile,fname2,line);
        if(funfound)
        {
            sprintf(msg,"%s %s::%s(%s)",ret,parMyClass,func,arg);
            WarnBeep();
            if( MessageBox(msg,"CONFIRM DELETE FUNCTION",MB_YESNOno) == IDNO)
                return;
            X.RemoveFunction(parHfile,parCfile,parMyClass,func,arg);
            LoadMLEfile(parCfile);
        }
        return;
    }
    ///
    /// EVENTS
    ///
    memset(arg,0,sizeof(arg));
    GetThisSymbol(s,mac);
    char *ps = strchr(s,' ');
    if(!ps)
    {
        return;
    }
    *ps = NULL; // macro make in 'mac'
    ps++; // point to 1st char of return string
    ParseRetFunArg(ps,ret,func,arg);

    m_Objects->GetSelString(s,sizeof(s)-1);
    char id[80+1];
    id[0] = NULL;
    sscanf(s,"%s",id);
    //
    if(strncmp(mac,"EV_WM_",6) != 0)    // User Named Function
    {
        char mac2[MAXPATHSIZE];
        strcpy(mac2,mac);
        strcat(mac2,"(");   // MACRONAME(
        int found = GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",mac2,id,",",parCfile,fname2,line);
        if(found)
        {
            char line2[MAXPATHSIZE];
            strcpy(line2,line);
            PrettyString(line2);
            sprintf(msg,"Remove Event Macro From Table\n"
                    "%s\n\n",line2);
            WarnBeep();
            if( MessageBox(msg,"CONFIRM DELETE EVENT",MB_YESNOno) == IDNO)
                return;

            char *p = strchr(line2,',');    // BN_CLICKED(ID_NAME,function);
            if(p)   // parse out function name
            {
                GetThisSymbol(p+1,func);
                p = strchr(p,')');
                if(p)
                    *p = NULL;
            }
            SaveMLEfile();  // SAVE FILE !!!
            X.DeleteLineFromFile(fname2,line);
            // SEE IF FUNCTION IS USED BY ANOTHER TABLE ENTRY !!!
            found = GrepFiles(FALSE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",parChildId,",",func,  parCfile,fname2,line);
            if(!found)  // Look again For macro Without Using ChilId !!!
                found = GrepFiles(FALSE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE","EV_",",",func,  parCfile,fname2,line);

            if(!found) // 2nd occurance not found?
            {
                sprintf(msg,"Comment Out Function From Code File"
                        "%s::%s(%s)\n\n",parMyClass,func,arg);
                WarnBeep();
                if( MessageBox(msg,"CONFIRM DELETE",MB_YESNOno) == IDNO)
                    return;
                X.RemoveFunction(parHfile,parCfile,parMyClass,func,arg);
            }
            else
            {
                char msg[1024+1];
                PrettyString(line);
                sprintf(msg,"Function %s is still needed by another\n"
                            "Event in response table\n\n"
                            "Event Usage: %s\n\n"
                            "Position Editor To 1st Usage?",func,line);
                WarnBeep();
                if( MessageBox(msg,"FUNCTION IS NEEDED",MB_YESNOno) == IDYES)
                {   // 1.03
                    PositionEditorSearch(line);
                }
            }
        }
    }
    else // EV_WM type of function name hardcoded
    {
        char mac2[MAXPATHSIZE];
        strcpy(mac2,mac);
        strcat(mac2,",");   // MACRONAME,
        int found = GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE",mac2,",",",",parCfile,fname2,line);
        if(found)
        {
            sprintf(msg,"Remove Event From Macro Table\n"
                    "Event: %s\n"
                    "Delete prototype from header and\n"
                    "Comment Out Function ?\n\n"
                    "Function: %s::%s(%s)\n\n",line,parMyClass,func,arg);
            WarnBeep();
            if( MessageBox(msg,"CONFIRM DELETE",MB_YESNOno) == IDNO)
                return;

            SaveMLEfile();  // SAVE FILE !!!
            X.DeleteLineFromFile(fname2,line);  // remove event !
            X.RemoveFunction(parHfile,parCfile,parMyClass,func,arg);
        }
    }
    LoadMLEfile(parCfile);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_VARIABLE
//----------------------------------
void DlgMsg::EvClickVariable(void)
{
    SaveMLEfile();  // SAVE FILE !!!
    StatusBar("Variables Dialog Active...");
    VarDlg V(this);
    V.SetHCfiles(parMyClass,parHfile,parCfile,m_Objects);
    V.Execute();
    StatusBar(NULL);
    LoadMLEfile(parCfile);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_SETUP
//----------------------------------
void DlgMsg::EvClickSetup(void)
{
    StatusBar("Setup Dialog Active...");
    Setup X(this);
    X.Execute();
    StatusBar(NULL);
}


//----------------------------------
// Event EV_WM_RBUTTONUP for ID_DLGMSG1000
//----------------------------------
void DlgMsg::EvRButtonUp(UINT modKeys, TPoint& point)
{
    TDialog::EvRButtonUp(modKeys, point);

    TRect rO = m_Objects->GetWindowRect();
    TRect rE = m_Event->GetWindowRect();
    TRect rM= m_Member->GetWindowRect();
    TPoint p2 = point;
    ClientToScreen(p2);
    //
    //
    if( PointInRect(&p2,&rO) )
    {
        RightMenu.DeleteMenu(CM_FINDEVENT,MF_BYCOMMAND);
        RightMenu.DeleteMenu(CM_FINDOBJECTS,MF_BYCOMMAND);
        RightMenu.DeleteMenu(CM_HELPINDEX,MF_BYCOMMAND);
        RightMenu.AppendMenu(MF_BYPOSITION,CM_FINDOBJECTS,"~Search Items...");
        RightMenu.TrackPopupMenu(TPM_LEFTALIGN | PU_KEYBOARD | PU_MOUSEBUTTON1,point,HWindow);
    }
    else if( PointInRect(&p2,&rE) )
    {
        RightMenu.DeleteMenu(CM_FINDEVENT,MF_BYCOMMAND);
        RightMenu.DeleteMenu(CM_FINDOBJECTS,MF_BYCOMMAND);
        RightMenu.DeleteMenu(CM_HELPINDEX,MF_BYCOMMAND);
        RightMenu.AppendMenu(MF_BYPOSITION,CM_HELPINDEX,"~Owl Pm Help Reference...");
        RightMenu.AppendMenu(MF_BYPOSITION,CM_FINDEVENT,"~Search Event/Functions...");
        RightMenu.TrackPopupMenu(TPM_LEFTALIGN | PU_KEYBOARD | PU_MOUSEBUTTON1,point,HWindow);
    }
    else
    {
        TClipboard &Clip = OpenClipboard();
        int paste = MF_DISABLED;
        if( Clip.IsClipboardFormatAvailable(CF_TEXT) )
            paste = MF_ENABLED;
        Clip.CloseClipboard();
        //
        UINT sPos,ePos;
        m_Member->GetSelection(sPos,ePos);
        int selected = MF_DISABLED;
        if(sPos < ePos) // selected text! ? TRUE
            selected = MF_ENABLED;

        // Enable Editting cut/paste menu
        EdMenu.EnableMenuItem(CM_COPY,selected);
        EdMenu.EnableMenuItem(CM_CUT,selected);
        EdMenu.EnableMenuItem(CM_DEL,selected);
        //
        EdMenu.EnableMenuItem(CM_PASTE,paste);
        //
        EdMenu.EnableMenuItem(CM_UNDO,m_Member->CanUndo() ? MF_ENABLED : MF_DISABLED);
        //
        int write = MF_DISABLED;
        if( m_Member->IsModified()
            && ( UserSetup.EdMle == promptsave || UserSetup.EdMle == autosave ) )
        {
            write = MF_ENABLED;
        }

        EdMenu.DeleteMenu(CM_HELPINDEX,MF_BYCOMMAND);
        EdMenu.AppendMenu(MF_BYPOSITION,CM_HELPINDEX,"~Owl Pm Help Reference...");
        EdMenu.DeleteMenu(CM_BROWSE,MF_BYCOMMAND);
        EdMenu.AppendMenu(MF_BYPOSITION,CM_BROWSE,"~Browse Code Snippets...");
        EdMenu.DeleteMenu(CM_SAVEMLEFILE,MF_BYCOMMAND);
        char s[MAXPATHSIZE];
        sprintf(s,"~Save %s",m_MleFile);
        EdMenu.AppendMenu(write|MF_BYPOSITION,CM_SAVEMLEFILE,s);
        EdMenu.TrackPopupMenu(TPM_LEFTALIGN | PU_KEYBOARD | PU_MOUSEBUTTON1,point,HWindow);
    }
}


//----------------------------------
// Event EV_COMMAND for CM_FINDOBJECTS
//----------------------------------
void DlgMsg::EvCmFindobjects(void)
{
    char s[MAXSTRLEN+1];
    InputDialog D(this);
    strcpy(s,HistoryUserData[0]);
    D.Init("Search History","Search Items For Partial String",MAXHISTORY,HistoryUserData,s,MAXSTRLEN);
    if( D.Execute() != IDOK)
        return;
    FindSubStrLB(s,m_Objects);
}

//----------------------------------
// Event EV_COMMAND for CM_FINDEVENT
//----------------------------------
void DlgMsg::EvCmFindevent(void)
{
    char s[MAXSTRLEN+1];
    strcpy(s,HistoryUserData[0]);
    InputDialog D(this);
    D.Init("Search History","Search Events For Partial String",MAXHISTORY,HistoryUserData,s,MAXSTRLEN);
    if( D.Execute() != IDOK)
        return;
    FindSubStrLB(s,m_Event);
}


//----------------------------------
// Event EV_COMMAND for CM_FINDMEMBER
//----------------------------------
void DlgMsg::EvCmFindmember(void)
{
    char s[1024+1];
    strcpy(s,HistoryUserData[0]);
    // If Edit has Hilited Selected Text Use that
    UINT sPos,ePos;
    m_Member->GetSelection(sPos,ePos);
    if(sPos < ePos) // selected text!
    {
        m_Member->GetSubText(s,sPos,ePos);
        removeNL(s);
        s[MAXSTRLEN-1] = NULL;
    }

    InputDialog D(this);
    D.Init("Search History","Search Editor For Partial String",MAXHISTORY,HistoryUserData,s,MAXSTRLEN);
    if( D.Execute() != IDOK)
        return;
    SetCursor(0,IDC_WAIT);

    m_Member->GetSelection(sPos,ePos);
    int i = m_Member->Search(ePos+1,s);
    if(i >= 0)
    {
        m_Member->SetSelection(i,i+strlen(s));
        int ln = m_Member->GetLineFromPos(i);
        int fv = m_Member->GetFirstVisibleLine();
        m_Member->Scroll(-9999,(ln-fv)-3);    // Show found item on 3rd line !!!
    }
    else
    {
        WarnBeep();
    }
    SetCursor(0,IDC_ARROW);
}


//
// Find Next Occurance of substriung in list box
//  return index at set selection there too
// Non case sensitive
int DlgMsg::FindSubStrLB(char *s, TListBox *pLB)
{
    char ss[80+1];
    strncpy(ss,s,sizeof(ss));
    strupr(ss);

    int n= pLB->GetCount();
    int max = n-1;
    int i = pLB->GetSelIndex();
    for(i=i+1; n-- > 0; i++)
    {
        if(i > max)
            i = 0;
        char ls[MAXPATHSIZE];
        pLB->GetString(ls,i);
        strupr(ls);
        if(strstr(ls,ss) )
        {
            pLB->SetSelIndex(i);
            return(i);
        }
    }
    WarnBeep();
    return -1;
}

//
// LOAD TEXT file INTO MLE Editor
//
int DlgMsg::LoadMLEfile(char *file)
{
    m_MleFile[0] = NULL;
    int tab = 16;   // pixels
    m_Member->SetTabStops(1,&tab);

    FILE *fi = fopen(file,"rb");
    if(!fi)
        return 1;
    //
    SetCursor(0,IDC_WAIT);
    strcpy(m_MleFile,file);
    char s[MAXPATHSIZE];
    sprintf(s,"Loading %s",file);
    m_EdFile->SetText(s);
    struct stat sb;
    sb.st_size = 0;
    fstat(fileno(fi),&sb);
    m_Member->Clear();
    m_Member->SetReadOnly(UserSetup.EdMle == rdonly);
    //
    char *p = m_Member->LockBuffer(sb.st_size+1);
    int n = fread(p,1,sb.st_size,fi);
    fclose(fi);
    //////////////////
    p[n] = NULL;
    m_Member->UnlockBuffer(p,TRUE);
    m_Member->EmptyUndoBuffer();
    m_Member->ClearModify();
    sprintf(s,"Edit: %s",file);
    m_EdFile->SetText(s);
    SetCursor(0,IDC_ARROW);     //11-5-95
    return 0;
}

void DlgMsg::EvClickSaveMLEfile(void)
{
    SaveMLEfile();
}

//
// SAVE MLE TEXT to File - based on setup option
//
int DlgMsg::SaveMLEfile(void)
{
    if(m_MleFile[0] == NULL
        || UserSetup.EdMle == rdonly
        || UserSetup.EdMle == neversave
        || !m_Member->IsModified() )
    {
        return 0; // no saving desired/needed
    }

    char s[512+1];
    if(UserSetup.EdMle == promptsave && m_Member->IsModified())
    {
        sprintf(s,"Save Changes To %s ?",m_MleFile);
        WarnBeep();
        if( MessageBox(s,"PROMPT TO SAVE",MB_YESNOno) == IDNO)
        {
            m_Member->ClearModify();    // throw away changes!
            return 0;
        }
    }

    SetCursor(0,IDC_WAIT);

    if(UserSetup.EnableBak)
    {
        // RENAME FILE TO BACKUP FILE NAME
        char bkufile[MAXPATHSIZE];
        strcpy(bkufile,m_MleFile);
        ForceExt(bkufile,UserSetup.EdBak);
        rename(m_MleFile,bkufile);
    }

    //
    // OPEN FILE TO WRITE
    //
    FILE *fo = fopen(m_MleFile,"wt");
    if(!fo)
    {
        sprintf(s,"ERROR Can Not Save %s\n"
                "Check File Name, Disk Space, File Attributes\n\n",m_MleFile);
        WarnBeep();
        SetCursor(0,IDC_ARROW);
        MessageBox(s,"FATAL ERROR");
        return 1;
    }
    //
    sprintf(s,"Writing %s",m_MleFile);
    m_EdFile->SetText(s);
    char *p = m_Member->LockBuffer();
    int lines = m_Member->GetNumLines();
    int lu1 = lines/40;
    if(lu1 < 1)
        lu1 = 1;
    for(int l=0; l<lines; l++)
    {
        char sl[1024];  // MAX LINE LENGTH !
        m_Member->GetLine(sl,sizeof(sl),l);
        removeNL(sl);
        int j = strlen(sl);
        fwrite(sl,1,j,fo);
        if(l < lines-1)     // avoid growing file an extra line each save!
            fputc('\n',fo); // dont write last CR/LF on last line !!!
        if( (l % lu1) == 0)
        {
            sprintf(s,"Writing %s (%d%%)",m_MleFile,l*100/lines);
            m_EdFile->SetText(s);
        }
    }
    fclose(fo);
    m_Member->UnlockBuffer(p);
    m_Member->EmptyUndoBuffer(); // not necessary here.. just for consistency
    m_Member->ClearModify();
    sprintf(s,"Edit: %s",m_MleFile);
    m_EdFile->SetText(s);
    SetCursor(0,IDC_ARROW);
    //
    return 0;
}



//
// Cursor is on or IN Function - search up for function name
// and Goto Event -- select Object & Event
//
void DlgMsg::EvCmGotoevent(void)
{
    char s[MAXPATHSIZE];
    char ss[MAXPATHSIZE];
    UINT sPos, ePos;
    m_Member->GetSelection(sPos, ePos);
    UINT curLine = m_Member->GetLineFromPos(ePos);
    m_Member->GetLine(ss,sizeof(ss),curLine);

    UINT l = curLine;
    while(l > 0)
    {
        m_Member->GetLine(s,sizeof(s),l);
        if(s[0] != '/' && !isspace(s[0]) && strstr(s,"::") && strchr(s,'(') && !strchr(s,';') )
        {
            removeNL(s);
            int i = m_Member->GetLineIndex(l);
            if(i > 0)
            {
                char *p = strchr(s,'(');
                int l = 0;
                if(p)
                    l = p-s;                // Hilite 'void Cls:Func('
                m_Member->SetSelection(i,i+l);
                GotoEventForFunction(s);
                int ln = m_Member->GetLineFromPos(i);
                int fv = m_Member->GetFirstVisibleLine();
                m_Member->Scroll(-9999,(ln-fv)-3);    // Show found item on 3rd line !!!
                break;
            }
        }
        l--;
    }
}


//----------------------------------
// input "void MyClass::MyFunc(int a)"
// Move Indexes onlist boxes to proper Object & Event
//
//----------------------------------
void DlgMsg::GotoEventForFunction(char *line_input)
{
    if(!line_input || !*line_input)
        return;
    //
    char s[MAXPATHSIZE];
    strcpy(s,line_input);
    char arg[128+1];
    char func[128+1];
    char *p = strstr(s,"::");
    if(!p)
        return; // Sorry!
    GetThisSymbol(p+2,func);
    p = strchr(p+2,'(');
    if(!p)
        return;
    p++;
    while(*p && isspace(*p))
        p++;
    if(!*p)
        return; // cant find arg list of function
    //
    strcpy(arg,p);
    p = strchr(arg,')');
    if(!*p)
        return; // cant find closing paren of function list
    //
    *p = NULL;

    char fname2[MAXPATHSIZE];
    char line[MAXPATHSIZE];
    char mac[MAXPATHSIZE];
    ReverseLookUpEV_WM(func,arg,mac);   // Is it a stanard Macro name ?
    int found = FALSE;
    if(mac[0])
    {
        char mac2[MAXPATHSIZE];
        strcpy(mac2,mac);
        strcat(mac2,",");   // Find WM_SOMETHING,  // name(comma)
        found = GrepFiles(TRUE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE","EV_WM_",mac2,",",parCfile,fname2,line);
    }
    else // user Message for child Control
    {
        found = GrepFiles(FALSE,"DEFINE_RESPONSE_TABLE","END_RESPONSE_TABLE","EV_",func,",",parCfile,fname2,line);
    }
    if(found)
    {
        onespace(line,s);
        removeNL(s);
        StatusBar(s);
        m_IgnoreEventChange = TRUE;
        char m[MAXPATHSIZE];
        if(mac[0])
        {
            m_Objects->SetSelIndex(0);
        }
        else
        {
            char *p = strchr(s,'(');
            char id[80+1];
            id[0] = NULL;
            if(p)
                GetThisSymbol(p+1,id);
            int i = m_Objects->FindString(id,-1);
            if(i >= 0)
                m_Objects->SetSelIndex(i);
        }
        DefaultProcessing();
        GetThisSymbol(s,m);
        int i = m_Event->FindString(m,-1);
        if(i >= 0)
            m_Event->SetSelIndex(i);
        DefaultProcessing();
        m_IgnoreEventChange = FALSE;
    }
}

//----------------------------------
// Event EV_COMMAND for CM_CUT
//----------------------------------
void DlgMsg::EvCmCut(void)
{
    m_Member->Cut();
}

//----------------------------------
// Event EV_COMMAND for CM_DEL
//----------------------------------
void DlgMsg::EvCmDel(void)
{
    m_Member->DeleteSelection();
}

//----------------------------------
// Event EV_COMMAND for CM_COPY
//----------------------------------
void DlgMsg::EvCmCopy(void)
{
    m_Member->Copy();
}


//----------------------------------
// Event EV_COMMAND for CM_PASTE
//----------------------------------
void DlgMsg::EvCmPaste(void)
{
    m_Member->Paste();
}


//----------------------------------
// Event EV_COMMAND for CM_UNDO
//----------------------------------
void DlgMsg::EvCmUndo(void)
{
    m_Member->Undo();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_PATH
//----------------------------------
void DlgMsg::EvClickPath(void)
{
    SaveMLEfile();
    PrjPath X(this);
    if( X.Execute() == IDOK)
    {
        DoTitle();
        EvClickRescan();
        m_Class->SetSelIndex(0);
        char line[MAXPATHSIZE];
        line[MAXPATHSIZE];
        strcpy(line,TestNewAppNeeded());
        if(line[0]==NULL && !NeedSetup() )
        {
            AppOpt X(this);
            if( X.Execute() == IDOK)
            {   // NEW APPLICATION CREATED !
                SaveSettings();
                EvClickRescan();
                m_Class->SetSelIndex(0);
            }
        }
    }
}

void DlgMsg::DoTitle(void)
{
    getcwd(MyProjectPath,sizeof(MyProjectPath));
    char s[512];
    sprintf(s,"%s %s",MyProjectPath,PROFESSOR_OWL_VERSION);
    GetWindowPtr(GetParent())->SetWindowText(s);
}



//----------------------------------
// Event EV_COMMAND for CM_ABOUT
//----------------------------------
void DlgMsg::EvCmAbout(void)
{
    About A(this);
    A.Execute();
}


//----------------------------------
// Event EV_COMMAND_ENABLE for CM_SAVEMLEFILE
//----------------------------------
void DlgMsg::EvCeSavemlefile(TCommandEnabler &CommandEnabler)
{
    int en = FALSE; // Disabled!
    if( m_Member->IsModified()
        && ( UserSetup.EdMle == promptsave || UserSetup.EdMle == autosave ) )
    {
        en = TRUE;
    }
    CommandEnabler.Enable(en);
}


//----------------------------------
// Event EV_COMMAND for CM_CREATEAPPLICATION
//----------------------------------
void DlgMsg::EvCmCreateapplication(void)
{
    if( NeedSetup() )
    {
        WarnBeep();
        MessageBox( "You Must Run Setup\n"
                    "Before You Can Create An Application\n",
                    "CREATE NEW APPLICATION");
        return;
    }

    char line[MAXPATHSIZE];
    strcpy(line,TestNewAppNeeded());
    if(line[0]!=NULL)
    {
        WarnBeep();
        int r = MessageBox( "\n\nThis Directory Already Has One Application."
                            " Are You Sure You Wish To Create Another ?",
                            "CONFIRM CREATE APPLICATION",MB_ICONSTOP|MB_YESNOno);
        if(r == IDNO)
            return;
    }

    AppOpt X(this);
    if( X.Execute() == IDOK)
    {   // NEW APPLICATION CREATED !
        SaveSettings();
        EvClickRescan();
        m_Class->SetSelIndex(0);
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void DlgMsg::EvClickHelp(void)
{
    Help("");
}

//----------------------------------
// Event EV_COMMAND for CM_HELPINDEX
//----------------------------------
void DlgMsg::EvCmHelpindex(void)
{
    const int MAXOWLSTRLEN = 32;
    static char *HistoryOwlHelp[MAXHISTORY];
    static char HistoryOwlHelpBuffer[MAXHISTORY][MAXOWLSTRLEN+1];
    for(int i=0; i<MAXHISTORY; i++)
    {
        HistoryOwlHelp[i] = HistoryOwlHelpBuffer[i];
    }
    char s[MAXPATHSIZE];
    strcpy(s,"OWLPM.INF ");
    char ss[MAXOWLSTRLEN+1];
    strcpy(ss,HistoryOwlHelp[0]);
    //
    // If Edit has Hilited Selected Text Use that
    //
    UINT sPos,ePos;
    m_Member->GetSelection(sPos,ePos);
    if(sPos < ePos) // selected text!
    {
        m_Member->GetSubText(ss,sPos,ePos);
        removeNL(ss);
        ss[MAXOWLSTRLEN-1] = NULL;
    }
    else // use event function name
    {
        char s[400+1];
        s[0] = NULL;
        m_Event->GetSelString(s,sizeof(s)-1);
        char *p = strchr(s,'(');    // p = "char far * GetClassName(void)" example
        if(p)
        {
            *p = NULL;      // clip tail "(void)"off to  "char far * GetClassName"
            p--;
            while(*p != ' ' && p > s )  // walk to left of function name
                p--;
            if(*p == ' ')
                p++;
            if(p > s &&*p != '?')
            {
                strcpy(ss,"::");
                strcat(ss,p);   // "GetClassName"
                strcat(ss,"*"); //  "::GetClassName*"
            }
            else    // some EV_BN_CLICKED or CM_COMMAND_??? go to general page !
            {
                strcpy(ss,"ObjectWindows Event Handlers");
            }
        }
        else if(s[0] == ' ' &&s[1] == '<') // if <TWindow> -- then "TWindow Class"
        {
            strcpy(ss,&s[2]);
            p = strchr(ss,'>');
            if(p)
            {
                *p = NULL;
                strcat(ss," Class");
            }
        }
    }

    InputDialog D(this);
    D.Init("OWL Help","Search OWLPM.INF For Phrase",MAXHISTORY,HistoryOwlHelp,ss,MAXOWLSTRLEN);
    if( D.Execute() != IDOK)
        return;

    strcat(s,ss);
    StartApp(NULLHANDLE,"OWLPM Help","OWLPM.INF","view.exe",s);
}

//----------------------------------
// Event EV_COMMAND for CM_HELPGENERAL
//----------------------------------
void DlgMsg::EvCmHelpgeneral(void)
{
    EvClickHelp();
}

//----------------------------------
// Postion Editor to 3 lines from top of list box
// on the line that matches 'key' and it not a comment
// Unless requested to search comments
//----------------------------------
int DlgMsg::PositionEditorSearch(char *key, int searchcomments)
{
    SetCursor(0,IDC_WAIT);
    //
    removeNL(key);  // for strstr function
    int i=0; // start seach from zero
    int ln = -1; // incase not found
    char s[MAXPATHSIZE];
    int found = FALSE;
    while(!found && i >= 0)
    {
        i = m_Member->Search(i,key,TRUE);
        m_Member->SetSelection(i,i+1);
        if(i >= 0)
        {
            ln = m_Member->GetLineFromPos(i);
            m_Member->GetLine(s,sizeof(s),ln);
            char *p = strstr(s,"//");
            if(p && !searchcomments)
                *p = NULL;
            // was key NOT part of a comment
            // unles key is comment ?
            if(strstr(s,key))
                found = TRUE;
            else
                i++; // search again after this line!
        }
    }

    if( i >= 0)
    {
        int fv = m_Member->GetFirstVisibleLine();
        m_Member->Scroll(-9999,(ln-fv)-3);  // Show found item on 3rd line !!!
    }
    SetCursor(0,IDC_ARROW);
    return(ln);
}



//----------------------------------
// Event EV_COMMAND for CM_FINDDELETED
//----------------------------------
void DlgMsg::EvCmFinddeleted(void)
{
    SetCursor(0,IDC_WAIT);
    UINT sPos,ePos;
    m_Member->GetSelection(sPos,ePos);
    int i = m_Member->Search(ePos+1," //DELETE_BEGIN");
    if(i >= 0)
    {
        m_Member->SetSelection(i,i);
        int ln = m_Member->GetLineFromPos(i);
        int fv = m_Member->GetFirstVisibleLine();
        m_Member->Scroll(-9999,(ln-fv)-3);    // Show found item on 3rd line !!!
    }
    else
    {
        WarnBeep();
    }
    SetCursor(0,IDC_ARROW);
}



//----------------------------------
// Event EV_COMMAND for CM_FINDCOMMENTFLAG
//----------------------------------
void DlgMsg::EvCmFindcommentflag(void)
{
    SetCursor(0,IDC_WAIT);
    UINT sPos, ePos;
    m_Member->GetSelection(sPos,ePos);
    int i = m_Member->Search(ePos+1,UserSetup.CommentFlag);
    if(i >= 0)
    {
        m_Member->SetSelection(i,i);
        int ln = m_Member->GetLineFromPos(i);
        int fv = m_Member->GetFirstVisibleLine();
        m_Member->Scroll(-9999,(ln-fv)-3);    // Show found item on 3rd line !!!
    }
    else
    {
        WarnBeep();
    }
    SetCursor(0,IDC_ARROW);
}

void DlgMsg::Ready(void)
{
    StatusBar("Ready...");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_SHOWCONST
//----------------------------------
void DlgMsg::EvClickShowconst(void)
{
    EvClickRescan();
}


//----------------------------------
// Virtual Function
//----------------------------------
BOOL DlgMsg::DialogFunction(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL RetValue =TDialog::DialogFunction(message, wParam, lParam);
    #define KC_KEYUP        0x40
    #define KC_VIRTUALKEY   0x02
    #define KC_SCANCODE     0x04
    #define KC_SHIFT        0x08
    #define KC_CONTROL      0x10
    #define KC_ALT          0x20
    #define KC_ALLSHIFT     0x38

    ShowStatusBar();

    if (message == 0x007A && !(wParam & KC_KEYUP) )
    {   // KEY WENT DOWN
        int fs = wParam & 0xFFFF;
        int c = lParam & 0xFFFF;
        int vk = (lParam >> 16) & 0xFF;

        if( (fs & ( KC_VIRTUALKEY | KC_ALLSHIFT)) == KC_VIRTUALKEY )
        {
            switch(vk)
            {
                case VK_F2:
                    EvClickSaveMLEfile();
                break;

                case VK_F3:
                    EvClose();
                break;

                case VK_F5:
                    EvCmFindmember();
                break;

                case VK_F6:
                    EvCmFindcommentflag();
                break;
            }
        }
        else if( (fs & (KC_SCANCODE | KC_ALLSHIFT)) == (KC_SCANCODE | KC_ALT))
        {
            switch(c)
            {
                case 'B':
                case 'b':
                    EvCmBrowse();
                break;

                case 'R':
                case 'r':
                    EvClickRescan();
                break;

                case 'N':
                case 'n':
                    EvClickAddClass();
                break;

                case 'V':
                case 'v':
                    EvClickVariable();
                break;
            }
        }
        if( (fs & (KC_VIRTUALKEY | KC_ALLSHIFT)) == (KC_CONTROL | KC_VIRTUALKEY))
        {
            switch(vk)
            {
                case VK_F1:
                    ToolDialog(1);
                break;

                case VK_F2:
                    ToolDialog(2);
                break;

                case VK_F3:
                    ToolDialog(3);
                break;

                case VK_F4:
                    ToolDialog(4);
                break;

                case VK_F5:
                    ToolDialog(5);
                break;

                case VK_F6:
                    ToolDialog(6);
                break;

                case VK_F7:
                    ToolDialog(7);
                break;

                case VK_F8:
                    ToolDialog(8);
                break;
            }
        }
    }
    return(RetValue);
}


//----------------------------------
// Event EV_COMMAND for CM_WINDOWDEFAULT
//----------------------------------
void DlgMsg::EvCmWindowdefault(void)
{
    int w,h;
    GetDefaultWH(w,h); //size based on desktop
    int y = 16;
    if(h <= 480)
        y = 3;
    MainFramePtr->MoveWindow(16,y,w,h,TRUE);
}


//----------------------------------
// Event EV_COMMAND for CM_WINDOWRECALLSIZEPOS
//----------------------------------
void DlgMsg::EvCmWindowrecallsizepos(void)
{
    RecallWindowSizePos();
}


//----------------------------------
// Event EV_COMMAND for CM_SAVESIZEPOS
//----------------------------------
void DlgMsg::EvCmSavesizepos(void)
{
    SaveWindowSizePos();
}


extern char MyHomePath[];

void RememberClassFileName(char *s)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    PrfWriteProfileString(hini,"profsowl","ClassFile",s);
    PrfCloseProfile(hini);
}


void RecallClassFile(char *s)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    PrfQueryProfileString(hini,"profsowl","ClassFile",".\\",s,80);
    PrfCloseProfile(hini);
}


ProjWin *ProjectWindowPtr;

//----------------------------------
// Event EV_COMMAND for CM_OPENPROJECTWIN
//----------------------------------
void DlgMsg::EvCmOpenprojectwin(void)
{
    if(ProjectWindowPtr == NULL)
    {
        ProjectWindowPtr = new ProjWin(this);
        ProjectWindowPtr->Create();
    }
    else
    {
        ProjectWindowPtr->ShowWindow(SW_SHOW);
    }
}


//----------------------------------
// Event EV_COMMAND_ENABLE for CM_OPENPROJECTWIN
//----------------------------------
void DlgMsg::EvCeOpenprojectwin(TCommandEnabler &CommandEnabler)
{
    CommandEnabler.Enable(TRUE);
}

//----------------------------------
// Event EV_COMMAND for CM_NOTIFY_PRJWIN_CLOSE
//----------------------------------
void DlgMsg::EvCmNotify_prjwin_close(void)
{
    ProjectWindowPtr = NULL;
}

//---------------------------------------------------------------
// Event EV_COMMAND for CM_BROWSE
//---------------------------------------------------------------
void DlgMsg::EvCmBrowse(void)
{
    Browse B(this);
    B.Execute();
}


//---------------------------------------------------------------
//  'User Function' public: paste Clipboard Text To Mle Editor
//---------------------------------------------------------------
void DlgMsg::Paste(void)
{
    m_Member->Paste();
}


//---------------------------------------------------------------
// Event EV_WM_CLOSE for DlgMsg     1.08  F3 = Close !
//---------------------------------------------------------------
void DlgMsg::EvClose(void)
{
    if( MessageBox("Exit Professor Owl ?","CONFIRM EXIT",MB_YESNOno) == IDNO)
         return;
    ///
    WinPostMsg( GetParent(), WM_CLOSE, 0, 0);
    TDialog::EvClose();
}



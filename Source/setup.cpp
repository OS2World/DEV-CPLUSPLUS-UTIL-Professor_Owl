#include    "def.h"
#define INCL_WINSHELLDATA
#define INCL_DOS
#define     NULL    0
#include    <os2.h>
//
#include    <io.h>
#include    <owl\dialog.h>
#include    <owl\combobox.h>
#include    <owl\edit.h>
#include    "resource.h"
#include    "setup.h"
#include    "settool.h"
#include    "setupdat.h"
#include    "tools.h"
#include    "parse.h"
#include    "WarnBeep.h"
#include    "Comment.h"
#include    "Version.h"
#include     "startapp.h"
//
SETUPDATA    UserSetup;

//----------------------------------
// MESSAGE MAP FOR Setup
//----------------------------------
DEFINE_RESPONSE_TABLE1(Setup,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_DEFAULT,EvClickDefault),
    EV_BN_CLICKED(ID_SAVEINI,EvClickSave),
    EV_EN_CHANGE(ID_EDBAK,EvChangeEdbak),
    EV_EN_KILLFOCUS(ID_OWLPATH,EvKillFocusOwlpath),
    EV_BN_CLICKED(ID_SETUP,EvClickSetup),
    EV_BN_CLICKED(ID_WARNBEEP,EvClickWarnbeep),
    EV_BN_CLICKED(ID_COMMENTSETUP,EvClickCommentsetup),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
    EV_BN_CLICKED(ID_RESCANTOOL,EvClickRescantool),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
Setup::Setup(TWindow *parent) : TDialog(parent, ID_DLGSETUP)
{
    m_Edbak = new TEdit(this,ID_EDBAK);
    m_Tabs = new TComboBox(this,ID_TABS);
    m_Edsave = new TComboBox(this,ID_EDSAVE);
    m_Includepath = new TEdit(this,ID_INCLUDEPATH);
    m_Memprefix = new TEdit(this,ID_MEMPREFIX);
}




//----------------------------------
// Destructor
//----------------------------------
Setup::~Setup()
{
    delete m_Edbak;
    delete m_Tabs;
    delete m_Edsave;
    delete m_Includepath;
    delete m_Memprefix;
}




//----------------------------------
// Init Dialog
//----------------------------------
BOOL Setup::EvInitDialog(HWND hWndFocus)
{
    BOOL Rc = TDialog::EvInitDialog(hWndFocus);
    m_Tabs->ClearList();
    m_Tabs->AddString("One Tab Character");
    m_Tabs->AddString("Two Tab Characters");
    m_Tabs->AddString("1 Space");
    m_Tabs->AddString("2 Spaces");
    m_Tabs->AddString("3 Spaces");
    m_Tabs->AddString("4 Spaces");
    m_Tabs->AddString("5 Spaces");
    m_Tabs->AddString("6 Spaces");
    m_Tabs->AddString("7 Spaces");
    m_Tabs->AddString("8 Spaces");
    m_Tabs->SetSelIndex(UserSetup.TabIndex);
    //
    m_Edsave->ClearList();
    m_Edsave->AddString("Read Only");
    m_Edsave->AddString("Never Save");
    m_Edsave->AddString("Prompt To Save");
    m_Edsave->AddString("Auto Save");
    m_Edsave->SetSelIndex(UserSetup.EdMle);
    //
    SetupFixups();
    CheckDlgButton(ID_ENABLEBAK,UserSetup.EnableBak);
    CheckDlgButton(ID_WARNBEEP,UserSetup.WarnBeep);
    CheckDlgButton(ID_RESCANTOOL,UserSetup.RescanTool);
    //
    SetDlgItemText(ID_EDBAK,UserSetup.EdBak);
    SetDlgItemText(ID_OWLPATH,UserSetup.OwlPath);
    //
    SetDlgItemText(ID_INCLUDEPATH,UserSetup.IncludePath);
    SetDlgItemText(ID_MEMPREFIX,UserSetup.Memprefix);
    return Rc;
}


//----------------------------------
// Cancel Button
//----------------------------------
void Setup::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void Setup::EvClickOk(void)
{
    Update();
    CmOk();
}

extern char MyHomePath[];

//----------------------------------
// Event EV_BN_CLICKED for ID_SAVEINI
//----------------------------------
void Setup::EvClickSave(void)
{
    Update();
    SaveSettings();
    CmOk();
    /// WRITE VERIONS NUMBER SO NEW VERSION RUN SETUP AUTOMATICALLY V1.05
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    PrfWriteProfileString(hini,"profsowl","Version",PROFESSOR_OWL_VERSION);
    PrfCloseProfile(hini);
}

//
// Update Structure UserSetup from controls
//
void Setup::Update(void)
{
    GetDlgItemText(ID_EDBAK,UserSetup.EdBak,sizeof(UserSetup.EdBak));
    UserSetup.EnableBak = IsDlgButtonChecked(ID_ENABLEBAK);
    UserSetup.WarnBeep  = IsDlgButtonChecked(ID_WARNBEEP);
    UserSetup.RescanTool= IsDlgButtonChecked(ID_RESCANTOOL);
    //
    GetDlgItemText(ID_OWLPATH,UserSetup.OwlPath,sizeof(UserSetup.OwlPath));
    //
    GetDlgItemText(ID_COMMENTFLAG,UserSetup.CommentFlag,sizeof(UserSetup.CommentFlag));
    //
    GetDlgItemText(ID_INCLUDEPATH,UserSetup.IncludePath,sizeof(UserSetup.IncludePath));
    //
    GetDlgItemText(ID_MEMPREFIX,UserSetup.Memprefix,sizeof(UserSetup.Memprefix));
    //
    UserSetup.EdMle = (MLESAVE) m_Edsave->GetSelIndex();
    UserSetup.TabIndex = m_Tabs->GetSelIndex();
    //
    if(UserSetup.TabIndex > 10 || UserSetup.TabIndex < 0)
    {
        UserSetup.TabIndex = 5;
    }
    Settabs();
}

void Settabs(void)
{
    //
    // Set the tabes or spaces need to generate code in 'UserSetup.Tabs'
    //
    char spaces[] = "            "; // 8 spaces or more
    if(UserSetup.TabIndex == 0)
    {
        strcpy(UserSetup.Tabs,"\t");
    }
    else if(UserSetup.TabIndex == 1)
    {
        strcpy(UserSetup.Tabs,"\t\t");
    }
    else // 2,3,4,5,6,7,8,9 --> 1--> 8 spaces
    {
        spaces[UserSetup.TabIndex-1] = NULL;
        strcpy(UserSetup.Tabs,spaces);
    }
}



//----------------------------------
// Event EV_BN_CLICKED for ID_DEFAULT
//----------------------------------
void Setup::EvClickDefault(void)
{
    // Find Borland Disk Drive !!!
    char ss[128+1];
    strcpy(ss,"C:\\BCOS2\\INCLUDE");
    ///
    const char *EnvPathPtr;
    DosScanEnv("PATH",&EnvPathPtr);
    if(EnvPathPtr)
    {
        char EnvPath[512];
        strcpy(EnvPath,EnvPathPtr);
        strupr(EnvPath);
        char *bcp = strstr(EnvPath,":\\BCOS2\\");
        if(bcp)
        {
            bcp--; // Get Drive Letter
            *(bcp+9) = NULL;
            sprintf(ss,"%sINCLUDE",bcp);
        }
    }
    SetDlgItemText(ID_INCLUDEPATH,ss);
    SetDlgItemText(ID_MEMPREFIX,"m_");
    //
    CheckDlgButton(ID_ENABLEBAK,UserSetup.EnableBak=TRUE);
    CheckDlgButton(ID_WARNBEEP,UserSetup.WarnBeep=TRUE);
    CheckDlgButton(ID_RESCANTOOL,UserSetup.RescanTool=TRUE);
    //
    SetDlgItemText(ID_EDBAK,".BKU");
    SetDlgItemText(ID_OWLPATH,"owl");
    m_Tabs->SetSelIndex(5);     // 4 spaces
    m_Edsave->SetSelIndex(3);   // auto save
}

void SetupFixups(void)
{
    UserSetup.EdBak[0] = '.';
    UserSetup.EdBak[4] = NULL;
    strlwr(UserSetup.EdBak);
    //
    strlwr(UserSetup.OwlPath);
    int i = strlen(UserSetup.OwlPath);
    while(i > 0 && (UserSetup.OwlPath[i-1] == '\\' || UserSetup.OwlPath[i-1]==' '))
    {
        UserSetup.OwlPath[--i] = NULL;
    }
}


//----------------------------------
// Event EV_EN_KILLFOCUS for ID_OWLPATH
//----------------------------------
void Setup::EvKillFocusOwlpath(void)
{
    char s[80+1];
    GetDlgItemText(ID_OWLPATH,s,sizeof(s));
    ValidateFileNamePath(s);

    int i = strlen(s);
    while(i > 0 && (s[i-1] == '\\' || s[i-1]==' ') )
    {
        s[--i] = NULL;  // remove trailing backslash or space
        SetDlgItemText(ID_OWLPATH,s);
    }
}


//----------------------------------
// Event EV_EN_CHANGE for ID_EDBAK
//----------------------------------
void Setup::EvChangeEdbak(void)
{
    char s[5+1];
    GetDlgItemText(ID_EDBAK,s,sizeof(s));
    ValidateSymbol(&s[1]);
    if(s[0] != '.' || strlen(s) > 4)
    {
        s[0] = '.';
        s[4] = NULL;
        SetDlgItemText(ID_EDBAK,s);
    }
}


//
// Load PROFOWL.INI Files setting to structures
//
void LoadSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    //
    HAB hab;
    HINI hini = PrfOpenProfile(hab,path);
    LoadToolSettings();
    //
    PrfQueryProfileString(hini,"profsowl","OwlPath","owl",UserSetup.OwlPath,sizeof(UserSetup.OwlPath));
    ///
    char s[32+1];
    PrfQueryProfileString(hini,"profsowl","EdBak",".BKU",UserSetup.EdBak,sizeof(UserSetup.EdBak));
    //
    PrfQueryProfileString(hini,"profsowl","EnableBak","1",s,sizeof(s) );
    UserSetup.EnableBak = atoi(s) ? TRUE : FALSE;
    //
    PrfQueryProfileString(hini,"profsowl","RescanTool","1",s,sizeof(s) );
    UserSetup.RescanTool = atoi(s) ? TRUE : FALSE;
    //
    PrfQueryProfileString(hini,"profsowl","ShowConstants","1",s,sizeof(s) );
    UserSetup.ShowConstants = atoi(s) ? TRUE : FALSE;
    //
    PrfQueryProfileString(hini,"profsowl","WarnBeep","1",s,sizeof(s) );
    UserSetup.WarnBeep = atoi(s) ? TRUE : FALSE;
    ///
    PrfQueryProfileString(hini,"profsowl","IncludePath","c:\\bcos2\\include",UserSetup.IncludePath,sizeof(UserSetup.IncludePath));
    //
    PrfQueryProfileString(hini,"profsowl","Memprefix","m_",UserSetup.Memprefix,sizeof(UserSetup.Memprefix));
    ///---
    PrfQueryProfileString(hini,"profsowl","TabIndex","5",s,sizeof(s));
    UserSetup.TabIndex = atoi(s);
    //
    PrfQueryProfileString(hini,"profsowl","EdMle","3",s,sizeof(s));
    UserSetup.EdMle = (MLESAVE)atoi(s);
    LoadCommentSettings();
    PrfCloseProfile(hini);
    Settabs();
    SetupFixups();
}

//
// Return TRUE is PROFOWL.INI is missing !
//
int NeedSetup(void)
{
    LoadSettings();
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    //
    HAB hab;
    HINI hini = PrfOpenProfile(hab,path);
    //
    char s[32+1];
    PrfQueryProfileString(hini,"profsowl","Version","0.00",s,sizeof(s));
    PrfCloseProfile(hini);

    if(strcmp(s,PROFESSOR_OWL_VERSION) == 0)
    {
        return(FALSE); // We saved a Setup With This Version !
    }
    return(TRUE);   // BETTER RUN SETUP TO BE SAFE !!!
}

//
// SAVE data struct setting to PROFOWL.INI Files
//
void SaveSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);

    SetupFixups();
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    //
    PrfWriteProfileString(hini,"profsowl","OwlPath",UserSetup.OwlPath);
    //
    PrfWriteProfileString(hini,"profsowl","IncludePath",UserSetup.IncludePath);
    //
    PrfWriteProfileString(hini,"profsowl","Memprefix",UserSetup.Memprefix);
    ///
    PrfWriteProfileString(hini,"profsowl","EdBak",UserSetup.EdBak);
    char s[32+1];
    sprintf(s,"%d",UserSetup.EnableBak);
    PrfWriteProfileString(hini,"profsowl","EnableBak",s);
    //
    sprintf(s,"%d",UserSetup.ShowConstants);
    PrfWriteProfileString(hini,"profsowl","ShowConstants",s);
    //
    sprintf(s,"%d",UserSetup.WarnBeep);
    PrfWriteProfileString(hini,"profsowl","WarnBeep",s);
    //
    sprintf(s,"%d",UserSetup.RescanTool);
    PrfWriteProfileString(hini,"profsowl","RescanTool",s);
    ///---
    sprintf(s,"%d",UserSetup.TabIndex);
    PrfWriteProfileString(hini,"profsowl","TabIndex",s);
    //
    sprintf(s,"%d",UserSetup.EdMle);
    PrfWriteProfileString(hini,"profsowl","EdMle",s);
    PrfCloseProfile(hini);
}




//----------------------------------
// Event EV_BN_CLICKED for ID_SETUP
//----------------------------------
void Setup::EvClickSetup(void)
{
    SetTool S(this);
    S.Execute();
}

//----------------------------------
// Event EV_BN_CLICKED for ID_WARNBEEP
//----------------------------------
void Setup::EvClickWarnbeep(void)
{
    WarnBeep();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_COMMENTSETUP
//----------------------------------
void Setup::EvClickCommentsetup(void)
{
    DlgComment C(this);
    C.Execute();
}



//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void Setup::EvClickHelp(void)
{
    Help("User Setup");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_RESCANTOOL
//----------------------------------
void Setup::EvClickRescantool(void)
{
}




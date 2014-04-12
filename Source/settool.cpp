#define INCL_WINSHELLDATA

/// FILE OPEN BROWSER
#define INCL_WINSTDFILE  // FILEDLG Browse Window Standard File Functions

#define NULL    0
#include    "def.h"
#include    <os2.h>
#include    <owl\dialog.h>
#include    <owl\opensave.h>
#include    "resource.h"
#include    "settool.h"
#include    "tools.h"
#include    "parse.h"
#include    "startapp.h"

extern TOOLNAMEPATH UserTools[];

//----------------------------------
// MESSAGE MAP FOR SetTool
//----------------------------------
DEFINE_RESPONSE_TABLE1(SetTool,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_EXAMPLE,EvClickExample),
    EV_BN_CLICKED(ID_BROWSE1,EvClickBrowse1),
    EV_BN_CLICKED(ID_BROWSE2,EvClickBrowse2),
    EV_BN_CLICKED(ID_BROWSE3,EvClickBrowse3),
    EV_BN_CLICKED(ID_BROWSE4,EvClickBrowse4),
    EV_BN_CLICKED(ID_BROWSE5,EvClickBrowse5),
    EV_BN_CLICKED(ID_BROWSE6,EvClickBrowse6),
    EV_BN_CLICKED(ID_BROWSE7,EvClickBrowse7),
    EV_BN_CLICKED(ID_BROWSE8,EvClickBrowse8),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
SetTool::SetTool(TWindow *parent) : TDialog(parent, ID_DLGSETTOOL)
{
    m_Tool8path = new TEdit(this,ID_TOOL8PATH);
    m_Tool7path = new TEdit(this,ID_TOOL7PATH);
    m_Tool6path = new TEdit(this,ID_TOOL6PATH);
    m_Tool5path = new TEdit(this,ID_TOOL5PATH);
    m_Tool4path = new TEdit(this,ID_TOOL4PATH);
    m_Tool3path = new TEdit(this,ID_TOOL3PATH);
    m_Tool2path = new TEdit(this,ID_TOOL2PATH);
    m_Tool1path = new TEdit(this,ID_TOOL1PATH);
    m_Tool1dir = new TEdit(this,ID_TOOL1DIR);
    m_Tool2dir = new TEdit(this,ID_TOOL2DIR);
    m_Tool3dir = new TEdit(this,ID_TOOL3DIR);
    m_Tool4dir = new TEdit(this,ID_TOOL4DIR);
    m_Tool5dir = new TEdit(this,ID_TOOL5DIR);
    m_Tool6dir = new TEdit(this,ID_TOOL6DIR);
    m_Tool7dir = new TEdit(this,ID_TOOL7DIR);
    m_Tool8dir = new TEdit(this,ID_TOOL8DIR);
}


//----------------------------------
// Destructor
//----------------------------------
SetTool::~SetTool()
{
    delete m_Tool8path;
    delete m_Tool7path;
    delete m_Tool6path;
    delete m_Tool5path;
    delete m_Tool4path;
    delete m_Tool3path;
    delete m_Tool2path;
    delete m_Tool1path;
    delete m_Tool1dir;
    delete m_Tool2dir;
    delete m_Tool3dir;
    delete m_Tool4dir;
    delete m_Tool5dir;
    delete m_Tool6dir;
    delete m_Tool7dir;
    delete m_Tool8dir;
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL SetTool::EvInitDialog(HWND hWndFocus)
{
    BOOL rc = TDialog::EvInitDialog(hWndFocus);
    SetDlgItemText(ID_TOOL1NAME,UserTools[1].Name);
    SetDlgItemText(ID_TOOL2NAME,UserTools[2].Name);
    SetDlgItemText(ID_TOOL3NAME,UserTools[3].Name);
    SetDlgItemText(ID_TOOL4NAME,UserTools[4].Name);
    SetDlgItemText(ID_TOOL5NAME,UserTools[5].Name);
    SetDlgItemText(ID_TOOL6NAME,UserTools[6].Name);
    SetDlgItemText(ID_TOOL7NAME,UserTools[7].Name);
    SetDlgItemText(ID_TOOL8NAME,UserTools[8].Name);
    //
    SetDlgItemText(ID_TOOL1PATH,UserTools[1].Path);
    SetDlgItemText(ID_TOOL2PATH,UserTools[2].Path);
    SetDlgItemText(ID_TOOL3PATH,UserTools[3].Path);
    SetDlgItemText(ID_TOOL4PATH,UserTools[4].Path);
    SetDlgItemText(ID_TOOL5PATH,UserTools[5].Path);
    SetDlgItemText(ID_TOOL6PATH,UserTools[6].Path);
    SetDlgItemText(ID_TOOL7PATH,UserTools[7].Path);
    SetDlgItemText(ID_TOOL8PATH,UserTools[8].Path);
    //
    SetDlgItemText(ID_TOOL1DIR,UserTools[1].Dir);
    SetDlgItemText(ID_TOOL2DIR,UserTools[2].Dir);
    SetDlgItemText(ID_TOOL3DIR,UserTools[3].Dir);
    SetDlgItemText(ID_TOOL4DIR,UserTools[4].Dir);
    SetDlgItemText(ID_TOOL5DIR,UserTools[5].Dir);
    SetDlgItemText(ID_TOOL6DIR,UserTools[6].Dir);
    SetDlgItemText(ID_TOOL7DIR,UserTools[7].Dir);
    SetDlgItemText(ID_TOOL8DIR,UserTools[8].Dir);

    return(rc);
}


//----------------------------------
// Cancel Button Pressed
//----------------------------------
void SetTool::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void SetTool::EvClickOk(void)
{
    Update();
    SaveToolSettings();
    CmOk();
}

extern char MyHomePath[];


void SetTool::SaveToolSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);

    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    //
    PrfWriteProfileString(hini,"profsowl","Tool1Name",UserTools[1].Name);
    PrfWriteProfileString(hini,"profsowl","Tool2Name",UserTools[2].Name);
    PrfWriteProfileString(hini,"profsowl","Tool3Name",UserTools[3].Name);
    PrfWriteProfileString(hini,"profsowl","Tool4Name",UserTools[4].Name);
    PrfWriteProfileString(hini,"profsowl","Tool5Name",UserTools[5].Name);
    PrfWriteProfileString(hini,"profsowl","Tool6Name",UserTools[6].Name);
    PrfWriteProfileString(hini,"profsowl","Tool7Name",UserTools[7].Name);
    PrfWriteProfileString(hini,"profsowl","Tool8Name",UserTools[8].Name);
    //--
    PrfWriteProfileString(hini,"profsowl","Tool1Path",UserTools[1].Path);
    PrfWriteProfileString(hini,"profsowl","Tool2Path",UserTools[2].Path);
    PrfWriteProfileString(hini,"profsowl","Tool3Path",UserTools[3].Path);
    PrfWriteProfileString(hini,"profsowl","Tool4Path",UserTools[4].Path);
    PrfWriteProfileString(hini,"profsowl","Tool5Path",UserTools[5].Path);
    PrfWriteProfileString(hini,"profsowl","Tool6Path",UserTools[6].Path);
    PrfWriteProfileString(hini,"profsowl","Tool7Path",UserTools[7].Path);
    PrfWriteProfileString(hini,"profsowl","Tool8Path",UserTools[8].Path);
    //--
    PrfWriteProfileString(hini,"profsowl","Tool1Dir",UserTools[1].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool2Dir",UserTools[2].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool3Dir",UserTools[3].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool4Dir",UserTools[4].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool5Dir",UserTools[5].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool6Dir",UserTools[6].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool7Dir",UserTools[7].Dir);
    PrfWriteProfileString(hini,"profsowl","Tool8Dir",UserTools[8].Dir);
    PrfCloseProfile(hini);
}


void LoadToolSettings(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    //
    int ns = sizeof(UserTools[1].Name);
    int ps = sizeof(UserTools[1].Path);
    int ds = sizeof(UserTools[1].Dir);
    //
    HAB hab;
    HINI hini = PrfOpenProfile(hab,path);
    PrfQueryProfileString(hini,"profsowl","Tool1Name","",UserTools[1].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool2Name","",UserTools[2].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool3Name","",UserTools[3].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool4Name","",UserTools[4].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool5Name","",UserTools[5].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool6Name","",UserTools[6].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool7Name","",UserTools[7].Name,ns);
    PrfQueryProfileString(hini,"profsowl","Tool8Name","",UserTools[8].Name,ns);
    //--
    PrfQueryProfileString(hini,"profsowl","Tool1Path","",UserTools[1].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool2Path","",UserTools[2].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool3Path","",UserTools[3].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool4Path","",UserTools[4].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool5Path","",UserTools[5].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool6Path","",UserTools[6].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool7Path","",UserTools[7].Path,ps);
    PrfQueryProfileString(hini,"profsowl","Tool8Path","",UserTools[8].Path,ps);
    //--
    PrfQueryProfileString(hini,"profsowl","Tool1Dir","",UserTools[1].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool2Dir","",UserTools[2].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool3Dir","",UserTools[3].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool4Dir","",UserTools[4].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool5Dir","",UserTools[5].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool6Dir","",UserTools[6].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool7Dir","",UserTools[7].Dir,ds);
    PrfQueryProfileString(hini,"profsowl","Tool8Dir","",UserTools[8].Dir,ds);
    PrfCloseProfile(hini);
}



//----------------------------------
//  'User Function' (private:)
//----------------------------------
void SetTool::Update(void)
{
    int ns = sizeof(UserTools[1].Name);
    int ps = sizeof(UserTools[1].Path);
    int ds = sizeof(UserTools[1].Dir);
    GetDlgItemText(ID_TOOL1NAME,UserTools[1].Name,ns);
    GetDlgItemText(ID_TOOL2NAME,UserTools[2].Name,ns);
    GetDlgItemText(ID_TOOL3NAME,UserTools[3].Name,ns);
    GetDlgItemText(ID_TOOL4NAME,UserTools[4].Name,ns);
    GetDlgItemText(ID_TOOL5NAME,UserTools[5].Name,ns);
    GetDlgItemText(ID_TOOL6NAME,UserTools[6].Name,ns);
    GetDlgItemText(ID_TOOL7NAME,UserTools[7].Name,ns);
    GetDlgItemText(ID_TOOL8NAME,UserTools[8].Name,ns);
    //
    GetDlgItemText(ID_TOOL1PATH,UserTools[1].Path,ps);
    GetDlgItemText(ID_TOOL2PATH,UserTools[2].Path,ps);
    GetDlgItemText(ID_TOOL3PATH,UserTools[3].Path,ps);
    GetDlgItemText(ID_TOOL4PATH,UserTools[4].Path,ps);
    GetDlgItemText(ID_TOOL5PATH,UserTools[5].Path,ps);
    GetDlgItemText(ID_TOOL6PATH,UserTools[6].Path,ps);
    GetDlgItemText(ID_TOOL7PATH,UserTools[7].Path,ps);
    GetDlgItemText(ID_TOOL8PATH,UserTools[8].Path,ps);
    //
    GetDlgItemText(ID_TOOL1DIR,UserTools[1].Dir,ds);
    GetDlgItemText(ID_TOOL2DIR,UserTools[2].Dir,ds);
    GetDlgItemText(ID_TOOL3DIR,UserTools[3].Dir,ds);
    GetDlgItemText(ID_TOOL4DIR,UserTools[4].Dir,ds);
    GetDlgItemText(ID_TOOL5DIR,UserTools[5].Dir,ds);
    GetDlgItemText(ID_TOOL6DIR,UserTools[6].Dir,ds);
    GetDlgItemText(ID_TOOL7DIR,UserTools[7].Dir,ds);
    GetDlgItemText(ID_TOOL8DIR,UserTools[8].Dir,ds);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_EXAMPLE
//----------------------------------
void SetTool::EvClickExample(void)
{
    SetDlgItemText(ID_TOOL1NAME,"~MisterEd $cfile");
    SetDlgItemText(ID_TOOL1PATH,"Med.exe $cfile $line");
    SetDlgItemText(ID_TOOL1DIR,".\\");
    //
    SetDlgItemText(ID_TOOL2NAME,"Edit $hfile");
    SetDlgItemText(ID_TOOL2PATH,"E.exe $hfile");
    SetDlgItemText(ID_TOOL2DIR,".\\");
    //
    SetDlgItemText(ID_TOOL3NAME,"OS/2 ~Shell");
    SetDlgItemText(ID_TOOL3PATH,"Cmd.Exe");
    SetDlgItemText(ID_TOOL3DIR,"C:\\OS2");
    //
    SetDlgItemText(ID_TOOL4NAME,"~Workshop");
    SetDlgItemText(ID_TOOL4PATH,"c:\\bcos2\\bin\\Workshop.Exe");
    SetDlgItemText(ID_TOOL4DIR,".\\");
    //
    SetDlgItemText(ID_TOOL5NAME,"Edit ~Resource.Rc");
    SetDlgItemText(ID_TOOL5PATH,"E.exe Resource.Rc");
    SetDlgItemText(ID_TOOL5DIR,".\\");
    //
    SetDlgItemText(ID_TOOL6NAME,"~Qedit $cfile line $line");
    SetDlgItemText(ID_TOOL6PATH,"Q.Exe $path\$cfile -n$line");
    SetDlgItemText(ID_TOOL6DIR,".\\");
    //
    SetDlgItemText(ID_TOOL7NAME,"MAKE");
    SetDlgItemText(ID_TOOL7PATH,"MAKE -fMyApp");
    SetDlgItemText(ID_TOOL7DIR,".\\");
    //
    SetDlgItemText(ID_TOOL8NAME,"Delete Junk*.*");
    SetDlgItemText(ID_TOOL8PATH,"c:\\os2\\cmd.exe /C \"del junk*.*\"");
    SetDlgItemText(ID_TOOL8DIR,".\\");
}



//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE
//----------------------------------
void SetTool::Browse(int id, int idname, char *toolnum)
{
    /*****************************************************************/
    /* Initially set all fields to 0                                 */
    /*****************************************************************/
    FILEDLG fild;                                     // File dialog info structure
    char pszFullFile[CCHMAXPATH] = "*.EXE;*.CMD;*.BAT;*.COM";     // File filter string
    memset(&fild, 0, sizeof(FILEDLG));

    /*****************************************************************/
    /* Initialize those fields in the FILEDLG structure that are     */
    /* used by the application                                       */
    /*****************************************************************/
    fild.cbSize = sizeof(FILEDLG);            // Size of structure
    fild.fl = FDS_CENTER | FDS_OPEN_DIALOG;

    char name[80+1];
    GetDlgItemText(idname,name,80);
    char title[128+1];
    sprintf(title,"Tool #%s -- %s",toolnum,name);
                                                    // FDS_* flags
    fild.pszTitle = title;                          // Dialog title string
    strcpy(fild.szFullFile, pszFullFile);           // Initial path,
                                                    // file name, or
                                                    // file filter

    /*****************************************************************/
    /* Display the dialog and get the file                           */
    /*****************************************************************/
    HWND  hwndDlg = WinFileDlg(HWND_DESKTOP, HWindow, &fild);
    if (hwndDlg && (fild.lReturn == DID_OK))
    {
        SetDlgItemText(id,fild.szFullFile);
    }
}



//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE1
//----------------------------------
void SetTool::EvClickBrowse1(void)
{
    Browse(ID_TOOL1PATH,ID_TOOL1NAME, "1");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE2
//----------------------------------
void SetTool::EvClickBrowse2(void)
{
    Browse(ID_TOOL2PATH,ID_TOOL2NAME,"2");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE3
//----------------------------------
void SetTool::EvClickBrowse3(void)
{
    Browse(ID_TOOL3PATH,ID_TOOL3NAME,"3");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE4
//----------------------------------
void SetTool::EvClickBrowse4(void)
{
    Browse(ID_TOOL4PATH,ID_TOOL4NAME,"4");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE5
//----------------------------------
void SetTool::EvClickBrowse5(void)
{
    Browse(ID_TOOL4PATH,ID_TOOL5NAME,"5");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE6
//----------------------------------
void SetTool::EvClickBrowse6(void)
{
    Browse(ID_TOOL6PATH,ID_TOOL6NAME,"6");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE7
//----------------------------------
void SetTool::EvClickBrowse7(void)
{
    Browse(ID_TOOL7PATH,ID_TOOL7NAME,"7");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_BROWSE8
//----------------------------------
void SetTool::EvClickBrowse8(void)
{
    Browse(ID_TOOL8PATH,ID_TOOL8NAME,"8");
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void SetTool::EvClickHelp(void)
{
    Help("Tools");
}


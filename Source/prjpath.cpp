#include    "def.h"
#define INCL_WINSHELLDATA
#define INCL_DOSFILEMGR
#define INCL_DOSMISC        // For DosError()
#define     INCL_WINSTDFILE         // FILEDLG FileOpen Window Standard File Functions
#include    <os2.h>
//
#include    <stdio.h>
#include    <owl\dialog.h>
#include    "resource.h"
#include    <owl\listbox.h>
#include    <owl\combobox.h>
#include    <dir.h>
#include    "prjpath.h"
#include    "startapp.h"
#include    "parse.h"
#include    "ProjFile.h"
#include    "ProjWin.h"


extern char MyProjectPath[MAXPATHSIZE];


//----------------------------------
// MESSAGE MAP FOR PrjPath
//----------------------------------
DEFINE_RESPONSE_TABLE1(PrjPath,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_LBN_DBLCLK(ID_PRJDIR,EvDblClickPrjdir),
    EV_BN_CLICKED(ID_MAKEDIR,EvClickMakedir),
    EV_CBN_SELCHANGE(ID_DRIVES,EvSelChangeDrives),
    EV_BN_CLICKED(ID_DRIVECZ,EvClickDrivecz),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


static char HiDrive;



//----------------------------------
// Constructor
//----------------------------------
PrjPath::PrjPath(TWindow *parent) : TDialog(parent, ID_DLGPRJPATH)
{
    m_Prjdir = new TListBox(this,ID_PRJDIR);
    getcwd(m_origdir,sizeof(m_origdir));
    m_Drives = new TComboBox(this,ID_DRIVES);
    if(HiDrive < 'C' || HiDrive > 'Z')
    {
        HiDrive = 'Z';
    }
}




//----------------------------------
// Destructor
//----------------------------------
PrjPath::~PrjPath()
{
    delete m_Prjdir;
    delete m_Drives;
}



//----------------------------------
// Cancel Button
//----------------------------------
void PrjPath::EvClickCancel(void)
{
    ChPath(m_origdir);
    char s[MAXPATHSIZE];
    getcwd(s,sizeof(s));
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    m_Drives->SetSelIndex(-1);  // deselect drive
    char s2[2+1];
    strncpy(s2,m_origdir,2); s2[3]=NULL;    // s2="x:"
    strupr(s2);
    int i = m_Drives->FindString(s2,-1);
    m_Drives->SetSelIndex(i);   // changes causes new dir to be scanned
    if(i == -1)
    {
        DoDirectories();        // oops!
    }
}

extern ProjWin *ProjectWindowPtr;


//----------------------------------
// OK Button Pressed
//----------------------------------
void PrjPath::EvClickOk(void)
{
    char s[512+1];
    GetDlgItemText(ID_INFO,s,sizeof(s));
    ChPath(s);  // 1.07
    //
    char recalled[512+1];
    RecallProjectPath(recalled,FALSE);
    //
    if(stricmp(s,recalled) != 0)
    {   // DIFFERENT PATH - ASK TO REMEMBER
        char msg[512+1];
        sprintf(msg,"Path: %s\n\n"
                    "Would You Like To Remember This Path "
                    "To Start Professor Owl Here Next Time ?\n\n",s);

        if(MessageBox(msg,"Remember Path",MB_YESNO) == IDYES)
        {
            RememberProjectPath();
        }
        else // No!
        {
            if(recalled[1] == ':') // If There Is A Remembered Path
            {
                if(MessageBox("Erase Remembered Path?",recalled,MB_YESNOno) == IDYES)
                {
                    ForgetProjectPath();
                }
            }
        }
    }

    getcwd(MyProjectPath,sizeof(MyProjectPath));
    if( ProjectWindowPtr )  // Project Window is Open - reload it!
    {
        ProjectWindowPtr->ReloadFile();
    }
    CmOk();
}



//----------------------------------
// Init Dialog
//----------------------------------
BOOL PrjPath::EvInitDialog(HWND hWndFocus)
{
    BOOL Rc = TDialog::EvInitDialog(hWndFocus);
    char s[MAXPATHSIZE];
    getcwd(s,sizeof(s));
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    ScanDrives();
    return(Rc);
}

void PrjPath::DoDirectories(void)
{
    m_Prjdir->ClearList();
    //
    struct ffblk dta;
    int done = findfirst("*.*", &dta, FA_DIREC);
    while( !done )
    {
        if(dta.ff_attrib & FA_DIREC && (strcmp(dta.ff_name,".") != 0) )
        {
            m_Prjdir->InsertString(dta.ff_name,-2);
        }
        done = findnext(&dta);
    }
}


//----------------------------------
// Event EV_LBN_DBLCLK for ID_PRJDIR
//----------------------------------
void PrjPath::EvDblClickPrjdir(void)
{
    char s[MAXPATHSIZE];
    m_Prjdir->GetSelString(s,sizeof(s));
    chdir(s);
    getcwd(s,sizeof(s));
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    DoDirectories();
}




//----------------------------------
// Event EV_BN_CLICKED for ID_MAKEDIR
//----------------------------------
void PrjPath::EvClickMakedir(void)
{
    char s[400+1];
    GetDlgItemText(ID_DIRNAME,s,sizeof(s));
    mkdir(s);
    chdir(s);
    getcwd(s,sizeof(s));
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    DoDirectories();
}


//----------------------------------
// Event EV_CBN_SELCHANGE for ID_DRIVES
//----------------------------------
void PrjPath::EvSelChangeDrives(void)
{
    char s[MAXPATHSIZE];
    m_Drives->GetSelString(s,sizeof(s));
    s[1] = NULL;
    setdisk(s[0] - 'A');
    getcwd(s,sizeof(s));
    PrettyPathFile(s);
    SetDlgItemText(ID_INFO,s);
    DoDirectories();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DRIVECZ
//----------------------------------
void PrjPath::EvClickDrivecz(void)
{
    HiDrive = 'Z';
    ScanDrives();
}


//----------------------------------
//  'User Function' private: ScanDrives
//----------------------------------
void PrjPath::ScanDrives(void)
{
    char s[MAXPATHSIZE];
    // GET DRIVES
    m_Drives->Clear();
    m_Drives->ClearList();
    char max;
    DosError(2);        // NO ANOYING  POPUP ERROR DRIVE NOT READY !!
    for(int drv = 'C'; drv<=HiDrive; drv++)
    {
        struct
        {
            unsigned long serno;
            unsigned char vollen;
            char volume[MAXPATHSIZE];
        }infobuf;
        infobuf.vollen = 0;
        infobuf.volume[0] = NULL;
        int rc = DosQueryFSInfo(drv-'@',2,&infobuf,sizeof(infobuf));
        if(rc == 0 || infobuf.vollen > 0 )
        {
            max = drv;
            sprintf(s,"%c:  [%s]",drv,infobuf.volume);
            m_Drives->AddString(s);
        }
    }
    DosError(1);    // Allow Popup excecptions again !!!
    HiDrive = max;  // Next Time Scan C-HiDrive skipping CD-ROMS
    sprintf(s,"C: - %c:",HiDrive);
    //
    // select current Disk Drive
    char s2[2+1];
    strncpy(s2,m_origdir,2); s2[2]=NULL;    // s2="x:"
    strupr(s2);
    int i = m_Drives->FindString(s2,-1);
    m_Drives->SetSelIndex(i);
    SetDlgItemText(ID_HIDRIVE,s);
    //
    DoDirectories();
}

//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void PrjPath::EvClickHelp(void)
{
    Help("Project Directory");
}


extern char MyHomePath[];

void RememberProjectPath(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    char s[512];
    getcwd(s,sizeof(s));
    PrfWriteProfileString(hini,"profsowl","ProjectPath",s);
    PrfCloseProfile(hini);
}

void ForgetProjectPath(void)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    PrfWriteProfileString(hini,"profsowl","ProjectPath",".\\");
    PrfCloseProfile(hini);
}

void RecallProjectPath(char *rs, int ChDir)
{
    char path[400+1];
    sprintf(path,"%s\\PROFOWL.INI",MyHomePath);
    HAB hab;
    HINI  hini = PrfOpenProfile(hab,path);
    char s[MAXPATHSIZE];
    PrfQueryProfileString(hini,"profsowl","ProjectPath",".\\",s,sizeof(s));
    PrfCloseProfile(hini);

    if(ChDir && s[1] == ':')
    {
        ChPath(s);
    }

    if(rs)
    {
        strcpy(rs,s);
    }
}




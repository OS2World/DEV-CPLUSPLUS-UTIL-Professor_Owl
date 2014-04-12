#include    "def.h"
#define     INCL_WINMESSAGEMGR
#define     INCL_WINSWITCHLIST
#define     INCL_DOSMEMMGR
#define     INCL_DOSERRORS
#include    <os2.h>
//
#include    <stdio.h>
#include    <string.h>
#include    <dir.h>
#include    <owl\dialog.h>
#include    "resource.h"
#include    "tools.h"
#include    "settool.h"
#include    "startapp.h"
#include    "warnbeep.h"

TOOLNAMEPATH UserTools[8+1];

extern char MyHomePath[];

static long Happ[8+1];   // 1..8 index

//----------------------------------
// MESSAGE MAP FOR Tools
//----------------------------------
DEFINE_RESPONSE_TABLE1(Tools,TDialog)
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_TOOL1,EvClickTool1),
    EV_BN_CLICKED(ID_TOOL2,EvClickTool2),
    EV_BN_CLICKED(ID_TOOL3,EvClickTool3),
    EV_BN_CLICKED(ID_TOOL4,EvClickTool4),
    EV_BN_CLICKED(ID_TOOL5,EvClickTool5),
    EV_BN_CLICKED(ID_TOOL6,EvClickTool6),
    EV_BN_CLICKED(ID_TOOL7,EvClickTool7),
    EV_BN_CLICKED(ID_TOOL8,EvClickTool8),
    EV_MESSAGE(WM_APPTERMINATENOTIFY,EvTerm),
    EV_BN_CLICKED(ID_SETUP,EvClickSetup),
    EV_WM_TIMER,
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
Tools::Tools(TWindow *parent, int tooln) : TDialog(parent, ID_TOOLDLG)
{
    m_Invoke = tooln;
}

//----------------------------------
// Destructor
//----------------------------------
Tools::~Tools()
{
}

void Tools::Set(char *hfile, char *cfile, long linenum)
{
    strncpy(m_Hfile,hfile,sizeof(m_Hfile));
    strncpy(m_Cfile,cfile,sizeof(m_Cfile));
    m_LineNum = linenum;
}


//----------------------------------
// Cancel Button
//----------------------------------
void Tools::EvClickCancel(void)
{
    if( Happ[1] || Happ[2] || Happ[3] || Happ[4]
        || Happ[5] || Happ[6] || Happ[7] || Happ[8])
    {
        WarnBeep();
        if( MessageBox("One Or More Tools Still Running\n"
                       "Close Tools Dialog Anyway ?",
                       "Notice",MB_YESNOno) == IDNO)
        {
            return; // NO = leave open ... dont exit ...
        }
    }
    CmCancel();
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL Tools::EvInitDialog(HWND hWndFocus)
{
    BOOL Rc = TDialog::EvInitDialog(hWndFocus);
    DrawButtons();
    //
    Happ[1] = 0;
    Happ[2] = 0;
    Happ[3] = 0;
    Happ[4] = 0;
    Happ[5] = 0;
    Happ[6] = 0;
    Happ[7] = 0;
    Happ[8] = 0;
    Invoke(m_Invoke);
    //
    return Rc;
}


//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL1
//----------------------------------
void Tools::EvClickTool1(void)
{
    RunTool(1);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL2
//----------------------------------
void Tools::EvClickTool2(void)
{
    RunTool(2);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL3
//----------------------------------
void Tools::EvClickTool3(void)
{
    RunTool(3);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL4
//----------------------------------
void Tools::EvClickTool4(void)
{
    RunTool(4);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL5
//----------------------------------
void Tools::EvClickTool5(void)
{
    RunTool(5);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL6
//----------------------------------
void Tools::EvClickTool6(void)
{
    RunTool(6);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL7
//----------------------------------
void Tools::EvClickTool7(void)
{
    RunTool(7);
}

//----------------------------------
// Event EV_BN_CLICKED for ID_TOOL8
//----------------------------------
void Tools::EvClickTool8(void)
{
    RunTool(8);
}


//
// input "e.exe $hfile -x" --> "e.exe myfile.h -x"
// Macro Names are:
//  $HFILE  header file
//  $CFILE  cpp file
//  $LINE   line number in MLE editor
//  $PATH   Current Path
void Tools::ExpandDollar(char *in, char *s)
{
    ExpandToolDollar(in,s,m_Hfile,m_Cfile,m_LineNum);
}





//
// Start TOOL #n if possible
//  stash HAPP in array
//
void Tools::RunTool(int n)
{
    if(n < 1 || n > 8)
        return;

    if(Happ[n])
    {
        MakePIDActiveWindow(n);
        return;
    }

    //
    // No Path For Tool ?? Then Run Setup !!
    //
    if(UserTools[n].Path[0] == NULL)
    {
        EvClickSetup();
        return;
    }

    char s[512];
    ExpandDollar(UserTools[n].Path,s);
    char *p = strchr(s,' ');
    // Find space and pass PROG & PARAM
    if(p)
    {
        *p++ = NULL;

        while(*p == ' ')
            p++;
    }
    if(!p)
        p = "";
    char Title[128+1];
    char TitleSufix[128+1];
    ExpandDollar(UserTools[n].Name,Title);
    sprintf(TitleSufix," (Prof_Owl_Tool#%d)",n);
    strcat(Title,TitleSufix);
    //
    char Dir[MAXPATHSIZE];
    ExpandDollar(UserTools[n].Dir,Dir);
    Happ[n] = StartApp(HWindow,Title,Dir,s,p);
}


//  CALLED WHEN THE TOOL TERMINATES EXECUTION !
//  Tool Application Terminated -- close dlg if none running
//
LRESULT Tools::EvTerm(WPARAM pid, LPARAM)
{
    for(int i=1; i<=8; i++)
    {
        if(Happ[i] == pid)
            Happ[i] = 0;
    }
    if(Happ[1]==0 && Happ[2]==0 && Happ[3]==0 && Happ[4]==0
     && Happ[5]==0 && Happ[6]==0 && Happ[7]==0 && Happ[8]==0)
    {
        CmOk(); // No Tools Running
    }
    return 0;
}


//----------------------------------
// Event EV_BN_CLICKED for ID_SETUP
//----------------------------------
void Tools::EvClickSetup(void)
{
    SetTool S(this);
    S.Execute();
    DrawButtons();
}


//----------------------------------
//  'User Function' private: DrawButtons
//----------------------------------
void Tools::DrawButtons(void)
{
    char s[MAXPATHSIZE];
    ExpandDollar(UserTools[1].Name,s);
    SetDlgItemText(ID_TOOL1,s);
    ExpandDollar(UserTools[2].Name,s);
    SetDlgItemText(ID_TOOL2,s);
    ExpandDollar(UserTools[3].Name,s);
    SetDlgItemText(ID_TOOL3,s);
    ExpandDollar(UserTools[4].Name,s);
    SetDlgItemText(ID_TOOL4,s);
    ExpandDollar(UserTools[5].Name,s);
    SetDlgItemText(ID_TOOL5,s);
    ExpandDollar(UserTools[6].Name,s);
    SetDlgItemText(ID_TOOL6,s);
    ExpandDollar(UserTools[7].Name,s);
    SetDlgItemText(ID_TOOL7,s);
    ExpandDollar(UserTools[8].Name,s);
    SetDlgItemText(ID_TOOL8,s);
}

void Tools::Invoke(int n)
{
    if(n >=1 && n <= 8)
    {
        m_Invoke = n;
        SetTimer(1,250);
    }
}


//----------------------------------
// Virtual Function
//----------------------------------
BOOL Tools::DialogFunction(UINT message, WPARAM wParam, LPARAM lParam)
{
    #define KC_KEYUP        0x40
    #define KC_VIRTUALKEY   0x02
    #define KC_SCANCODE     0x04
    #define KC_SHIFT        0x08
    #define KC_CONTROL      0x10
    #define KC_ALT          0x20
    #define KC_ALLSHIFT     0x38
    if (message == 0x007A && !(wParam & KC_KEYUP) )
    {   // KEY WENT DOWN
        int fs = wParam & 0xFFFF;
        int vk = (lParam >> 16) & 0xFF;
        if( (fs & (KC_VIRTUALKEY | KC_ALLSHIFT)) == (KC_CONTROL | KC_VIRTUALKEY))
        {
            switch(vk)
            {
                case VK_F1:
                    Invoke(1);
                break;

                case VK_F2:
                    Invoke(2);
                break;

                case VK_F3:
                    Invoke(3);
                break;

                case VK_F4:
                    Invoke(4);
                break;

                case VK_F5:
                    Invoke(5);
                break;

                case VK_F6:
                    Invoke(6);
                break;

                case VK_F7:
                    Invoke(7);
                break;

                case VK_F8:
                    Invoke(8);
                break;
            }
        }
    }
    return(0);
}




/*
 This example determines the number of items in the Task List, allocates
 memory for the required buffer, and then calls WinQuerySwitchList again to
 fill the buffer with the information about each item in the Task List.
*/
void MakePIDActiveWindow(long index)
{
    HAB      hab        = NULLHANDLE;
    static ULONG    cbItems    = 0,     // Compiler bug ! trashing ESI  Number of items in list
             ulBufSize  = 0;            /* Size of buffer for information */
    PVOID    pvBase     = NULL;         /* Pointer to local memory pool */
    PSWBLOCK pswblk     = NULL;         /* Pointer to information returned */
    APIRET   rc         = NO_ERROR;     /* Return code from Dos APIs */
    // Allocate a large block of memory (uncommitted) and make
    // it available for suballocation.  This allows the system
    // to commit memory only when it is actually needed.
    #define MEMPOOL  20000   // Size of local memory pool area
    ///
    rc = DosAllocMem( &pvBase, MEMPOOL, PAG_READ | PAG_WRITE );
    if(rc)
    {
        DosBeep(1000,1000);
        return;
    }

    rc = DosSubSetMem( pvBase, DOSSUB_INIT | DOSSUB_SPARSE_OBJ, MEMPOOL );
    if(rc)
    {
        DosBeep(2000,2000);
        return;
    }
    // Determine the number of items in the list and calculate
    // the size of the buffer needed.
    cbItems = WinQuerySwitchList(hab, NULL, 0);
    ulBufSize = (cbItems * sizeof(SWENTRY)) + sizeof(HSWITCH);
    // Allocate the buffer from our memory pool
    rc = DosSubAllocMem( pvBase, (PPVOID) &pswblk, ulBufSize);
    if(rc)
    {
        DosBeep(3000,3000);
        return;
    }
    // Call WinQuerySwitchList again to fill our buffer with information
    cbItems = WinQuerySwitchList(hab, pswblk, ulBufSize);
    ///
    char KeyTitle[128+1];
    sprintf(KeyTitle,"(Prof_Owl_Tool#%ld)",index);

    for(long Cnt = cbItems-1; Cnt >= 0; Cnt--)
    {
        SWENTRY TList;
        TList= pswblk->aswentry[Cnt];
                // List.swctl.hwnd              /*  Window handle. */
                // TList.swctl.hwndIcon         /*  Window-handle icon. */
                // TList.swctl.hprog            /*  Program handle. */
                // TList.swctl.idProcess        /*  Process identity. */
                // TList.swctl.idSession        /*  Session identity. */
                // TList.swctl.uchVisibility    /*  Visibility: */
                // TList.swctl.fbJump           /*  Jump indicator: */
                // TList.swctl.szSwtitle        /*  Switch-list control block title (null-terminated). */

        if( strstr(TList.swctl.szSwtitle, KeyTitle) != NULL)
        {
            WinShowWindow(TList.swctl.hwnd,TRUE);
            HSWITCH  hs = WinQuerySwitchHandle(TList.swctl.hwnd, 0);
            WinSwitchToProgram(hs);
            break;
        }
    }
    DosFreeMem(pvBase);
}


//----------------------------------
// Event EV_WM_TIMER for ID_TOOLDLG
//----------------------------------
void Tools::EvTimer(UINT timerId)
{
    TDialog::EvTimer(timerId);
    KillTimer(timerId);
    if(m_Invoke)
    {
        RunTool(m_Invoke);
        m_Invoke = 0;
    }
}








//
// Given a hfile and cfile expand the 'in' string to 's'
// where $cfile $hfile $home $path $line expand correctly
//
void ExpandToolDollar(char *in, char *s, char *hfile, char *cfile, int LineNum)
{
    char tail[512+1];
    strcpy(s, in);
    char *p= strchr(s,'$');
    int flag = 1;

    while(flag)
    {
        flag = 0;
        if( p && strnicmp(p,"$hfile",6)==0)
        {
            strcpy(tail,p+6);
            strcpy(p,hfile);
            strcat(p,tail);
            flag = 1;
        }
        p= strchr(s,'$');
        if( p && strnicmp(p,"$cfile",6)==0)
        {
            strcpy(tail,p+6);
            strcpy(p,cfile);
            strcat(p,tail);
            flag = 1;
        }
        p= strchr(s,'$');
        if( p && strnicmp(p,"$line",5)==0)
        {
            strcpy(tail,p+5);
            sprintf(p,"%ld",LineNum);
            strcat(p,tail);
            flag = 1;
        }
        p= strchr(s,'$');
        if( p && strnicmp(p,"$home",5)==0)
        {
            strcpy(tail,p+5);
            sprintf(p,"%s",MyHomePath);
            strcat(p,tail);
            flag = 1;
        }
        p= strchr(s,'$');
        if( p && strnicmp(p,"$path",5)==0)
        {
            strcpy(tail,p+5);
            char path[257+1];
            getcwd(path,sizeof(path)-1);
            int l = strlen(path);
            if(l > 1 && path[l-1] == '\\')
            {
                path[l-1] = NULL;
            }
            sprintf(p,"%s",path);
            strcat(p,tail);
            flag = 1;
        }
    }
}



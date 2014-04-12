#include    "def.h"
#define NULL 0
#include    <os2.h>
#include    <owl\dialog.h>
#include    "resource.h"
#include    <owl\listbox.h>
#include    <owl\combobox.h>
#include    <owl\edit.h>
#include    <stdio.h>
#include    "vardlg.h"
#include    "varadd.h"
#include    "cl.h"
#include    "parse.h"
#include    "warnbeep.h"

//----------------------------------
// MESSAGE MAP FOR VarDlg
//----------------------------------
DEFINE_RESPONSE_TABLE1(VarDlg,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_ADDVAR,EvClickAddvar),
    EV_BN_CLICKED(ID_DELVAR,EvClickDelvar),
    EV_BN_CLICKED(ID_RESCAN,EvClickRescan),
    EV_WM_PAINT,
    EV_LBN_SELCHANGE(ID_LISTVARS,EvSelChangeListvars),
    EV_LBN_DBLCLK(ID_LISTVARS,EvDblClickListvars),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
VarDlg::VarDlg(TWindow *parent) : TDialog(parent, ID_VARDLG)
{
    m_Scan = TRUE;
    m_ListVars = new TListBox(this,ID_LISTVARS);
}

//----------------------------------
// Destructor
//----------------------------------
VarDlg::~VarDlg()
{
    delete m_ListVars;
}

//----------------------------------
// Event EV_WM_PAINT for ID_VARDLG
//----------------------------------
void VarDlg::EvPaint(void)
{
    TDialog::EvPaint();
    if(m_Scan)
    {
        m_Scan = FALSE;
        Rescan();
    }
}



//----------------------------------
// Cancel Button
//----------------------------------
void VarDlg::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void VarDlg::EvClickOk(void)
{
    CmOk();
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL VarDlg::EvInitDialog(HWND hWndFocus)
{
    m_Scan = TRUE;
    BOOL Rc = TDialog::EvInitDialog(hWndFocus);
    //
    char s[80+512];
    sprintf(s,"Class %s ( %s, %s )",m_Class,m_Hfile,m_Cfile);
    SetDlgItemText(ID_INFO,s);
    //
    return(Rc);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_ADDVAR
//----------------------------------
void VarDlg::EvClickAddvar(void)
{
    VarAddDlg X(this);
    X.Pass(m_Hfile,m_Cfile,m_Class,m_Objects);
    if( X.Execute() == IDOK)
    {
        Rescan();
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DELVAR
//----------------------------------
void VarDlg::EvClickDelvar(void)
{
    char s[128+1];
    s[0] = NULL;
    m_ListVars->GetSelString(s,sizeof(s)-1);
    if( m_ListVars->GetSelIndex() < 0 || s[0] == NULL)
    {
        WarnBeep();
        MessageBox("No Variable Selected","Warning",MB_CANCEL);
        return;
    }
    char msg[700+1];
    sprintf(msg,"%s %s\n\n"
            "From Files: %s & %s",
            m_Type,m_Symbol,
            m_Hfile,m_Cfile);
    //
    WarnBeep();
    if( MessageBox(msg,"Confirm Delete",MB_YESNOno) == IDYES )
    {
        Cproto X;
        X.DeleteLineFromFile(m_Cfile,m_StrCon);
        X.DeleteLineFromFile(m_Cfile,m_StrDes);
        X.DeleteLineFromFile(m_Hfile,m_StrH);
        Rescan();
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_RESCAN
//----------------------------------
void VarDlg::EvClickRescan(void)
{
    Rescan();
}


void VarDlg::Rescan(void)
{
    SetDlgItemText(ID_INFOSYM,"Scanning...");
    SetDlgItemText(ID_INFOH,"");
    SetDlgItemText(ID_INFOCON,"");
    SetDlgItemText(ID_INFODES,"");
    //
    m_ListVars->ClearList();
    Cproto X;
    X.ScanVariables(m_Hfile,m_Cfile,m_Class,m_ListVars);
    SetDlgItemText(ID_INFOSYM,"");
    m_ListVars->SetSelIndex(0);
}


void VarDlg::SetHCfiles(char *Class, char *hfile, char *cfile, TListBox *obj)
{
    strncpy(m_Class,Class,sizeof(m_Class));
    strncpy(m_Hfile,hfile,sizeof(m_Hfile));
    strncpy(m_Cfile,cfile,sizeof(m_Cfile));
    m_Objects = obj;
}



//----------------------------------
// Event EV_LBN_SELCHANGE for ID_LISTVARS
//----------------------------------
void VarDlg::EvSelChangeListvars(void)
{
    char s[512+1];
    s[0] = NULL;
    m_ListVars->GetSelString(s,sizeof(s)-1);
    // Parse List Box String
    char *Tok[] =
    {
        "=",
        "new",
        "(",
        "this",
        ",",
        ")",
        ";",
        NULL
    };
    char *Tok2[] =  { "=",";", NULL };
    //
    m_StrH[0] = m_StrCon[0] = m_StrDes[0] = m_Symbol[0] = m_Type[0] = NULL;
    //
    if(StringOrder(s,Tok) )
    {
        char *p = strstr(s,"new");
        if(p)
        {
            GetThisSymbol(s,m_Symbol);
            GetThisSymbol(p+4,m_Type);
            char fname[MAXPATHSIZE];
            char sec1[MAXPATHSIZE];
            sprintf(sec1,"class %s",m_Class);
            ExactGrepFiles(sec1,"};",m_Symbol,m_Type,";",m_Hfile,fname,m_StrH);
            // CONSTRUCTOR USAGE
            sprintf(sec1,"%s::%s",m_Class,m_Class);
            if( ! ExactGrepFiles(sec1,"}\n",m_Symbol,m_Type,"new",m_Cfile,fname,m_StrCon) )
            {
                sprintf(sec1,"%s :: %s",m_Class,m_Class);   // alternate format (spaces)
                ExactGrepFiles(sec1,"}\n",m_Symbol,m_Type,"new",m_Cfile,fname,m_StrCon);
            }
            // DESTRUCTOR USAGE
            sprintf(sec1,"%s::~%s",m_Class,m_Class);
            if( ! ExactGrepFiles(sec1,"}\n",m_Symbol,"delete",";",m_Cfile,fname,m_StrDes) )
            {
                sprintf(sec1,"%s :: ~%s",m_Class,m_Class);   // alternate format (spaces)
                if( ! ExactGrepFiles(sec1,"\n",m_Symbol,"delete",";",m_Cfile,fname,m_StrDes) )
                {
                    sprintf(sec1,"%s :: ~ %s",m_Class,m_Class);   // alternate format (spaces)
                    ExactGrepFiles(sec1,"}\n",m_Symbol,"delete",";",m_Cfile,fname,m_StrDes);
                }
            }
        }
    }
    else if(StringOrder(s,Tok2) ) // SIMPLE VARIABLES  'm_Var = 0;'
    {
        char *p = strstr(s,"=");
        if(p)
        {
            GetThisSymbol(s,m_Symbol);
            char fname[MAXPATHSIZE];
            char sec1[MAXPATHSIZE];
            sprintf(sec1,"class %s",m_Class);
            ExactGrepFiles(sec1,"};",m_Symbol,m_Type,";",m_Hfile,fname,m_StrH);
            strcpy(m_Type,m_StrH);
            // CONSTRUCTOR USAGE
            sprintf(sec1,"%s::%s",m_Class,m_Class);
            if( ! ExactGrepFiles(sec1,"}\n",m_Symbol,"=",";",m_Cfile,fname,m_StrCon) )
            {
                sprintf(sec1,"%s :: %s",m_Class,m_Class);   // alternate format (spaces)
                ExactGrepFiles(sec1,"}\n",m_Symbol,"=",";",m_Cfile,fname,m_StrCon);
            }
            // DESTRUCTOR USAGE
            strcpy(m_StrDes,"<not used>");
        }
    }

    // avoid remove variable from string such as "Var1 is unsigned short Var1"
    char *q1 = strstr(m_Type,m_Symbol);
    if(q1)
        *q1 = NULL;
    if(m_Type[0])
        sprintf(s,"Symbol: %s is %s",m_Symbol,m_Type);
    else
        sprintf(s,"Variable: %s",m_Symbol);

    PrettyString(s);
    SetDlgItemText(ID_INFOSYM,s);
    //
    sprintf(s,"Header: %s",m_StrH);
    PrettyString(s);
    SetDlgItemText(ID_INFOH,s);
    //
    sprintf(s,"Constructor: %s",m_StrCon);
    PrettyString(s);
    SetDlgItemText(ID_INFOCON,s);
    //
    sprintf(s,"Destructor: %s",m_StrDes);
    PrettyString(s);
    SetDlgItemText(ID_INFODES,s);
}



//----------------------------------
// Event EV_LBN_DBLCLK for ID_LISTVARS
//----------------------------------
void VarDlg::EvDblClickListvars(void)
{
    VarDlg::EvClickDelvar();
}


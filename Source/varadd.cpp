#include    "def.h"
#define NULL 0
#include    <os2.h>
#include    <owl\dialog.h>
#include    <owl\combobox.h>
#include    "resource.h"
#include    <owl\listbox.h>
#include    <owl\edit.h>
#include    "varadd.h"
#include    <stdio.h>
#include    "cl.h"
#include    "parse.h"
#include    "setupdat.h"
#include    "bigmess.h"
#include    "warnbeep.h"
#include    "input.h"
#include    "vardlg.h"


extern SETUPDATA    UserSetup;

#define MAXHISTORY  10
#define MAXSTRLEN   40
static char *HistoryUserData[MAXHISTORY];
static char HistoryUserDataBuffer[MAXHISTORY][MAXSTRLEN+1];



//----------------------------------
// MESSAGE MAP FOR VarAddDlg
//----------------------------------
DEFINE_RESPONSE_TABLE1(VarAddDlg,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_EN_CHANGE(ID_VARNAME,EvChangeVarname),
    EV_WM_PAINT,
    EV_CBN_EDITCHANGE(ID_IDNAME,EvEditChangeIdname),
    EV_BN_CLICKED(ID_SKIP,EvClickSkip),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
VarAddDlg::VarAddDlg(TWindow *parent) : TDialog(parent, ID_VARDLG2)
{
    m_ParentNeedsToRescan = FALSE;
    m_Scan = TRUE;
    m_Ids = new TListBox(this,ID_IDNAME);
    m_EdVarName = new TEdit(this,ID_VARNAME);
    m_Ppp = new TComboBox(this,ID_PPP);

    for(int i=0; i<MAXHISTORY; i++)
    {
        HistoryUserData[i] = HistoryUserDataBuffer[i];
    }
}


//----------------------------------
// Destructor
//----------------------------------
VarAddDlg::~VarAddDlg()
{
    delete m_Ids;
    delete m_EdVarName;
    delete m_Ppp;
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL VarAddDlg::EvInitDialog(HWND hWndFocus)
{
    m_VarName[0] = NULL;
    m_VarType[0] = NULL;
    m_VarId[0] = NULL;
    BOOL Rc = TDialog::EvInitDialog(hWndFocus);
    m_Ppp->ClearList();
    m_Ppp->AddString("private:");
    m_Ppp->AddString("protected:");
    m_Ppp->AddString("public:");
    m_Ppp->SetSelIndex(0);
    m_Scan = TRUE;
    return Rc;
}



//----------------------------------
// Cancel Button -- CLOSE Button !
//----------------------------------
void VarAddDlg::EvClickCancel(void)
{
    if( m_ParentNeedsToRescan )
    {
        CmOk(); // tell Parent to Rescan!
    }
    else
    {
        CmCancel(); // dont need to rescan
    }
}


static int IdIndex;
static char LastCfile[MAXPATHSIZE];
#define IDSINDEX    10


//----------------------------------
// OK Button Pressed
//----------------------------------
void VarAddDlg::EvClickOk(void)
{
    // bad variable name
    if(m_VarName[0] == NULL)
    {
        WarnBeep();
        MessageBox("Bad Variable Name","CAN NOT CREATE VARIABLE",MB_CANCEL);
        return;
    }

    char ppp[80+1];
    m_Ppp->GetSelString(ppp,sizeof(ppp));
    char found[MAXPATHSIZE];
    CheckDuplicate(found);
    if(found[0]!=NULL)
    {
        char title[300+1];
        sprintf(title,"VARIABLE %s FOUND IN %s -- PROCEES?",m_VarName,m_Hfile);
        WarnBeep();
        int n = MessageBox(found,title,MB_YESNOno);
        if(n == IDNO)
            return;
    }

    if(IdIndex == 0)
    {
        InputDialog D(this);
        D.Init("UserData","Enter User Defined Data Type",MAXHISTORY,HistoryUserData,m_VarId,MAXSTRLEN);
        if( D.Execute() != IDOK)
            return;
    }

    Cproto X;
    if(m_VarType[0])    // if type make pointer type
    {
        char s[MAXPATHSIZE];
        sprintf(s,"*%s",m_VarName);
        X.AddHdrVariable(m_Hfile,m_Class,ppp,m_VarType,s);
        // VAR = new ...
        char code[MAXPATHSIZE];
        sprintf(code,"%s%s = new %s(this,%s);",UserSetup.Tabs,m_VarName,m_VarType,m_VarId);
        X.WriteCode(m_Cfile,m_Class,m_Class,code,TRUE);
        // delete VAR
        char des[MAXPATHSIZE];// destructor name
        sprintf(des,"~%s",m_Class);
        sprintf(code,"%sdelete %s;",UserSetup.Tabs,m_VarName);
        X.WriteCode(m_Cfile,m_Class,des,code,TRUE);
        // Last But Not Least add proper OWL header
        char hdr[MAXPATHSIZE];
        GetHeaderForClass(hdr, m_VarType);
        X.AppendIncludeFile(m_Hfile,hdr);
    }
    else // just add to header file  "int myname;" // and constructor myname =0;
    {
        X.AddHdrVariable(m_Hfile,m_Class,ppp,m_VarId,m_VarName);
        char code[MAXPATHSIZE];
        sprintf(code,"%s%s = 0;",UserSetup.Tabs,m_VarName);
        X.WriteCode(m_Cfile,m_Class,m_Class,code,TRUE);
    }

    if(IdIndex >= IDSINDEX && IdIndex < (m_Ids->GetCount()-1))
    {
        IdIndex++;
    }
    m_ParentNeedsToRescan = TRUE;
    m_Ids->SetSelIndex(IdIndex);
}




//----------------------------------
// Event EV_WM_PAINT for ID_VARDLG2
//----------------------------------
void VarAddDlg::EvPaint(void)
{
    TDialog::EvPaint();
    if(m_Scan)
    {
        m_Scan = FALSE;
        int i = m_Objects->GetCount()-1;
        while(i > 1)
        {
            char s[MAXPATHSIZE];
            m_Objects->GetString(s,i);
            if(isalpha(s[0]) && strstr(s,"EV_COMMAND")==0 )
                m_Ids->InsertString(s,-2);
            i--;
        }

        m_Ids->InsertString("UserDefined",0);
        m_Ids->InsertString("BOOL",1);
        m_Ids->InsertString("short",2);
        m_Ids->InsertString("unsigned short",3);
        m_Ids->InsertString("long",4);
        m_Ids->InsertString("unsigned long",5);
        m_Ids->InsertString("char",6);
        m_Ids->InsertString("unsigned char",7);
        m_Ids->InsertString("int",8);
        m_Ids->InsertString("unsigned int",9);

        // Different C File start at Id Index !
        if(strcmp(m_Cfile,LastCfile) != 0)
        {
            IdIndex = IDSINDEX;
        }
        strcpy(LastCfile,m_Cfile);

        if(IdIndex > (m_Ids->GetCount()-1))
        {
            IdIndex = IDSINDEX;
            if(IdIndex > (m_Ids->GetCount()-1))
            {
                IdIndex = 1;
            }
        }
        m_Ids->SetSelIndex(IdIndex);
    }
}


//----------------------------------
// Pass Paramaters to Dialog !
//----------------------------------
void VarAddDlg::Pass(char *Hfile, char *Cfile, char *Class, TListBox *Objects)
{
    strncpy(m_Hfile,Hfile,sizeof(m_Hfile));
    strncpy(m_Cfile,Cfile,sizeof(m_Cfile));
    strncpy(m_Class,Class,sizeof(m_Class));
    m_Objects = Objects;
}



//----------------------------------
// Event EV_EN_CHANGE for ID_VARNAME
//----------------------------------
void VarAddDlg::EvChangeVarname(void)
{
    m_EdVarName->GetText(m_VarName,sizeof(m_VarName));
    int bad = FALSE;
    for(int i=0; m_VarName[i]; i++)
    {
        if(
            (i == 0 && !isalpha(m_VarName[0]) && m_VarName[i] != '_' )
            ||
            (i > 0 && !isalpha(m_VarName[i]) && !isdigit(m_VarName[i]) && m_VarName[i] != '_')
            )
        { // illegal char found in class name - remove it
            for(int j=i; m_VarName[j]; j++)
                m_VarName[j] = m_VarName[j+1];
            bad = TRUE;
        }
    }
    if(bad)
    {
        m_EdVarName->SetText(m_VarName);
    }
    UpdateInfo();
}

//----------------------------------
// Event EV_CBN_EDITCHANGE for ID_IDNAME
//----------------------------------
void VarAddDlg::EvEditChangeIdname(void)
{
    char s[MAXPATHSIZE];
    s[0] = NULL;
    m_Ids->GetSelString(s,sizeof(s));
    //
    char cons[MAXPATHSIZE];
    cons[0] = NULL;
    sscanf(s,"%s %s",m_VarId,cons);
    LookUpControlToOwl(cons,m_VarType);
    //
    IdIndex = m_Ids->GetSelIndex();

    if(IdIndex < IDSINDEX )
    {
        strcpy(m_VarId,s);  // so unsigned long - 2 strings as one name
        char vars[MAXPATHSIZE];
        sprintf(vars,"%sVariable",UserSetup.Memprefix);
        sprintf(m_VarName,"%sVariable",UserSetup.Memprefix);

        m_EdVarName->SetText(m_VarName);
    }
    else // Non INT Or LONG -- Control Type !
    {
        //
        // PICK A NICE DEFAULT NAME !
        //
        char vars[MAXPATHSIZE];
        strcpy(vars,m_VarId);
        strlwr(vars);
        char *q = vars;
        if(strncmp(vars,"id_",3) == 0) // was m_varId --> vars
        {
            q += 3;
        }
        *q = toupper(*q);
        sprintf(m_VarName,"%s%s",UserSetup.Memprefix,q); // vars --> q
        m_EdVarName->SetText(m_VarName);
    }

    //
    // Check For Duplicate Names !
    //
    char found[MAXPATHSIZE];
    int n = 2;
    char origname[MAXPATHSIZE];
    strcpy(origname,m_VarName);
    found[0] = NULL;
    CheckDuplicate(found);
    while(found[0] != NULL)
    {
        sprintf(m_VarName,"%s%d",origname,n++);
        CheckDuplicate(found);
    }
    m_EdVarName->SetText(m_VarName);
    ///
    UpdateInfo();
}

//
// UPDATE INFO TEXT PANEL AS EDIT NAME OR ID LIST CHANGES
//
void VarAddDlg::UpdateInfo(void)
{
    char msg[128];
    msg[0] = NULL;
    if(m_Ids->GetSelIndex() < IDSINDEX )
    {
        sprintf(msg,"%s %s;",m_VarId, m_VarName);
    }
    else if(m_VarName[0] && m_VarType[0] && m_VarId[0])
    {
        sprintf(msg,"%s=new %s(this,%s);",m_VarName,m_VarType,m_VarId);
    }
    SetDlgItemText(ID_INFO,msg);
}


//
// Check if m_VarName is found inside class in header file
//      return 1st found occurance - made pretty
//
void VarAddDlg::CheckDuplicate(char *rFound)
{
    *rFound = NULL;
    char fname[MAXPATHSIZE];
    char found[MAXPATHSIZE];
    // check if name is found in header already !
    if( ExactGrepFiles(m_Class,"}\n",m_VarName,"","",m_Hfile,fname,found) )
    {
        char *p = strstr(found,m_VarName);
        if(p)
        {
            char sym[MAXPATHSIZE];
            GetThisSymbol(p,sym);
            PrettyString(found);
            if(strcmp(sym,m_VarName) == 0)
            {
                strcpy(rFound,found);
                return;
            }
        }
    }
}



//
// Look up a control type (i.e. WC_LISTBOX WC_BUTTON)
// and return the class name in OWL used to access the
// control (i.e. TListBox TButton TComboBox)
//
void LookUpControlToOwl(char *con,char *rS)
{
    struct Str2
    {
        char *Control;
        char *Class;
    };
    Str2 Db[] = {
    {   "WC_BUTTON",    "TButton"   },
    {   "PUSHBUTTON",   "TButton"   },
    {   "WC_LISTBOX",   "TListBox"  },
    {   "WC_COMBOBOX",  "TComboBox"},
    {   "WC_STATIC",    "TStatic"   },
    {   "WC_SCROLLBAR", "TScrollBar"},
    {   "WC_MLE",       "TEdit" },
    {   "WC_ENTRYFIELD","TEdit" },
    {   NULL,NULL   }
    };

    *rS = NULL;
    for(int i = 0; Db[i].Control; i++)
    {
        if(strcmp(con,Db[i].Control) == 0)
        {
            strcpy(rS,Db[i].Class);
            return;
        }
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_SKIP
//----------------------------------
void VarAddDlg::EvClickSkip(void)
{
    if(IdIndex < (m_Ids->GetCount()-1))
    {
        IdIndex++;
    }
    m_Ids->SetSelIndex(IdIndex);
    EvEditChangeIdname();   // Force Update
}



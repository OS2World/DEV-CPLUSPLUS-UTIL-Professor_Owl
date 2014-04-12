#include    "def.h"
#define NULL    0
#include    <os2.h>
#include    <owl\window.h>
#include    <owl\combobox.h>
#include    <owl\dialog.h>
#include    <owl\edit.h>
#include    "resource.h"
#include    "userfunc.h"
#include    "setupdat.h"
#include    "parse.h"
#include    <stdio.h>
#include    "cl.h"
#include    "warnbeep.h"
#include    "startapp.h"


extern SETUPDATA    UserSetup;

//----------------------------------
// MESSAGE MAP FOR UserFunc
//----------------------------------
DEFINE_RESPONSE_TABLE1(UserFunc,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_DEFAULT,EvClickDefault),
    EV_EN_CHANGE(ID_RETYPE,EvChangeRetype),
    EV_EN_CHANGE(ID_FUNCNAME,EvChangeFuncname),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
UserFunc::UserFunc(TWindow *parent) : TDialog(parent, ID_USERFUNC)
{
    m_Scope = new TComboBox(this,ID_PPP);
    m_Retype = new TEdit(this,ID_RETYPE);
    m_FuncName = new TEdit(this,ID_FUNCNAME);
}

//----------------------------------
// Destructor
//----------------------------------
UserFunc::~UserFunc()
{
    delete m_Scope;
    delete m_Retype;
    delete m_FuncName;
}

//----------------------------------
// Init Dialog
//----------------------------------
BOOL UserFunc::EvInitDialog(HWND hWndFocus)
{
    char s[MAXPATHSIZE];
    sprintf(s,"Add Function to %s",m_Class);
    SetCaption(s);
    //
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
    m_Scope->AddString("private:");
    m_Scope->AddString("protected:");
    m_Scope->AddString("public:");
    EvClickDefault();
    return RetValue;
}

//----------------------------------
// Cancel Button Pressed
//----------------------------------
void UserFunc::EvClickCancel(void)
{
    CmCancel();
}

//----------------------------------
// OK Button Pressed
//----------------------------------
void UserFunc::EvClickOk(void)
{
    char ppp[32+1];
    char *vir = NULL;
    char Comment[80+1];
    char body[80+1];
    char s[80+1];
    char sRet[80+1];
    char sFun[80+1];
    char sArg[80+1];

    m_Retype->GetText(s,sizeof(s)); onespace(s,sRet);
    if(stricmp(sRet,"void") == 0)
        body[0] = NULL;
    else
        sprintf(body,"%sreturn((%s)0);",UserSetup.Tabs,sRet);
    //
    m_FuncName->GetText(s,sizeof(s));   onespace(s,sFun);
    //
    char *p = strchr(sFun,'(');
    if(p)
    {
        *p = NULL;
        p++;
        sprintf(sArg,"%s",p);
        p = strchr(sArg,')');
        if(p)
            *p = NULL;
    }
    else // no parens -- default void !
    {
        strcpy(sArg,"void");
    }
    m_Scope->GetString( ppp, m_Scope->GetSelIndex() );
    if(IsDlgButtonChecked(ID_VIRTUAL) )
    {
        vir = "virtual";
        sprintf(Comment,"Virtual 'User Function' %s %s",ppp,sFun);
    }
    else
    {
        sprintf(Comment," 'User Function' %s %s",ppp,sFun);
    }

    if(sRet[0] == NULL || sFun[0] == NULL || sArg[0] == NULL)
    {
        WarnBeep();
        MessageBox("Check Function: Name, Return, or Arguments","CAN NOT ADD FUNCTION",
            MB_ICONSTOP|MB_CANCEL);
    }
    else
    {
        Cproto X;
        X.AddFunction(m_Hfile,m_Cfile,m_Class,ppp,vir, sRet, sFun,
                        sArg, Comment, body);

        X.UnDoDefaultParms(sArg, s);    // return args as in cpp file
        sprintf(m_Result,"%s %s::%s(%s)",sRet,m_Class,sFun,s);
        CmOk();
    }
}


//----------------------------------
// return result "void MyClass::MyFunc(void)"
// of added function so parent class can search for it!
//----------------------------------
void UserFunc::GetResult(char *s)
{
    strcpy(s,m_Result);
}


//----------------------------------
// Event EV_BN_CLICKED for ID_DEFAULT
//----------------------------------
void UserFunc::EvClickDefault(void)
{
    SetDlgItemText(ID_RETYPE,"void");
    SetDlgItemText(ID_FUNCNAME,"UserExample(int a, int b=1, int c=2)");
    CheckDlgButton(ID_VIRTUAL,FALSE);
    CheckDlgButton(ID_VIRTUAL,FALSE);
    m_Scope->SetSelIndex(0);
}


void UserFunc::SetHCfiles(char *hfile, char *cfile, char *Class)
{
    strncpy(m_Class,Class,sizeof(m_Class));
    strncpy(m_Hfile,hfile,sizeof(m_Hfile));
    strncpy(m_Cfile,cfile,sizeof(m_Cfile));
}

//----------------------------------
// Event EV_EN_CHANGE for ID_RETYPE
//----------------------------------
void UserFunc::EvChangeRetype(void)
{
    char s[MAXPATHSIZE];
    m_Retype->GetText(s,sizeof(s));
    //
    char s2[MAXPATHSIZE];
    char *p2 = s2;
    int changed = FALSE;
    for(int i=0; i < strlen(s); i++)
    {
        int ok = TRUE;
        if(i == 0)
        {
            if(s[i] == ' ' || isdigit(s[i]) || ispunct(s[i]) )
                ok = FALSE;
            if(s[i] == '_')
                ok = TRUE;
        }
        else
        {
            if(ispunct(s[i]) || s[i] == ' ' )
                ok = FALSE;
            if(s[i] =='*' || s[i] == '&' || s[i] == ' ')
                ok = TRUE;
        }
        if(ok)
            *p2++ = s[i];
        else
            changed = TRUE;
    }
    *p2 = NULL;
    if( changed )
        m_Retype->SetText(s2);
}

//----------------------------------
// Event EV_EN_CHANGE for ID_FUNCNAME
//----------------------------------
void UserFunc::EvChangeFuncname(void)
{
    char sFun[MAXPATHSIZE];
    char sArg[MAXPATHSIZE];
    m_FuncName->GetText(sFun,sizeof(sFun));
    strcpy(sArg,"void");
    //
    char *p = strchr(sFun,'(');
    if(p)
    {
        *p = NULL;
        p++;
        if(*p != NULL)
            sprintf(sArg,"%s",p);
        p = strchr(sArg,')');
        if(p)
            *p = NULL;
    }
    //
    char s[MAXPATHSIZE];
    nospace(sFun,s);    strcpy(sFun,s);
    onespace(sArg,s);   strcpy(sArg,s);
    // allow "operator++()" type of functions
    if( p== NULL || (strncmp(sFun,"operator",8) != 0 && ValidateSymbol(sFun)) )
    {
        char s[MAXPATHSIZE];
        sprintf(s,"%s(%s)",sFun,sArg);
        onespace(s,sFun);
        m_FuncName->SetText(sFun);
    }
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void UserFunc::EvClickHelp(void)
{
    Help("User Functions");
}


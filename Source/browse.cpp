//===============================================
//
// This Header File Was Created On fri mar 22 23:21:19 1996
// This File Has Prototypes For Functions
// Contained In browse.Cpp
//
// The Class is Browse and the base class is TDialog
// Author Edward J. March Jr.
//
// New For Version 1.08
//===============================================
#include    "def.h"

#include    ".\Resource.h"
#include    <stdio.h>
#include    "dlgmsg.h"
#include    "Browse.h"
#include    "Parse.h"
#include    "startapp.h"
#include    "warnbeep.h"
#include    "vfun.h"
#include    "bigmess.h"

//---------------------------------------------------------------
// MESSAGE MAP FOR Browse
//---------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(Browse,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
    EV_LBN_DBLCLK(ID_MEMBER,EvDblClickMember),
    EV_CBN_EDITCHANGE(ID_CLASS,EvEditChangeClass),
    EV_BN_CLICKED(ID_COPY,EvClickCopy),
    EV_BN_CLICKED(ID_CLIPOS2,EvClickClipos2),
    EV_LBN_SELCHANGE(ID_MEMBER,EvSelChangeMember),
    EV_BN_CLICKED(ID_PARENT,EvClickParent),
END_RESPONSE_TABLE;


//---------------------------------------------------------------
// Constructor
//---------------------------------------------------------------
Browse::Browse(TWindow *parent) : TDialog(parent, ID_OWLBROWSE)
{
    m_Class = new TComboBox(this,ID_CLASS);
    m_Member = new TListBox(this,ID_MEMBER);
    m_Edit   = new TEdit(this,ID_EDIT);        // Editor
    m_Edit->isMulti = TRUE;
}



//---------------------------------------------------------------
// Destructor
//---------------------------------------------------------------
Browse::~Browse()
{
    delete m_Class;
    delete m_Member;
    delete m_Edit;
}


static int RememberClassIndex, RememberMemberIndex;

//---------------------------------------------------------------
// Init Dialog
//---------------------------------------------------------------
BOOL Browse::EvInitDialog(HWND hWndFocus)
{
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);

    m_Class->SetSelIndex(-1);    // so next setsel index will cause change event
    m_Class->Clear();            // clear item in window too!
    m_Class->ClearList();

    ParseFile("PROFOWL.DAT",NULL);
    ParseFile("PROFOWL.USR",NULL);
    m_Class->SetSelIndex(RememberClassIndex);
    m_Member->SetSelIndex(RememberMemberIndex);

    return(RetValue);
}




//---------------------------------------------------------------
// Cancel Button Pressed
//---------------------------------------------------------------
void Browse::EvClickCancel(void)
{
    CmCancel();
}

extern DlgMsg *TheDlgMsg;




//---------------------------------------------------------------
// Event EV_BN_CLICKED for ID_HELP
//---------------------------------------------------------------
void Browse::EvClickHelp(void)
{
    Help("Browse Code Snippets");
}


//---------------------------------------------------------------
// Event EV_LBN_DBLCLK for ID_MEMBER
//---------------------------------------------------------------
void Browse::EvDblClickMember(void)
{
    EvClickOk();
}

extern char MyHomePath[];

//---------------------------------------------------------------
//  'User Function' private: ParseFile
//---------------------------------------------------------------
void Browse::ParseFile(char *fname, char *sect)
{
    char path[400+1];
    sprintf(path,"%s\\%s",MyHomePath,fname);
    FILE *fi = fopen(path,"rt");
    if(fi)
    {
        while(!feof(fi))
        {
            char s[128];
            s[0] = NULL;
            fgets(s,sizeof(s),fi);
            TrimString(s);
            ///
            int l = strlen(s);
            if(sect == NULL)    // Parse out "[SECTION IN BRACKETS]"
            {
                if(l > 2 && s[0] == '[' && s[l-1] == ']')
                {
                    s[l-1] = NULL;
                    m_Class->InsertString(&s[1],-2);
                }
            }
            else if( l > 2 && s[0] == '[' && s[l-1] == ']'
                    && strncmp(&s[1],sect,strlen(sect)) == 0)   // Find ".title" --> ".END"
            {
                s[0] = NULL;
                int scan = TRUE;
                while( scan )
                {
                    s[0] = NULL;
                    long index = ftell(fi);
                    fgets(s,sizeof(s),fi);
                    if(s[0] == '.' && strnicmp(s,".END",4) != 0)
                    {
                        TrimString(s);
                        char ss[sizeof(s)+80];
                        sprintf(ss,"%-70.70s #%ld",&s[1],index);
                        m_Member->InsertString(ss,-2);
                    }
                    else if(s[0] == '[' || feof(fi) )
                    {
                        scan = FALSE;
                    }
                }
            }
        }
    }
    fclose(fi);
}



//---------------------------------------------------------------
//  'User Function' private: ParseFileCode
//---------------------------------------------------------------
int Browse::ParseFileCode(char *fname, long index, char *buffer)
{
    *buffer = NULL;
    char path[400+1];
    sprintf(path,"%s\\%s",MyHomePath,fname);

    FILE *fi = fopen(path,"rt");
    if(fi)
    {
        fseek(fi,index,SEEK_SET);
        char s[1024];   // max line length
        s[0] = NULL;
        fgets(s,sizeof(s),fi);
        TrimString(s);
        if(s[0] != '.')
        {
            fclose(fi);
            return 2;   // ERROR ! File Sync Index
        }

        int scan = TRUE;
        while( scan )
        {
            s[0] = NULL;
            fgets(s,sizeof(s),fi);
            if(strnicmp(s,".END",4) == 0 || s[0] == '[' || feof(fi) )
            {
                fclose(fi);
                return(0);  // SUCCESS
            }
            else // Code Data...
            {
                strcat(buffer,s);
            }
        }
    }
    fclose(fi);
    return(1);
}


//---------------------------------------------------------------
// Event EV_CBN_EDITCHANGE for ID_CLASS
//---------------------------------------------------------------
void Browse::EvEditChangeClass(void)
{
    char s[256];
    s[0] = NULL;
    m_Class->GetSelString(s,sizeof(s));

    m_Member->ClearList();
    ParseFile("PROFOWL.DAT",s);
    ParseFile("PROFOWL.USR",s);
    //
    char Hdr[80+1];
    GetHeaderForClass(Hdr,s);
    sprintf(s,"Header File %s",Hdr);
    SetDlgItemText(ID_INFO2,s);
}

//---------------------------------------------------------------
// Event EV_BN_CLICKED for ID_COPY
//---------------------------------------------------------------
void Browse::EvClickCopy(void)
{
    UINT s,e;
    m_Edit->GetSelection(s,e);
    if(s == e)    // If No Selection - Select All
    {
        m_Edit->SetSelection(0,-1);
    }
    m_Edit->Copy();
    m_Edit->SetSelection(s,e);
    m_Edit->SetFocus();
}

//---------------------------------------------------------------
// OK Button Pressed -- Insert Text Into Editor !
//---------------------------------------------------------------
void Browse::EvClickOk(void)
{
    EvClickCopy();
    TheDlgMsg->Paste();
}


int Browse::GetCode(char *buffer)
{
    *buffer = NULL;
    RememberMemberIndex = m_Member->GetSelIndex();
    RememberClassIndex = m_Class->GetSelIndex();
    //
    char s[128+1];
    m_Member->GetSelString(s,sizeof(s));
    char *p = &s[strlen(s)-1];
    while(*p != '#' && *p != NULL)
        p--;
    if(*p == '#')
    {
        long index = atol(p+1);

        int rc = ParseFileCode("PROFOWL.DAT",index,buffer);
        if(rc)
            rc = ParseFileCode("PROFOWL.USR",index,buffer);
        return(rc);
    }
    return(3);
}


//---------------------------------------------------------------
// Event EV_BN_CLICKED for ID_CLIPOS2
//---------------------------------------------------------------
void Browse::EvClickClipos2(void)
{
    StartApp(HWindow,"", ".\\", "CLIPOS2.EXE", "");
}


//---------------------------------------------------------------
// Event EV_LBN_SELCHANGE for ID_MEMBER
//---------------------------------------------------------------
void Browse::EvSelChangeMember(void)
{
    char buffer[17000+1];
    SetCursor(0,IDC_WAIT);
    int rc = GetCode(buffer);
    if(rc == 0)
    {
        int length = strlen(buffer);
        //
        m_Edit->EmptyUndoBuffer();
        m_Edit->ClearModify();
        m_Edit->Clear();
        int tab = 16;   // pixels
        m_Edit->SetTabStops(1,&tab);
        //
        char *p = m_Edit->LockBuffer(length+1);
        strcpy(p,buffer);
        p[length] = NULL;
        m_Edit->UnlockBuffer(p,TRUE);
        //
        // Position To Line #1
        //
        int fv = m_Edit->GetFirstVisibleLine();
        m_Edit->Scroll(-9999,-fv);
    }
    SetCursor(0,IDC_ARROW);
}


//---------------------------------------------------------------
// Event EV_BN_CLICKED for ID_PARENT
//---------------------------------------------------------------
void Browse::EvClickParent(void)
{
    char theclass[256];
    theclass[0] = NULL;
    m_Class->GetSelString(theclass,sizeof(theclass));
    ///
    char parent1[80+1];
    char parent2[80+1];
    FindParentClass( theclass, parent1, parent2 );

    if(parent1[0] && parent2[0])
    {
        char msg[128+1];
        sprintf(msg,"Parents of class '%s' are '%s' and '%s'",theclass,parent1,parent2);
        MessageBox(msg,"Two Parents");
    }

    if(parent1[0])
    {
        int i = m_Class->FindExactString(parent1, 0);
        if(i >= 0)
        {
            m_Class->SetSelIndex(i);
            m_Member->SetSelIndex(0);
        }
        if(parent2[0] == NULL)
        {
            char msg[128+1];
            sprintf(msg,"Parent of class '%s' is '%s'",theclass,parent1);
            MessageBox(msg,"One Parent");
        }
    }
}



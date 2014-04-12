//===============================================
// This Header File Was Created On sun feb 11 15:24:40 1996
// This File Has Prototypes For Functions
// Contained In makedlg.Cpp
//
// The Class is MakeDlg and the base class is TDialog
// Author Edward J. March Jr.
//===============================================
#include    <stdio.h>
#include    "def.h"
#include    "Resource.h"
#include    "Makedlg.h"
#include    "Create1.h"
#include    "projfile.h"
#include    "StartApp.h"

//----------------------------------
// MESSAGE MAP FOR MakeDlg
//----------------------------------
DEFINE_RESPONSE_TABLE1(MakeDlg,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
    EV_BN_CLICKED(ID_HELP,EvClickHelp),
    EV_BN_CLICKED(ID_EXAMPLE,EvClickExample),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
MakeDlg::MakeDlg(TWindow *parent, TListBox *Lb) : TDialog(parent, ID_MAKEDLG)
{
    ListBox = Lb;
    m_Name = new TEdit(this,ID_NAME);
    m_Outputpath = new TEdit(this,ID_OUTPUTPATH);
}



//----------------------------------
// Destructor
//----------------------------------
MakeDlg::~MakeDlg()
{
    delete m_Name;
    delete m_Outputpath;
}



//----------------------------------
// Init Dialog
//----------------------------------
BOOL MakeDlg::EvInitDialog(HWND hWndFocus)
{
   BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
   SetDlgItemText(ID_NAME,"MyApp.Mak");
   SetDlgItemText(ID_OUTPUTPATH,".");
   SetDlgItemText(ID_LINKSWITCHES,"/wdpl /Toe /aa");
   return(RetValue);
}


//----------------------------------
// Cancel Button Pressed
//----------------------------------
void MakeDlg::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void MakeDlg::EvClickOk(void)
{
    const int MAXITEMS = 256;
    char ResName[MAXPATHSIZE];
    ResName[0] = NULL;
    char *List[MAXITEMS+1];
    int l = ListBox->GetCount();
    if(l > MAXITEMS)
        l = MAXITEMS;

    int i = 0;
    for(short int index=0; index<l; index++)
    {
        char s[MAXPATHSIZE];
        ListBox->GetString(s,index);
        PrettyPathFile(s);
        //
        if(s[1] == ':')
        {
            PrettyPathFile(s);
            if(strstr(s,".rc") )
            {
                strcpy(ResName,s);
            }
            else if(strstr(s,".c") || strstr(s,".asm") )
            {
                List[i] = new char[strlen(s)+1];
                strcpy(List[i],s);
                i++;
            }
        }
    }
    List[i] = NULL;
    char MakeName[MAXPATHSIZE];
    GetDlgItemText(ID_NAME,MakeName,sizeof(MakeName));
    char ObjDir[MAXPATHSIZE];
    GetDlgItemText(ID_OUTPUTPATH,ObjDir,sizeof(ObjDir));
    //
    char LinkSwitches[MAXPATHSIZE];
    GetDlgItemText(ID_LINKSWITCHES,LinkSwitches,sizeof(LinkSwitches));
    //
    l = strlen(ObjDir);
    if(l > 1 &&ObjDir[l] == '\\')   // remove trailing backslash
    {
        ObjDir[l-1] = NULL;
    }

    if(ObjDir[0] == NULL)
    {
        strcpy(ObjDir,".");
    }
    if(MakeName[0] == NULL)
    {
        MessageBox("Invalid Name for MakeFile","Error");
        return;
    }

    if(ResName[0] == NULL)
    {
        strcpy(ResName,"No-name.RC");
    }

    CreateMakeFile(MakeName, ResName, List,ObjDir, LinkSwitches);
    while(i)
    {
        delete List[--i];
    }
    CmOk();
}


//----------------------------------
// Event EV_BN_CLICKED for ID_HELP
//----------------------------------
void MakeDlg::EvClickHelp(void)
{
    Help("Create Makefile");
}


//---------------------------------------------------------------
// Event EV_BN_CLICKED for ID_EXAMPLE
//---------------------------------------------------------------
void MakeDlg::EvClickExample(void)
{
    SetDlgItemText(ID_LINKSWITCHES,"/wdpl /Toe /aa");
}


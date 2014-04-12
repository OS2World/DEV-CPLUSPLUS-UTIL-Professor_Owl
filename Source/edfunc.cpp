#include    "def.h"
#include    <owl\dialog.h>
#include	<owl\edit.h>
#include	<owl\static.h>
#include	"resource.h"
#include	"edfunc.h"
#include	"parse.h"
#include    "warnbeep.h"


//----------------------------------
// MESSAGE MAP FOR EdFunc
//----------------------------------
DEFINE_RESPONSE_TABLE1(EdFunc,TDialog)
	EV_BN_CLICKED(IDOK,EvClickOk),
	EV_BN_CLICKED(IDCANCEL,EvClickCancel),
	EV_BN_CLICKED(ID_DEFAULT,EvClickDefault),
    EV_EN_CHANGE(ID_FNAME,EvChangeFname),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
EdFunc::EdFunc(TWindow *parent) : TDialog(parent, ID_EDITNAME)
{
	m_Fname = new TEdit(this,ID_FNAME);
	m_Info = new TStatic(this,ID_INFO);
	// INSERT>> Your code here
}

//----------------------------------
// Destructor
//----------------------------------
EdFunc::~EdFunc()
{
	delete m_Fname;
	delete m_Info;
	// INSERT>> Your code here
}

//----------------------------------
// Cancel Button
//----------------------------------
void EdFunc::EvClickCancel(void)
{
	// INSERT>> Your code here
	CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void EdFunc::EvClickOk(void)
{
	m_Fname->GetText(m_Func,sizeof(m_Func));
	if(m_Func[0] == NULL)
		return;

    char sec1[MAXPATHSIZE];
    char s[MAXPATHSIZE];
    char fname[MAXPATHSIZE];
	sprintf(sec1,"class %s",m_Class);
	if( ExactGrepFiles(sec1,NULL,m_Func,"","",m_Hfile,fname,s) )
	{
		PrettyString(s);
        char title[MAXPATHSIZE];
		char msg[1024];
        sprintf(title,"FUNCTION EXISTS IN %s -- PROCEED?",m_Hfile);
        sprintf(msg,"Usage: %s\n\n"
                    "Yes: Use Existing Funtion\n"
                    "No:  Select Another Name",s);
		WarnBeep();
		if( MessageBox(msg,title,MB_YESNO | MB_DEFBUTTON2) == IDNO)
			return;
	}
	CmOk();
}


//----------------------------------
// Init Dialog
//----------------------------------
BOOL EdFunc::EvInitDialog(HWND hWndFocus)
{
	BOOL b = TDialog::EvInitDialog(hWndFocus);
	m_Fname->SetText(m_Func);
	m_Info->SetText(Info);
	return b;
}



//----------------------------------
// Event EV_BN_CLICKED for ID_DEFAULT
//----------------------------------
void EdFunc::EvClickDefault(void)
{
	m_Fname->SetText(m_FuncDef);
	strcpy(m_Func,m_FuncDef);
}

void EdFunc::Input(char *fh, char *fc, char *myclass, char *func)
{
	strncpy(m_Func,func,sizeof(m_Func));
	strcpy(m_FuncDef,m_Func);
	//
	strncpy(m_Class,myclass,sizeof(m_Class));
	sprintf(Info,"%s (%s %s)",myclass,fh,fc);
	strncpy(m_Hfile,fh,sizeof(m_Hfile));
	strncpy(m_Cfile,fc,sizeof(m_Cfile));
}

void EdFunc::Output(char *s)
{
	strcpy(s,m_Func);
}


//----------------------------------
// Event EV_EN_CHANGE for ID_FNAME
//----------------------------------
void EdFunc::EvChangeFname(void)
{
	char s[128+1];
	m_Fname->GetText(s,sizeof(s));
	if( ValidateSymbol(s) )
	{
		m_Fname->SetText(s);
	}
}


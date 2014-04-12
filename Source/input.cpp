//===============================================
// d:\pro-owl\os2\input.cpp created wed oct 25 20:50:16 1995
//
// Using Professor Owl 1.04b
// input -- INPUT
// Prototypes are in input.H
//===============================================
//
//
//
#include    "def.h"
#include    "resource.h"
#include    "input.h"


//----------------------------------
// MESSAGE MAP FOR InputDialog
//----------------------------------
DEFINE_RESPONSE_TABLE1(InputDialog,TDialog)
    EV_BN_CLICKED(IDOK,EvClickOk),
    EV_BN_CLICKED(IDCANCEL,EvClickCancel),
END_RESPONSE_TABLE;


//----------------------------------
// Constructor
//----------------------------------
InputDialog::InputDialog(TWindow *parent) : TDialog(parent, ID_GENINPUT)
{
    m_InputDropDown = new TComboBox(this,ID_INPUT_DROPDOWN);
    m_InputMsg = new TStatic(this,ID_INPUT_MSG);
}



//----------------------------------
// Destructor
//----------------------------------
InputDialog::~InputDialog()
{
    delete m_InputDropDown;
    delete m_InputMsg;
}



//----------------------------------
// Init Dialog
//----------------------------------
BOOL InputDialog::EvInitDialog(HWND hWndFocus)
{
    BOOL RetValue = TDialog::EvInitDialog(hWndFocus);
    SetWindowText(m_Title);
    SetDlgItemText(ID_INPUT_MSG,m_Prompt);

    m_InputDropDown->ClearList();
    m_InputDropDown->Clear();
    for(int i=0; i<m_HistLen; i++)
    {
        m_InputDropDown->AddString(m_HistoryPtr[i]);
    }
    m_InputDropDown->SetWindowText(m_StrPtr);
    //
    return(RetValue);
}


//----------------------------------
// Cancel Button Pressed
//----------------------------------
void InputDialog::EvClickCancel(void)
{
    CmCancel();
}


//----------------------------------
// OK Button Pressed
//----------------------------------
void InputDialog::EvClickOk(void)
{
    m_InputDropDown->GetWindowText(m_StrPtr,m_MaxStr); // return user the string

    //
    // If m_StrPtr is in history buffer deleet it
    //
    for(short j=0; j<m_HistLen; j++)
    {
        if(strcmp(m_StrPtr, m_HistoryPtr[j]) == 0)
        {
            for(short int k=j; k<m_HistLen-1; k++)
            {
                strcpy( m_HistoryPtr[k] ,  m_HistoryPtr[k+1] );
            }
            *m_HistoryPtr[m_HistLen-1] = NULL;
        }
    }

    //
    // Shuffle History List Down One !
    //
    for(short int i=m_HistLen-1; i>=1; i--)
    {
        strcpy( m_HistoryPtr[i] ,  m_HistoryPtr[i-1] );
    }
    strncpy(m_HistoryPtr[0], m_StrPtr, m_MaxStr); // Add Selection to top of list
    CmOk();
}



//----------------------------------
//  'User Function' private: Init
//----------------------------------
void InputDialog::Init(char *title, char *prompt, int historylen, char *history[], char *str, int maxstr)
{
    strcpy(m_Title,title);
    strcpy(m_Prompt,prompt);
    m_HistLen = historylen;
    m_HistoryPtr = history;
    m_StrPtr = str;
    m_MaxStr = maxstr;
}


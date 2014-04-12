//===============================================
// This header file was created on wed oct 25 20:50:16 1995
// Using Professor Owl 1.04b
// This file has prototypes for function
// contained in input.CPP
// input.H and input.CPP are in d:\pro-owl\os2
//===============================================


#include    <owl\dialog.h>
#include    <owl\combobox.h>
#include    <owl\static.h>
class InputDialog : public TDialog
{
private:    // Variables
	char m_Title[80+1];
	char m_Prompt[128+1];
	short int m_HistLen;
	char **m_HistoryPtr;
    char *m_StrPtr;
	short int m_MaxStr;
	///
	TStatic *m_InputMsg;
    TComboBox *m_InputDropDown;
public:    // Methods
	InputDialog(TWindow *parent);
	void Init(char *title, char *prompt, int historylen, char *history[], char *str, int maxstr);
	virtual ~InputDialog();
        BOOL virtual EvInitDialog(HWND hWndFocus);
        void EvClickCancel(void);
        void EvClickOk(void);
    DECLARE_RESPONSE_TABLE(InputDialog);
protected:    // Events
}; // end of class InputDialog


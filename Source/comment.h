#include    <owl\dialog.h>
#include    <owl\edit.h>
//
// Created Fri Oct 13 21:37:30 1995
//
class DlgComment : public TDialog
{
private:    // Variables
    TEdit *m_Commentflag;
    TEdit *m_HfileHead;
    TEdit *m_CfileHead;
    TEdit *m_FuncDelimeter;
public:    // Methods
	 DlgComment(TWindow *parent);
	 virtual ~DlgComment();
		BOOL virtual EvInitDialog(HWND hWndFocus);
		void EvClickCancel(void);
		void EvClickOk(void);
	DECLARE_RESPONSE_TABLE(DlgComment);
protected:    // Events
    void EvClickHelp(void);
    void EvClickSamplehead(void);
    void EvClickEditc(void);
    void EvClickEdith(void);
	void EvClickExample(void);
}; // end of class DlgComment


void LoadCommentSettings(void);
void SaveCommentSettings(void);
void EditFile(char *fname);


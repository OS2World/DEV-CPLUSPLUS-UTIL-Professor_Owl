//===============================================
//
// This header file was created on fri mar 22 23:21:19 1996
// This file has prototypes for functions
// contained in BROWSE.CPP
//
// The class is Browse and the base class is TDialog
// Author Edward J. March Jr.
//===============================================


#include    <owl\dialog.h>
#include    <owl\combobox.h>
#include    <owl\listbox.h>
#include    <owl\edit.h>


class Browse : public TDialog
{
private:    // Variables
    void ParseFile(char *fname, char *sect);
    int  ParseFileCode(char *filename,long index, char *buffer); // return error code
    int  GetCode(char *buffer);
    //
    TListBox *m_Member;
    TComboBox *m_Class;
    TEdit    *m_Edit;

public:    // Methods
     Browse(TWindow *parent);
     virtual ~Browse();
        BOOL virtual EvInitDialog(HWND hWndFocus);
        void EvClickCancel(void);
        void EvClickOk(void);
    DECLARE_RESPONSE_TABLE(Browse);
protected:    // Events
    void EvClickParent(void);
    void EvSelChangeMember(void);
    void EvClickClipos2(void);
    void EvClickCopy(void);
    void EvEditChangeClass(void);
    void EvDblClickMember(void);
    void EvClickHelp(void);
}; // end of class Browse


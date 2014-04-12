//===============================================
//   Find Duplicate Files (OS/2)
//
// This header file was created on sun feb 11 15:24:40 1996
// This file has prototypes for functions
// contained in MAKEDLG.CPP
//
// The class is MakeDlg and the base class is TDialog
// Author Edward J. March Jr.
//===============================================


#include    <owl\dialog.h>
#include    <owl\edit.h>
#include    <owl\listbox.h>


class MakeDlg : public TDialog
{
private:    // Variables
    TListBox *ListBox;
    TEdit *m_Outputpath;
    TEdit *m_Name;
public:    // Methods
     MakeDlg(TWindow *parent, TListBox *lb);
     virtual ~MakeDlg();
        BOOL virtual EvInitDialog(HWND hWndFocus);
        void EvClickCancel(void);
        void EvClickOk(void);
    DECLARE_RESPONSE_TABLE(MakeDlg);
protected:    // Events
    void EvClickExample(void);
    void EvClickHelp(void);
}; // end of class MakeDlg



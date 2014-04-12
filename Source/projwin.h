//===============================================
//
// This header file was created on mon jan 29 22:30:02 1996
// This file has prototypes for functions
// contained in PROJWIN.CPP
//
// The class is ProjWin and the base class is TDialog
// Author Edward J. March Jr.
//===============================================


#include    <owl\dialog.h>
#include    <owl\listbox.h>
#include    <owl\menu.h>


#define MAXUNDELETE 128

class ProjWin : public TDialog
{
private:    // Variables
    TPopupMenu *RightMenu;
    void RunTool(int n);
    char dirty;             // change sin list box / project.owl file
    int UndeleteIndex;
    char *Undelete[MAXUNDELETE+1];
    TListBox *m_Projectfiles;
    PAPSZ FileOpen(void);       // File Open Common Dialog (multiple selection)
    TWindow *mother;
public:    // Methods
     ProjWin(TWindow *parent);
     virtual ~ProjWin();
        BOOL virtual EvInitDialog(HWND hWndFocus);
    DECLARE_RESPONSE_TABLE(ProjWin);
protected:    // Events
    void EvCmToolsetup(void);
    void EvCmRuntool8(void);
    void EvCmRuntool7(void);
    void EvCmRuntool6(void);
    void EvCmRuntool5(void);
    void EvCmRuntool4(void);
    void EvCmRuntool3(void);
    void EvCmRuntool2(void);
    void EvCmRuntool1(void);
    void EvRButtonUp(UINT modKeys, TPoint& point);
    void EvClickUndelete(void);
    void EvClickCreatemakefile(void);
    void EvClickSave(void);
    void Destroy(int retVal);
    void EvClickHelp(void);
    void EvClickDelete(void);
    void EvClickAdd(void);
    void EvClickEdit(void);
    void EvDblClickProjectfiles(void);
public:
    void ReloadFile(void);
}; // end of class ProjWin



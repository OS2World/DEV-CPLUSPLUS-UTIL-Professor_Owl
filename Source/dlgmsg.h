#include    <owl\dialog.h>
#include    <owl\listbox.h>
#include    <owl\checkbox.h>
#include    <owl\edit.h>
#include    <owl\editfile.h>
#include    <owl\static.h>
#include    <owl\menu.h>
#include    <owl\clipboar.h>
#include    <owl\button.h>


class DlgMsg: public TDialog
{
private:
    int m_HasBegun;             // 0= not ready, 1= reaady to scan, 2= did it once ! -- sync with TFrame window
    TStatic *m_Advertise;
    short m_ModeFun;
    char m_MleFile[MAXPATHSIZE+1];      // name of file in MLE editor
    int m_IgnoreEventChange;            // to click on function and find ID & event
    char StatMsg[256+1];                // ID_STATUS message text saver
    TPopupMenu RightMenu;
    TPopupMenu EdMenu;
    TComboBox *m_Class;
    TListBox *m_Objects;
    TListBox *m_Event;
    TEdit    *m_Member;
    TStatic  *m_EdFile;
    char parMyClass[80+1];
    char parBaseClass[80+1];
    char parHfile[80+1];
    char parCfile[80+1];
    char parID[80+1];
    char parChildId[80+1];
    char parCtrlType[80+1];
public:
    void Paste(void);
         DlgMsg(TWindow *parent, int ResIdNum);
         ~DlgMsg(void);
    void Begin(void);

protected:
    void EvClose(void);
    void EvCmBrowse(void);
    void EvCmNotify_prjwin_close(void);
    void EvCeOpenprojectwin(TCommandEnabler &CommandEnabler);
    void EvCmOpenprojectwin(void);
    void EvCmWindowrecallsizepos(void);
    void EvCmWindowdefault(void);
    void EvCmSavesizepos(void);
    BOOL DialogFunction(UINT message, WPARAM wParam, LPARAM lParam);
    void EvClickShowconst(void);
    void EvTimer(UINT timerId);
    void EvCmFindcommentflag(void);
    void EvCmFinddeleted(void);
    void EvCmHelpgeneral(void);
    void EvCmHelpindex(void);
    void EvCmCreateapplication(void);
    void EvCeSavemlefile(TCommandEnabler &CommandEnabler);
    void EvCmAbout(void);
    virtual BOOL EvInitDialog(HWND hWndFocus);
    virtual BOOL CanClose(void);
    void EvClickCancel(void);
    void EvClickAddClass(void);
    void EvClickTools(void);
    void EvClickAddEvent(void);
    void EvClickDelEvent(void);
    void EvClickVariable(void);
    void EvClickHelp(void);
    void EvClass(void);
    void EvObject(void);
    void EvEvent(void);
    void EvClickSaveMLEfile(void);
    DECLARE_RESPONSE_TABLE(DlgMsg);
private:
    void  GotoEventForFunction(char *line);
    int   FindSubStrLB(char *s, TListBox *pLB);
    void  AddEvent(int warning);
    void  DoTitle(void);
    int   PositionEditorSearch(char *key, int searchcomments=0);
    void  Ready(void); // Status = "Ready..."
    void  ToolDialog(int toolnum);

protected:
    void EvPaint(void);
    void EvMember(void);
    LRESULT EvTerm(WPARAM w, LPARAM);
    void EvClickSetup(void);
    void EvRButtonUp(UINT modKeys, TPoint& point);
    void EvCmFindobjects(void);
    void EvCmFindevent(void);
    void EvCmFindmember(void);
    void EvDblClickEvent(void);
    void EvCmGotoevent(void);
    void EvClickPath(void);
    void EvCmCut(void);
    void EvCmDel(void);
    void EvCmCopy(void);
    void EvCmPaste(void);
    void EvCmUndo(void);
public:
    int   LoadMLEfile(char *file);
    int   SaveMLEfile(void);
    void EvClickRescan(void);
};


void RememberClassFileName(char *s);
void RecallClassFile(char *s);


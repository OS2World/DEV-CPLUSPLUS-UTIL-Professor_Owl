#include    <owl\edit.h>
//
// Created Sat Sep 23 11:56:40 1995
//
class SetTool : public TDialog
{
private:    // Variables
    TEdit *m_Tool8dir;
    TEdit *m_Tool7dir;
    TEdit *m_Tool6dir;
    TEdit *m_Tool5dir;
    TEdit *m_Tool4dir;
    TEdit *m_Tool3dir;
    TEdit *m_Tool2dir;
    TEdit *m_Tool1dir;
    TEdit *m_Tool1path;
    TEdit *m_Tool2path;
    TEdit *m_Tool3path;
    TEdit *m_Tool4path;
    TEdit *m_Tool5path;
    TEdit *m_Tool6path;
    TEdit *m_Tool7path;
    TEdit *m_Tool8path;
    void Update(void);
public:    // Methods
     SetTool(TWindow *parent);
     virtual ~SetTool();
     BOOL virtual EvInitDialog(HWND hWndFocus);
     void EvClickCancel(void);
     void EvClickOk(void);

    void SaveToolSettings(void);

    DECLARE_RESPONSE_TABLE(SetTool);
protected:    // Events
    void EvClickHelp(void);
    void EvClickBrowse8(void);
    void EvClickBrowse7(void);
    void EvClickBrowse6(void);
    void EvClickBrowse5(void);
    void EvClickBrowse4(void);
    void EvClickBrowse3(void);
    void EvClickBrowse2(void);
    void EvClickBrowse1(void);
    void Browse(int id, int idname, char *toolnum);
    void EvClickExample(void);
}; // end of class SetTool


void LoadToolSettings(void);


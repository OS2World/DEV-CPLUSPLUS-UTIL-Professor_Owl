//
// Created Tue Feb 07 22:39:46 1995
//
class PrjPath : public TDialog
{
private:  // Class Variables
    void ScanDrives(void);
    TComboBox *m_Drives;
    char m_origdir[MAXPATHSIZE+1];
    TListBox *m_Prjdir;
protected:// Events
    void EvClickHelp(void);
    void EvClickDrivecz(void);
public:   // Methods
     PrjPath(TWindow *parent);
     ~PrjPath();
        void EvClickCancel(void);
        void EvClickOk(void);
        BOOL virtual EvInitDialog(HWND hWndFocus);
protected:
    void EvDblClickPrjdir(void);
private:
    void DoDirectories(void);
    DECLARE_RESPONSE_TABLE(PrjPath);
protected:
    void EvClickMakedir(void);
    void EvSelChangeDrives(void);
}; // end of class PrjPath


void RememberProjectPath(void);
void ForgetProjectPath(void);
void RecallProjectPath(char *s, int ChDir);


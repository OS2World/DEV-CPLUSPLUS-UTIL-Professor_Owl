//
// Created Sat Feb 25 15:18:30 1995
//
class UserFunc : public TDialog
{
private:    // Variables
	TEdit *m_FuncName;
    TEdit *m_Retype;
    TComboBox *m_Scope;
    char m_Result[256+1];
    char m_Class[256+1];
    char m_Hfile[256+1];
    char m_Cfile[256+1];
public:    // Methods
    void SetHCfiles(char *hfile, char *cfile, char *Class);
    void GetResult(char *s);
     UserFunc(TWindow *parent);
     virtual ~UserFunc();
        BOOL virtual EvInitDialog(HWND hWndFocus);
        void EvClickCancel(void);
        void EvClickOk(void);
    DECLARE_RESPONSE_TABLE(UserFunc);
protected:    // Events
    void EvClickHelp(void);
    void EvChangeFuncname(void);
    void EvChangeRetype(void);
    void EvClickDefault(void);
}; // end of class UserFunc


//
// Created Sun Jan 22 19:34:52 1995
//
class AppOpt : public TDialog
{
private:  // Class Variables
    TEdit *m_FileApp;
    TEdit *m_FileWindow;
    TEdit *m_FileAbout;
    TEdit *m_FileRes;
protected:// Events
    void EvClickHelp(void);
    void EvClickDefault(void);
    void EvChangeFileRes(void);
    void EvChangeFileAbout(void);
    void EvChangeFileWindow(void);
    void EvChangeFileApp(void);
    void EvClickSdi(void);
    void EvClickOptstatusbar(void);
    void EvClickOptmdi(void);
    void EvClickOptmenu(void);
public:   // Methods
    AppOpt(TWindow *parent);
    ~AppOpt();
		void EvClickCancel(void);
		void EvClickOk(void);
		BOOL virtual EvInitDialog(HWND hWndFocus);
	DECLARE_RESPONSE_TABLE(AppOpt);
protected:
	void EvClickOptmaindialog(void);
private:
	void EnableChildWithId(int id, int enable=1);
protected:
}; // end of class AppOpt


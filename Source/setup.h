//
// Created Sat Feb 04 22:51:46 1995
//
class Setup : public TDialog
{
private:  // Class Variables
    TEdit *m_Memprefix;
    TEdit *m_Includepath;
	TComboBox *m_Edsave;
	TComboBox *m_Tabs;
	TEdit *m_Edbak;
public:   // Methods
	 Setup(TWindow *parent);
	 ~Setup();
      void EvClickCancel(void);
      void EvClickOk(void);
     BOOL virtual EvInitDialog(HWND hWndFocus);
protected:// Events
    void EvClickRescantool(void);
    void EvClickHelp(void);
    void EvClickCommentsetup(void);
    void EvClickWarnbeep(void);
    void EvClickSetup(void);
	void EvActivate(UINT active, BOOL minimized, HWND hWndOthers);
	void EvClickDefault(void);
	void EvClickSave(void);
    void EvChangeEdbak(void);
    void EvKillFocusOwlpath(void);
private:
	void Update(void);
	DECLARE_RESPONSE_TABLE(Setup);
}; // end of class Setup


int NeedSetup(void);
void LoadSettings(void);
void SaveSettings(void);
void Settabs(void);
void SetupFixups(void);




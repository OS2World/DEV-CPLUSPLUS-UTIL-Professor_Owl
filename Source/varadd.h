//
// Created Sun Jan 15 00:28:17 1995
//
class VarAddDlg : public TDialog
{
private:
    TComboBox *m_Ppp;
	int m_Scan;
	int m_ParentNeedsToRescan;
	char m_VarName[256+1];		// 	m_MyThing
	char m_VarType[256+1];	   	//	TListBox
	char m_VarId[256+1];        //	ID_MYTHING
	//
	char m_Hfile[256+1];
	char m_Cfile[256+1];
	char m_Class[256+1];
	TListBox *m_Objects;	// parents parent objects listbox!
	TListBox *m_Ids;
	TEdit    *m_EdVarName;
public:
	VarAddDlg(TWindow *parent);
	 ~VarAddDlg();
	void Pass(char *m_Hfile, char *m_Cfile, char *m_Class, TListBox *m_Objects);
protected:
    void EvClickSkip(void);
		void EvClickCancel(void);
		void EvClickOk(void);
		BOOL virtual EvInitDialog(HWND hWndFocus);
	DECLARE_RESPONSE_TABLE(VarAddDlg);
	void EvChangeVarname(void);
	void EvEditChangeIdname(void);
	void EvPaint(void);
protected:
private:
	void UpdateInfo(void);
	void CheckDuplicate(char *rFound);
};

void LookUpControlToOwl(char *con,char *rS);



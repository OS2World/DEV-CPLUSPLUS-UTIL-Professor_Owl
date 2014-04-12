//
// Created Sun Jan 15 00:09:42 1995
//
class VarDlg : public TDialog
{
private:
	TListBox *m_Objects;    // MainDialog Parent Control
	char m_StrH[256+1]; 	// string in .H file
	char m_StrCon[256+1];   // constructor use
    char m_StrDes[256+1];   // destructor use
    char m_Symbol[256+1];   // symbol name i.e. MyThing
    char m_Type[256+1];     // type i.e. TListBox
    //
    int  m_Scan;
	char m_Class[80+1];
	char m_Hfile[256+1];
	char m_Cfile[256+1];
	TListBox *m_ListVars;
public:
	VarDlg(TWindow *parent);
	 ~VarDlg();
	void SetHCfiles(char *Class, char *hfile, char *cfile, TListBox *objs);

protected:
	void EvDblClickListvars(void);
		void EvClickCancel(void);
		void EvClickOk(void);
		BOOL virtual EvInitDialog(HWND hWndFocus);
	DECLARE_RESPONSE_TABLE(VarDlg);
	void EvClickAddvar(void);
protected:
	void EvClickDelvar(void);
	void EvClickRescan(void);
	void EvPaint(void);
protected:
	void EvSelChangeListvars(void);
private:
	void Rescan(void);
};



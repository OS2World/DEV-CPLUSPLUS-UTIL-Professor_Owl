//
// Created Fri Jan 13 00:30:28 1995
//
class EdFunc : public TDialog
{
private:
    char m_Func[MAXPATHSIZE+1];     // function name
    char m_FuncDef[MAXPATHSIZE+1];  // saved on entry - for default button
    char m_Class[MAXPATHSIZE+1];    // class name
    char m_Hfile[MAXPATHSIZE+1];    // ,H file name
    char m_Cfile[MAXPATHSIZE+1];    // .CPP file name
	//
	char Info[80+1];
	TEdit *m_Fname;
	TStatic *m_Info;
public:
	void Input(char *fh, char *fc, char *myclass, char *func);
	void Output(char *s);
	EdFunc(TWindow *parent);
	~EdFunc();

protected:
		void EvClickCancel(void);
		void EvClickOk(void);
		BOOL virtual EvInitDialog(HWND hWndFocus);
	DECLARE_RESPONSE_TABLE(EdFunc);
	void EvClickDefault(void);
	void EvChangeFname(void);
};



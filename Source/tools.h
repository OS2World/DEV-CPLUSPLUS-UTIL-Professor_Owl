//
// Created Sat Feb 04 22:40:33 1995
//
class Tools : public TDialog
{
private:  // Class Variables
    void DrawButtons(void);
	int m_Invoke;			// Start Tool From CONTROL_F1-Fn
    char m_Cfile[MAXPATHSIZE];
    char m_Hfile[MAXPATHSIZE];
	long m_LineNum;			// line number in editor
protected:// Events
    void EvTimer(UINT timerId);
    void EvUserStartApp(void);
	BOOL DialogFunction(UINT message, WPARAM wParam, LPARAM lParam);
	void EvClickSetup(void);
	LRESULT EvTerm(WPARAM, LPARAM);
public:   // Methods
    Tools(TWindow *parent, int tooln);
	void Invoke(int n);
	~Tools();
	void Set(char *hfile, char *cfile, long linenum);
	//
	void EvClickCancel(void);
	BOOL virtual EvInitDialog(HWND hWndFocus);
    DECLARE_RESPONSE_TABLE(Tools);
protected:
	void EvClickTool1(void);
	void EvClickTool2(void);
	void EvClickTool3(void);
	void EvClickTool4(void);
	void EvClickTool5(void);
	void EvClickTool6(void);
	void EvClickTool7(void);
	void EvClickTool8(void);
private:
    void ExpandDollar(char *in, char *s);
    void RunTool(int n);
}; // end of class Tools


#define MAXTOOLNAME 50

struct TOOLNAMEPATH
{
    char Name[MAXTOOLNAME+1];
    char Path[MAXPATHSIZE];
    char Dir[MAXPATHSIZE];
};


void MakePIDActiveWindow(long TheHapp);

void ExpandToolDollar(char *in, char *s, char *hfile, char *cfile, int LineNum);





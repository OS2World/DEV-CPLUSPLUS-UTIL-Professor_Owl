
class NewClass: public TDialog
{
private:
	int	 m_Scan;			// scan .RC files when Paint is done
    char OutStr[MAXPATHSIZE];     // return myclass : base ( me.h me.cpp )
    char BaseClass[MAXPATHSIZE];
    char MyClass[MAXPATHSIZE];
    char Hfile[MAXPATHSIZE];
    char Cfile[MAXPATHSIZE];
    char MyId[MAXPATHSIZE];
    char IdCfile[MAXPATHSIZE];
    char IdHfile[MAXPATHSIZE];

    TComboBox *m_Base;
    TEdit *m_Class;
    TEdit *m_Hfile;
	TEdit *m_Cfile;
	TComboBox *m_Ident;
public:
			NewClass(TWindow *parent);
			~NewClass(void);
	BOOL virtual EvInitDialog(HWND hWndFocus);
protected:
    void EvClickHelp(void);
	void 	EvClassChg(void);
	void 	EvHfileChg(void);
	void 	EvCfileChg(void);
	void	EvClickOk(void);
	void 	EvClickCancel(void);
	void 	EvChgBaseList(void);
public:
	void    Output(char *s);

	DECLARE_RESPONSE_TABLE(NewClass);
private:
	void Parser(void);
	void Rescan(void);

protected:
	void EvPaint(void);
	void EvClickRescan(void);
};



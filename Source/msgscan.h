class MsgScan
{
private:
    char s[MAXPATHSIZE+2];
    char s1[MAXPATHSIZE+2];
public:
	void Scanmask(char *mask, char *MyClass, char *BaseClass, TListBox *pLB);
	int  ScanFile(char *fname, char *MyClass, char *BaseClass, TListBox *pLB);
};


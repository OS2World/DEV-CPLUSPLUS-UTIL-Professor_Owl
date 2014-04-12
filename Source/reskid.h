
class ResKid
{
private:
    char s[MAXPATHSIZE+2];
    char s1[MAXPATHSIZE+2];
public:
    void Scanmask(char *pah, char *mask, char *idmatch, TListBox *pLB, BOOL showconst);
	void ScanCommands(char *path, char *fmask, TListBox *pLB);

private:
    int  Scanfile(char *path, char *fname, char *idmatch, TListBox *pLB, BOOL showconst);
	int  ScanCmd(char *path, char *fname, TListBox *pLB);
};



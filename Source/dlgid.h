
class DlgId
{
private:
    char s[MAXPATHSIZE+2];
    char s1[MAXPATHSIZE+2]; // plus /n and NULL
public:
    void Scanmask(char *path, char *mask, TListBox *pLB);
private:
    int  Scanfile(char *path, char *fname, TListBox *pLB);
};


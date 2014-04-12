
class ID
{
private:
    char s[MAXPATHSIZE+2];
public:
    void ScanRCmask(char *mask);
    int  ScanRCfile(char *fname);
    int  ScanHfile(char *fname);
private:
};


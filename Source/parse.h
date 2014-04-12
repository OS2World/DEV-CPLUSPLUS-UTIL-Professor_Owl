
void ParseRetFunArg(char *p, char *sRet, char *sFun, char *sArg);
void GetThisSymbol(char *p, char *s);
int  GrepFiles(int squish, char *sec1, char *sec2, char *key, char *key2, char *key3, char *fmask, char *fname, char *rs);
int  ExactGrepFiles(char *sec1, char *sec2, char *key, char *key2, char *key3, char *fmask, char *fname, char *rs);
void onespace(char *s, char *s1);
void nospace(char *s, char *s1);
int  ValidateSymbol(char *s);
void ConvArgParam(char *arg, char *param);
void FindFuncMemberForEvent(char *fname, char *MyClass, char *evmac, char *line);
void removeNL(char *s);
void TrimString(char *s);
int  StringOrder(char *s, char *strv[]);
int  PrettyString(char *s);
void ForceExt(char *fname, char *ext);
void ValidateFileNamePath(char *s);
void PathSplit(char *s1, char *s2, char *pf);
void ChPath(char *drivepath);


struct Proto
{
    char *desc;     // description comment
    char *virt;     // virtual or NULL
    char *retype;   // ret type [  NULL void long int ]
    char *func_args;// NULL ptr ends the list !
};


class Cproto
{
private:
    char s[MAXPATHSIZE+2];  // plus /n and NULL
    char s0[MAXPATHSIZE+2];
    char s1[MAXPATHSIZE+2];
    char key[MAXPATHSIZE+2];
    //
public:
	int  AddClass(char *Fh, char *Fc, char *Class, char *base, char *Hincludes[], char *Cincludes[], Proto members[]);
    //
    int  AddHdrVariable(char *Fh, char *Class, char *ppp, char *dtype, char *var);
    int  RemoveHdrVariable(char *Fh, char *Class, char *dtype, char *var);
    //
	int  AddFunction(char *fh, char *fc, char *Class, char *ppp, char *virt, char *retype, char *func,
					char *args="void", char *comment=NULL, char *code = NULL);
    int  RemoveFunction(char *Fh, char *Fc, char *Class, char *func, char *args="void");
    int  WriteCode(char *Fc, char *Class, char *func, char *code, int append=1);
	int  AddEvent(char *Fc, char *Class, char *EvMacro, char *Id, char *Func);
	int  AppendIncludeFile(char *fname,char *inc);
	int  DeleteLineFromFile(char *fname,char *mathingline);

	int  ScanVariables(char *Fh, char *Fc, char *Class, TListBox *Lb);
	void UnDoDefaultParms(char *pfa, char *qfa);

private:
	void CopyFile(FILE *fs, char *src, FILE *fd, char *dest);
	void CopyFileLines(FILE *fi,FILE *fo, long line);
	int  CommentOut(char *Fc, char *Class, char *func, char *args);
};



void SlapOnHeader(char *fname, char *src, char *Class, char *base);
void ExpandDollar(char *in, char *s, char *fname="",char *Class="", char *base="");

void TempPathName(char *real, char *tempname);


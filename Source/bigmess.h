struct BigMess2
{
	char *Event;
    char *Func;
};

struct BigTypeMess
{
	char *Class;
	BigMess2 *BigMessPtr;
};


void AddEventTypeMess(char *Class, TListBox *pLB);
void LookUpEV_WM(char *ev, char *rRet,char *rFunc=NULL, char *rArg=NULL);
void ReverseLookUpEV_WM(char *Func, char *Arg, char *rEv);
void GetHeaderForClass(char *hdr, char *Class);
int  IsVirtualDestructorNeeded(char *baseclass);
void GetConstructorForClass(char *con, char *MyClass, char *BaseClass);


struct VFuncs
{
	char *retype;
	char *funcparm;
};

struct VFunctions
{
	char *Class;
	VFuncs   *V;
};

struct ClassTree
{
    char *Base;
    char *Parent;
};

void FindParentClass(char *Class, char *Parent1, char *Parent2);


void AddVirtualFunctionMess(char *Class, TListBox *pLB);
void AddVfs(char *Class, TListBox *pLB);

int  IsClassFromTWindow(char *BaseClass);
char IsClassFromTWindow(char *flag, char *BaseClass);



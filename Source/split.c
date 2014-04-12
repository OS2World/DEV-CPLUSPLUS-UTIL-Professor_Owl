//
// Created: 02/05/96 09:35 am
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <process.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <io.h>
#include <dir.h>
//
enum boolean { FALSE=0, TRUE=1 };
//
//
void PathSplit(char *s1, char *s2, char *pf);
//
//
int main(int argc, char *argv[], char *envp[])
{
	char s1[256+1];
	char s2[256+1];
	PathSplit(s1,s2,"d:\\bin\\test\\junk.c");
	printf("%s %s\n",s1,s2);
	PathSplit(s1,s2,"C:\\TOOLS\\split\\junk.c");
	printf("%s %s\n",s1,s2);
	PathSplit(s1,s2,"D:\\junk.c");
	printf("%s %s\n",s1,s2);
	PathSplit(s1,s2,"..\\junk.c");
	printf("%s %s\n",s1,s2);
	PathSplit(s1,s2,".\\junk.c");
	printf("%s %s\n",s1,s2);
	PathSplit(s1,s2,"junk.c");
	printf("%s %s\n",s1,s2);

	return(0);
}


//
// Split The PathFileName into a Path and FileName
// If Path Is The Same As The Current Working Dir
// Return a ".\" for the path.
// Examples:
// --> "d:\bin\test\junk.c"     --> "d:\bin\test" "junk.c"
// --> "C:\TOOLS\split\junk.c"  --> ".\"  "junk.c"
// --> "D:\junk.c"              --> "D:\" "junk.c"
// --> ".\junk.c"			    --> ".\"  "junk.c"
// --> "junk.c"				    --> ".\"  "junk.c"
// --> "..\junk.c"			    --> "..\" "junk.c"
void PathSplit(char *s1, char *s2, char *pf)
{
	printf("---> %s ---> ",pf);
	char cwd[256+1];
	getcwd(cwd,sizeof(cwd));
	char *p = pf;
	char *p1 = p;
	while( *p )
	{
		if(*p == '\\')
			p1 = p;
		p++;
	}
	int l = p1-pf;
	strncpy(s1,pf, l);
	s1[l] = NULL;
	if(l == 0)
	{
		strcpy(s1,".\\");
		strcpy(s2,pf);
	}
	else
	{
		if(l == 2 && s1[1] == ':' || s1[0] == '.')
		{
			strcat(s1,"\\");
		}
		strcpy(s2,p1+1);
		if(stricmp(s1,cwd) == 0)
		{
			strcpy(s1,".\\");
		}
	}
}



#include    "def.h"
#include    <stdio.h>
#include	<string.h>
#include	<time.h>
#include	<dir.h>
#include	<ctype.h>
#include	<share.h>
//
#include	<owl\listbox.h>
#include	<owl\combobox.h>
//
#include    "msgscan.h"
#include    "parse.h"
#include    "bigmess.h"

//
// Scan For All Message Handler Function in the RESPONSE Table
// The table must be of class type MyClass Devrived From BaseClass
//
//      Add All Text Liens To List Box - after pruning them
//		if string is already in list box - dont add it twice !
//
void MsgScan::Scanmask(char *mask, char *MyClass, char *BaseClass, TListBox *pLB)
{
    struct ffblk dta;
	int done = findfirst(mask, &dta, 0);
	while( !done )
    {
        ScanFile(dta.ff_name, MyClass, BaseClass, pLB);
		done = findnext(&dta);
    }
}

//
// Add functions who have an entry in response table to Function listbox
//
int MsgScan::ScanFile(char *fname, char *MyClass, char *BaseClass, TListBox *pLB)
{
    FILE *fi = _fsopen(fname,"rt",SH_DENYNONE);
    if(fi)
    {
		int endsec = 0;
		while(!endsec && !feof(fi) )
		{
            char s[MAXPATHSIZE+2];
            char s1[MAXPATHSIZE+2];
            fgets(s,sizeof(s1)-2,fi);
            onespace(s,s1);
            char *paren = strchr(s1,'(');
            char *pcomma = strchr(s1,',');

            if(paren && strstr(s1,"DEFINE_RESPONSE_TABLE") &&
				strstr(paren,MyClass) && strstr(pcomma,BaseClass) && strchr(paren,')') )
			{
				while(!endsec && !feof(fi) )
				{
					fgets(s,sizeof(s1)-2,fi);
					onespace(s,s1);
					if(strstr(s1,"END_RESPONSE_TABLE") && strchr(s1,';') )
						endsec = 1;
					else if(strstr(s1,"EV_") && strchr(s1,',') )
					{
                        char line[MAXPATHSIZE+2];
						FindFuncMemberForEvent(fname,MyClass,s1,line);
						if(line[0]==NULL)
						{
							sprintf(line,"@ Missing function for %s",s1);
							removeNL(line);
						}
					if(pLB->FindString(line,-1) < 0)
							pLB->AddString(line);	// add unique string
					}
				}
            }
        }
        fclose(fi);
    }
	return 0;
}





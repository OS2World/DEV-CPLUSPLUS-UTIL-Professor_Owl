#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <dir.h>
#include    <share.h>
#include    <owl\listbox.h>
#include    "bigmess.h"
#include    "parse.h"
#include    "def.h"

#define FALSE   0
#define TRUE    1

int IsPunct(char *p);

int IsPunct(char *p)
{
    if(p == 0)
        return(TRUE);
    int c = *p;

    if(ispunct(c) )
        return(TRUE);
    if(isspace(c))
        return(TRUE);
    if(c == 0)
        return(TRUE);
    return(FALSE);
}

//
// Return function name in a
// string like "char far * Test(int j)" OR "char far * Test(int j=2)"
// by scanning to '(' and walking left to space!
//  return -- sRet = "char far *"
//            sFun = "Test"
//            sArg = "int j=2"
void ParseRetFunArg(char *p, char *sRet, char *sFun, char *sArg)
{
    *sRet = NULL;
    *sFun = NULL;
    *sArg = NULL;
    char *p1 = strchr(p,' '); // is the a space??
    if(!p1)
        return; // No space in string !!!
    ///
    p1 = strchr(p,'(');  // is there a open paren ??
    if(!p1)
        return; // can't find open paren
    strcpy(sArg,p1+1);
    char *cp = strchr(sArg,')');
    if(cp)
    {
        *cp = NULL;
    }
    char tArg[MAXPATHSIZE+1]; // temp arg
    onespace(sArg,tArg);
    strcpy(sArg,tArg);
    TrimString(sArg);
    //
    // walk left...
    while(*p1 != ' ')
    {
        p1--;
    }
    p1++;
    strcpy(sFun,p1);    // return "Test(int j)"
    char *op = strchr(sFun,'(');
    if(op)
    {
        *op = NULL;
    }
    char *d = sRet; // TInStream* TinStream() would fail when ret_name is same as func name!!!

    // Skip leading white space before
    while(*p && *p == ' ')  // needed for %-18s format in addstring() bigmess.cpp & vfun.cpp
    {
        p++;
    }

    while(p < (p1-1) )
    {
        *d++ = *p++;
    }
    *d = NULL;
    TrimString(sRet);
}

//
// Given(",ThisSymbol ,more , 9100 ",s);
// return s = "ThisSymbol"
//
void GetThisSymbol(char *p, char *s)
{
    *s = NULL;  // if no symbol found...
    //
    if(p == NULL)   // safe function
        return;
    //
    while(*p && (*p == ' ' || *p == ',') )
        p++;

    while(*p && ( isalpha(*p) || isdigit(*p) || *p == '_') )
        *s++ = *p++;
    //
    *s = NULL;
}


//-- !!!Notes:  "\" this is not in quotes " (but it really is !!)
//
// Convert tabs to spaces, and remove extra spaces
// so only TAB TAB or TAB SPACE becomes just one space.
//
//  NEVER DESTROY original Input String 's'
// Input: 's'  Output: 'ss'
//
void onespace(char *s, char *ss)
{
    int inquote = FALSE;
    //
    // ignore C & C++ style comments  '//' and '/*'
    //
    while(*s)
    {
        int c = *s++;
        if(c == '"')
            inquote = !inquote;
        if(c == '\t')
            c = ' ';
        if(!inquote && c == '/' && (*s == '/' || *s == '*') )
        {
            *ss = NULL;
            break;
        }
        *ss++ = c;
        // eat extra white spaces...
        while( c == ' ' && ( *s == ' ' || *s == '\t') )
            s++;
    }
    *ss = NULL;
}

//
// Convert tabs and spaces, to nothing
//
//  NEVER DESTROY original Input String 's'
void nospace(char *s, char *ss)
{
    int inquote = FALSE;
    //
    // ignore C & C++ style comments  '//' and '/*'
    //
    while(*s)
    {
        int c = *s++;
        if(c == '"')
            inquote = !inquote;
        if(c == '\t')
            c = ' ';
        if(!inquote && c == '/' && (*s == '/' || *s == '*') )
        {
            *ss = NULL;
            break;
        }
        if(c != ' ')  // Ignore all spaces !!!
            *ss++ = c;
        // eat extra white spaces...
        while( c == ' ' && ( *s == ' ' || *s == '\t') )
            s++;
    }
    *ss = NULL;
}



//
// Find key and key2 between sec1 & sec2 in fmask of files
// return fname of first file and return original input rs string
//
// sec1 may be NULL for any-section
// sec2 may also be null
// NOTE: if sec2 end in newline then this symbol must match from s[0]
//
int GrepFiles(int squish, char *sec1, char *sec2, char *pkey, char *pkey2, char *pkey3, char *fmask, char *fname, char *rs)
{
    char key[MAXPATHSIZE+1];
    char key2[MAXPATHSIZE+1];
    char key3[MAXPATHSIZE+1];

    if(squish)
    {   // zero space
        nospace(pkey,key);
        nospace(pkey2,key2);
        nospace(pkey3,key3);
    }
    else
    {   // 1 space
        onespace(pkey,key);
        onespace(pkey2,key2);
        onespace(pkey3,key3);
    }

    *fname = *rs = NULL;
    struct ffblk dta;
    int done = findfirst(fmask, &dta, 0);
    int sec2has_nl = FALSE;
    if(sec2 && strchr(sec2,'\n') )
        sec2has_nl = TRUE;

    while( !done )
    {
        FILE *fi = _fsopen(dta.ff_name,"rt",SH_DENYNONE);
        if(fi)
        {
            int endsec = 0;
            while(!endsec && !feof(fi) )
            {
                char s[MAXPATHSIZE+2];
                char s1[MAXPATHSIZE+2];
                fgets(s,sizeof(s)-2,fi);
                if(squish)
                    nospace(s,s1);
                else
                    onespace(s,s1);

                if(sec1 == NULL || strstr(s1,sec1) )    // BEGINNING FOUND !
                {
                    while(!endsec && !feof(fi) )
                    {
                        fgets(s,sizeof(s)-2,fi);
                        if(squish)
                            nospace(s,s1);
                        else
                            onespace(s,s1);

                        if(sec2has_nl && strncmp(s1,sec2,strlen(sec2)-1) == 0)
                            endsec = 1;
                        else if(sec2 && strstr(s1,sec2) )
                            endsec = 1;
                        else if(strstr(s1,key) && strstr(s1,key2) && strstr(s1,key3) )
                        {
                            strcpy(rs,s);
                            strcpy(fname,dta.ff_name);
                            fclose(fi);
                            return 1;
                        }
                    }
                }
            }
            fclose(fi);
        }
        done = findnext(&dta);
    }
    return 0;
}

//
// unlike GrepFiles the key CAN NOT BE A PARTIAL STRING !
// the key string is matched when a space or punctuation is found.
// So a key of "MyVar" will not match "MyVar1" but WILL match "MyVar;"
//
// Find exact key and key2 between sec1 & sec2 in fmask of files
// return fname of first file and return original input rs string
//
// sec1 may be NULL for any-section
// sec2 may also be null
// NOTE: if sec2 end in newline then this symbol must match from s[0]
//
int ExactGrepFiles(char *sec1, char *sec2, char *key, char *key2, char *key3, char *fmask, char *fname, char *rs)
{
    *fname = *rs = NULL;
    struct ffblk dta;
    int done = findfirst(fmask, &dta, 0);
    int sec2has_nl = FALSE;
    if(sec2 && strchr(sec2,'\n') )
        sec2has_nl = TRUE;

    int kl = strlen(key);
    int kl2 = strlen(key2);
    int kl3 = strlen(key3);

    while( !done )
    {
        FILE *fi = _fsopen(dta.ff_name,"rt",SH_DENYNONE);
        if(fi)
        {
            int endsec = 0;
            while(!endsec && !feof(fi) )
            {
                char s[256+2];
                char s1[256+2];
                fgets(s,sizeof(s)-2,fi);
                onespace(s,s1);
                if(sec1 == NULL || strstr(s1,sec1) )    // BEGINNING FOUND !
                {
                    while(!endsec && !feof(fi) )
                    {
                        fgets(s,sizeof(s1)-2,fi);
                        onespace(s,s1);
                        if(sec2has_nl && strncmp(s1,sec2,strlen(sec2)-1) == 0)
                            endsec = 1;
                        else if(sec2 && strstr(s1,sec2) )
                            endsec = 1;
                        else
                        {
                            char *pk = strstr(s1,key);
                            char *pk2 = strstr(s1,key2);
                            char *pk3 = strstr(s1,key3);
                            if( ((pk && kl>0)||kl==0)
                                && ((pk2 && kl2>0)||kl2==0)
                                && ((pk3 && kl3>0)||kl3==0) )
                            {
                                pk += kl;
                                pk2 += kl2;
                                pk3 += kl3;
                                if(    (kl==0 || IsPunct(pk) )
                                    && (kl2==0 || IsPunct(pk2) )
                                    && (kl3==0 || IsPunct(pk3) ) )
                                {
                                    strcpy(rs,s);
                                    strcpy(fname,dta.ff_name);
                                    fclose(fi);
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
            fclose(fi);
        }
        done = findnext(&dta);
    }
    return 0;
}





//
// Remove Invalid char's from symbol name
// return TRUE if changes made - else false as is
//
int ValidateSymbol(char *s)
{
    if(s == NULL || *s == NULL)
        return FALSE;
    //
    int changes = FALSE;
    int l = strlen(s);
    char *s1 = new char[l];
    //
    char *p = s;
    char *p1 = s1;
    //
    if(s[0] == '_' || isalpha(s[0]) )
        *p1++ = *p;
    else
        changes = TRUE;

    p++;
    while(*p)
    {
        if( isdigit(*p) || isalpha(*p) || *p == '_' )
            *p1++ = *p;
        else
            changes = TRUE;
        p++;
    }
    *p1 = NULL;

    if(changes)
        strcpy(s,s1);

    delete s1;
    return(changes);
}


//
// Convert args as "int x, long y, char *z"
// to parameters to calls base class function
// as "x,y,z"
// Note: input "void" output = "" !
//
void ConvArgParam(char *arg, char *param)
{
    *param = NULL;
    if(strcmp(arg,"void") == 0)
        return; // return null string - not NULL pointer!
    //
    char *p = arg;
    while(*p)
    {
        char *p1 = strchr(p,',');
        if(p1)
            p = p1;
        else    // NO COMMA - GET NULL
            p = p + strlen(p);
        p--;
        while( *p && isspace(*p) )    // incase "char far myname ," SPACE COMMA!
            p--;

        int l = 0;
        while( !isspace(*p) && !ispunct(*p) )
        {
            l++;
            p--;
        }
        p++;
        if(*param)
        {
            strcat(param,", ");
        }
        strncat(param,p,l);
        p += l;
        if(*p)
            p++;
    }
}

//
// Given a filename and class name and event macro return the
// Line of the code that has the function defined
//
// example
//      "SOURCE.CPP", "MyClass", "EV_WM_LBUTTONDOWN"
// would return
//      line = "void MyClass::EvLButtonDown(UINT modKeys, TPoint& point)"
//
void FindFuncMemberForEvent(char *fname, char *MyClass, char *evmac, char *line)
{
    *line = NULL;
    char func[256+1];
    func[0] = NULL;
    char fname2[256+1];
    //
    if(strncmp(evmac,"EV_WM_",6)!=0)  // MACRO WITH parameters
    {
        // parse second parameter
        char *p2=strchr(evmac,','); // Find Comma after -- ID_NAME,Function
        if(p2)                      // two param macro - get par #2
            GetThisSymbol(p2,func);     // Get Function Name
    }
    else // Macro With NO Parameters - use look up tables.
    {
        char mac[256+1];
        // "EV_MW_APP void Func(int ... )" -> Strip Out Macro "EV_WM_XXX"
        GetThisSymbol(evmac,mac);
        LookUpEV_WM(mac,NULL,func,NULL);
    }
    if(func[0])
    {
        strcat(func,"(");
        GrepFiles(TRUE,NULL,NULL,MyClass,"::",func,fname,fname2,line);
        // Remove line feed at end
        removeNL(line);
    }
}

//
// Remove last char(s) if CR or LF
//
void removeNL(char *s)
{
    int l = strlen(s);
    if(l--)
    {
        char *p = &s[l];
        while(*p == '\r' || *p == '\n')
            *p-- = NULL;
    }
}

//
// Trim Trailing Space CR LF TAB off tail of string
//
void TrimString(char *s)
{
    int l = strlen(s);
    if(l--)
    {
        char *p = &s[l];
        while(*p == '\r' || *p == '\n' || *p == ' ' || *p == '\t')
            *p-- = NULL;
    }
}


//
// return true is s contains these strings in this order.
//  strv[] = pointer to argv like list of tokens
//
int StringOrder(char *s, char *strv[])
{
    char *p = s;

    for(int i = 0; strv[i]; i++)
    {
        p = strstr(p,strv[i]);  // find string
        if(!p)
            return FALSE;       // token string not found...
        p += strlen(strv[i]);   // skip past it ...
    }
    return(TRUE);
}

//
// Make it nice and prety for
// static control and list boxes
// tab become space - \n becomes space!
//
int PrettyString(char *s)
{
    int len = 0;
    while(*s)
    {
        if(isspace(*s))
            *s = ' ';
        s++;
        len++;
    }
    return(len);
}


//
// force ext on fname .ext has period
//
// input fname ='.\test.h" ext=".cpp"
// Note: finds last period in text !
// fname = test.h"
//
void ForceExt(char *fname, char *ext)
{
    char *p = fname;
    char *p1;
    while( *p && (p1=strchr(p,'.') ) != NULL)
    {
        p = ++p1;
    }
    p--;
    if(*p == '.')
        strcpy(p,ext);
    else
        strcat(fname,ext);
}


void ValidateFileNamePath(char *s)  //MSDOS 5.0 Chapter 4 page 69
{
    // A-Z 0-9 _^$~!#%&-{}()@`'
    //!!!Note: needs to be added someday
}

//
// Split The PathFileName into a Path and FileName
// If Path Is The Same As The Current Working Dir
// Return a ".\" for the path.
// Examples:
// --> "d:\bin\test\junk.c"     --> "d:\bin\test\" "junk.c"
// --> "C:\TOOLS\split\junk.c"  --> ".\"  "junk.c"
// --> "D:\junk.c"              --> "D:\" "junk.c"
// --> ".\junk.c"               --> ".\"  "junk.c"
// --> "junk.c"                 --> ".\"  "junk.c"
// --> "..\junk.c"              --> "..\" "junk.c"
// Path Always Ends In BackSlash !
//
void PathSplit(char *s1, char *s2, char *pf)
{
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
        strcat(s1,"\\");
        strcpy(s2,p1+1);
        //
        if(stricmp(s1,cwd) == 0)
        {
            strcpy(s1,".\\");
        }
    }
}


//
// ChPath("C:\newdir");
// Change to that drive and path
//
void ChPath(char *drivepath)
{
    char s[256+1];
    strcpy(s,drivepath);
    int c = s[0];
    c = toupper(c);
    if(s[1] == ':')
        setdisk(c-'A');
    //
    // Remove Trailing Backslash (unless its "X:\")
    //
    short int l = strlen(s);
    if(l > 3 && s[l-1] == '\\') // greater than 3 chars!
    {
        s[l-1] = NULL;
    }
    //
    chdir(s);
}




#include    "def.h"
#include    <stdio.h>
#include    <string.h>
#include    <time.h>
#include    <dir.h>
#include    <ctype.h>
#include    <share.h>
//
#include    <owl\listbox.h>
#include    <owl\combobox.h>
//
#include    "cl.h"
#include    "reskid.h"
#include    "parse.h"
#include    "projfile.h"
//

//
// Scan .RC files for DLGTEMPLATE <idmatch> and add the child
// controls to the listbox...
//
void ResKid::Scanmask(char *path, char *fmask, char *idmatch, TListBox *pLB, BOOL showconst)
{
    char thisdir[MAXPATHSIZE];
    getcwd(thisdir,sizeof(thisdir));
    ChPath(path);

    struct ffblk dta;
    int done = findfirst(fmask, &dta, FA_DIREC + FA_SYSTEM + FA_HIDDEN);
    while( !done )
    {
        Scanfile(path, dta.ff_name, idmatch, pLB, showconst);
        done = findnext(&dta);
    }
    ChPath(thisdir);
}

//
// ADD String To ListBox as "ID_MYTHING PUSHBUTTON"
//                      or  "ID_MYLIST WC_LISTBOX"
// Showconst flag to ALLOW 101 or constants without an ID_NAME #define
//
int ResKid::Scanfile(char *path, char *fname, char *idmatch, TListBox *pLB, BOOL showconst)
{
    FILE *fi = _fsopen(fname,"rt",SH_DENYNONE);
    if(!fi)
        return 0;
    //
    // SEARCH FOR
    //      DLGTEMPLATE "???", ID_THISIDKEY,
    //
    while(!feof(fi))
    {
        fgets(s,sizeof(s)-2,fi);
        if(strstr(s,"DLGTEMPLATE") && strstr(s,idmatch) )
        {
            fgets(s,sizeof(s)-2,fi);
            if( strstr(s,"BEGIN") || strstr(s,"{") )    // 1.07 braces
            {
                while(!feof(fi)
                    && strncmp(s1,"}",1) !=0
                    && strncmp(s1," }",2) !=0       // 1.07!
                    && strncmp(s1,"END",3) != 0
                    && strncmp(s1," END",4) != 0)
                {
                    fgets(s,sizeof(s)-2,fi);
                    onespace(s,s1);

                    int isbutton = FALSE;
                    char *ctrl = strstr(s1,"CONTROL ");
                    if(!ctrl)
                    {
                        ctrl = strstr(s1,"PUSHBUTTON ");
                        isbutton = TRUE;
                    }

                    if(ctrl)
                    {
                        // dont be confused by commas in title string!
                        char *p = strchr(s1,'"');    // find open quote
                        if(p)
                            p = strchr(p+1,'"');  // find closing quote
                        else
                            p = s1;
                        char *p2 = strstr(p,"\\\"");    // backslash quote!!!!!
                        if(p2)
                            p = p2;
                        if(p)
                        {
                            p = strchr(p,','); // skip 1st comma
                            if(p)
                            {
                                char ChildId[80+1];
                                GetThisSymbol(p,ChildId);
                                int j = strlen(ChildId);
                                if(j < 16)
                                {
                                    strcat(ChildId,"                 ");
                                    ChildId[16] = NULL;
                                }
                                if(isbutton)
                                {
                                    strcat(ChildId," PUSHBUTTON");
                                }
                                else
                                {
                                    p = strstr(p,"WC_");
                                    if(p)
                                    {
                                        GetThisSymbol(p,s);
                                        strcat(ChildId," ");
                                        strcat(ChildId,s);
                                    }
                                }
                                if(showconst || !isdigit(ChildId[0]) )
                                {
                                    pLB->AddString(ChildId);
                                }
                            }
                        }
                    }
                }
                fclose(fi);
                return 1;
            }
        }
    }
    fclose(fi);
    return 1;
}

// -- SCAN FOR #define CM_commandname
//
// Scan .H .HPP or .C or .CPP for #define CM_
// add controls to the listbox...
//
void ResKid::ScanCommands(char *path, char *fmask, TListBox *pLB)
{
    char thisdir[MAXPATHSIZE];
    getcwd(thisdir,sizeof(thisdir));
    ChPath(path);

    struct ffblk dta;
    int done = findfirst(fmask, &dta, FA_DIREC + FA_SYSTEM + FA_HIDDEN);
    while( !done )
    {
        ScanCmd(path, dta.ff_name, pLB);
        done = findnext(&dta);
    }

    ChPath(thisdir);
}

//
// ADD String To ListBox as "CM_MYTHING COMMAND"
//                      or  "CM_FILEOPEN COMMAND"
// always a EV_COMMAND macro message type !
//
int ResKid::ScanCmd(char *path, char *fname, TListBox *pLB)
{
    FILE *fi = _fsopen(fname,"rt",SH_DENYNONE);
    if(!fi)
        return 0;
    //
    // SEARCH FOR
    //      #define CM_[a-z]
    //
    int firstadd = TRUE;
    while(!feof(fi))
    {
        //
        fgets(s,sizeof(s)-2,fi);
        if(strstr(s,"#define") && strstr(s,"CM_") )
        {
            char ss[MAXPATHSIZE];
            char ss2[MAXPATHSIZE];
            ss2[0] = NULL;
            char *p1 = strchr(s,'#');
            if(!p1)
                p1 = s;
            int n = sscanf(p1,"#define %s %s",ss,ss2);
            if( n == 2 && strncmp(ss,"CM_",3) == 0)  // look like a command symbol
            {
                strcat(ss,"  EV_COMMAND");
                char *p = strchr(ss,'#');
                if(!p)
                    p = ss;
                PrettyString(p);
                if(pLB->FindString(p,-1) < 0)
                {
                    if(firstadd)    // show path and file
                    {
                        firstadd = FALSE;
                        PrettyPathFile(path);
                        PrettyPathFile(fname);
                        sprintf(s," <%s%s>",path,fname);  // path\\etc\\file.h
                        pLB->AddString(s);
                    }
                    pLB->AddString(p);
                }
            }
        }
    }
    fclose(fi);
    return 1;
}






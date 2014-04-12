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
#include    "ob.h"
#include    "reskid.h"
#include    "parse.h"
#include    "projfile.h"
//


//
// Scan C & CPP files for classes derived for OWL classes
// Save in ListBox as "MyClass : TOwlClass ( hdr.h cfile.cpp ) id=ID_TEMPLATENAME"
// For easy parsing when the list box string is selected.
//
void Ob::Scanmask(char *path, char *fmask, TListBox *pLB)
{
    char thisdir[MAXPATHSIZE];
    getcwd(thisdir,sizeof(thisdir));
    ChPath(path);

    struct ffblk dta;
    int done = findfirst(fmask, &dta, 0);
    while( !done )
    {
        PrettyPathFile(path);
        PrettyPathFile(dta.ff_name);
        Scanfile(path, dta.ff_name, pLB);
        done = findnext(&dta);
    }
    ChPath(thisdir);
}

int Ob::Scanfile(char *path, char *fname, TListBox *pLB)
{
    FILE *fi = _fsopen(fname,"rt",SH_DENYNONE);
    if(!fi)
        return 0;
    //
    while(!feof(fi))
    {
        char MyClass[128+1];
        char BaseClass[128+1];
        MyClass[0] = NULL;
        BaseClass[0] = NULL;
        //
        fgets(s,sizeof(s)-2,fi);
        onespace(s,s1);
        char *pub = strstr(s1,"public ");
        char *cla = strstr(s1,"class ");
        //
        if( pub && cla && strchr(cla,':')
            && sscanf(cla,"class %s",MyClass) == 1 && sscanf(pub,"public %s",BaseClass) == 1 )
        {
            char *q = strchr(MyClass,':');  // incase 'class Derived:public Base'
            if(q)
                *q = NULL;
            if( MyClass[0] != NULL && BaseClass[0] =='T' && isupper(BaseClass[1]) && islower(BaseClass[2]) )
            {
                //
                // --- FIND A Class Member
                //
                char Cfile[80+1];
                Cfile[0] = NULL;
                char key[MAXPATHSIZE];
                sprintf(key,"%s::",MyClass,MyClass);
                char FnameC[MAXPATHSIZE];

                // Get Symbol Before Destroying 's' string 1.07
                char sym[80+1];
                sym[0] = NULL;
//// new for 1.07
                // if PROJECT.OWL file exists -- USE IT! else use WILD MASKS!
                int use_wildmask = TRUE;
                ProjFile  Project;
                while( ! Project.IsEof()  )
                {
                    char fpath[MAXPATHSIZE];
                    Project.GetFile(fpath);
                    if(strstr(fpath,".c") )
                    {
                        char ThePath[MAXPATHSIZE];
                        char TheFile[MAXPATHSIZE];
                        PathSplit(ThePath,TheFile,fpath);
                        //
                        char thisdir[MAXPATHSIZE];
                        getcwd(thisdir,sizeof(thisdir));
                        ChPath(ThePath);
                        ///
                        GrepFiles(FALSE,NULL,NULL,key,"(","",TheFile,Cfile,s);
                        ChPath(thisdir);
                        if(Cfile[0] != NULL)
                        {
                            use_wildmask = FALSE;
                            PrettyPathFile(path);
                            PrettyPathFile(Cfile);
                            GetThisSymbol(strstr(s,"ID_"),sym);
                            sprintf(s,"%s : %s ( %s%s %s%s )",MyClass,BaseClass,path,fname,ThePath,TheFile);
                            break;
                        }
                    }
                }
                Project.Close();

                if(use_wildmask)
                {
                    strcpy(FnameC,fname);
                    ForceExt(FnameC,".c");
                    GrepFiles(FALSE,NULL,NULL,key,"(","",FnameC,Cfile,s);
                    ForceExt(FnameC,".cpp");
                    if(Cfile[0] == NULL)
                        GrepFiles(FALSE,NULL,NULL,key,"(","",FnameC,Cfile,s);
                    if(Cfile[0] == NULL)
                        GrepFiles(FALSE,NULL,NULL,key,"(","","*.cpp",Cfile,s);
                    if(Cfile[0] == NULL)
                        GrepFiles(FALSE,NULL,NULL,key,"(","","*.c",Cfile,s);
                    //
                    PrettyPathFile(fname);
                    PrettyPathFile(Cfile);
                    GetThisSymbol(strstr(s,"ID_"),sym);
                    sprintf(s,"%s : %s ( %s%s %s%s )",MyClass,BaseClass,path,fname,path,Cfile);
                }

                //
                if(sym[0])
                {
                    strcat(s," id="); strcat(s,sym);
                }
                pLB->InsertString(s,-2);    // Sorted!
            }
        }
    }
    fclose(fi);
    return 1;
}



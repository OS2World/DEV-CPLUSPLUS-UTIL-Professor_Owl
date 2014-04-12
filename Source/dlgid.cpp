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
#include    "dlgid.h"
#include    "parse.h"
#include    "projfile.h"
//


void DlgId::Scanmask(char *path, char *fmask, TListBox *pLB)
{
    char thisdir[256+1];
    getcwd(thisdir,sizeof(thisdir));
    ChPath(path);

    struct ffblk dta;
    int done = findfirst(fmask, &dta, FA_DIREC + FA_SYSTEM + FA_HIDDEN);
    while( !done )
    {
        Scanfile(path, dta.ff_name, pLB);
        done = findnext(&dta);
    }
    ChPath(thisdir);
}

//
// Scan Resource File for DLGTEMPLATE ID_name' that has no class
//
// ADD String To ListBox as "ID_UNUSEDDLG ( resource.h codefile.cpp)
//                       or "ID_UNDEFINED ( ? ? )"
//                       or "ID_UNDEFINED ( resource.h ? )"
//                       or "ID_UNDEFINED ( ? myfile.cpp )"
//
int DlgId::Scanfile(char *path, char *fname, TListBox *pLB)
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
        //
        fgets(s,sizeof(s)-2,fi);
        onespace(s,s1);
        if(strstr(s1,"DLGTEMPLATE"))
        {
            char id[80+1];
            id[0] = NULL;
            if(sscanf(s1,"DLGTEMPLATE %s",id) == 1)
            {
                char Cfile[80+1];
                char Hfile[80+1];
                Cfile[0] = NULL;
                Hfile[0] = NULL;

//// new for 1.07
                // if PROJECT.OWL file exists -- USE IT! else use WILD MASKS!
                int use_wildmask = TRUE;
                ProjFile  Project;
                while( ! Project.IsEof()  )
                {
                    char fpath[256+1];
                    Project.GetFile(fpath);
                    if(strstr(fpath,".c") )
                    {
                        char ThePath[256+1];
                        char TheFile[256+1];
                        PathSplit(ThePath,TheFile,fpath);
                        //
                        char thisdir[256+1];
                        getcwd(thisdir,sizeof(thisdir));
                        ChPath(ThePath);
                        //
                        if(strstr(TheFile,"*.c") != NULL)
                        {
                            GrepFiles(FALSE,NULL,NULL,"::",id,",", TheFile, Cfile,s);
                            if(Cfile[0] == NULL)
                                GrepFiles(FALSE,NULL,NULL,"::",id,",", "*.c",Cfile,s);
                            use_wildmask= FALSE;
                        }

                        if(strstr(TheFile,"*.rc") != NULL || strstr(TheFile,".h") != NULL)
                        {
                            // Find Where constant is defined
                            GrepFiles(FALSE,NULL,NULL,id,"#define ","", TheFile, Hfile,s);
                            if(Hfile[0]==NULL)
                                GrepFiles(FALSE,NULL,NULL,id,"#define ","", TheFile, Hfile,s);
                            if(Hfile[0]==NULL)
                                GrepFiles(FALSE,NULL,NULL,id,"#define ","", TheFile, Hfile,s);
                            use_wildmask= FALSE;
                        }
                        ChPath(thisdir);
                    }
                }
                Project.Close();

                if(use_wildmask)
                {
                    GrepFiles(FALSE,NULL,NULL,"::",id,",", "*.cpp",Cfile,s);
                    if(Cfile[0] == NULL)
                        GrepFiles(FALSE,NULL,NULL,"::",id,",", "*.c",Cfile,s);

                    // Find Where constant is defined
                    GrepFiles(FALSE,NULL,NULL,id,"#define ","", "*.rc",Hfile,s);
                    if(Hfile[0]==NULL)
                        GrepFiles(FALSE,NULL,NULL,id,"#define ","", "*.h",Hfile,s);
                    if(Hfile[0]==NULL)
                        GrepFiles(FALSE,NULL,NULL,id,"#define ","", "*.hpp",Hfile,s);
                }

                if(Cfile[0]==NULL)
                    strcpy(Cfile,"?");
                if(Hfile[0]==NULL)
                    strcpy(Hfile,"?");
                //
                PrettyPathFile(Hfile);
                PrettyPathFile(Cfile);
                sprintf(s,"%s ( %s%s %s%s )",id,path,Hfile,path,Cfile);
                pLB->InsertString(s,-2); // ADD DIALOG ID_
                //
                // EAT LINES UNTILL "END" of dialog found
                //
                do
                {
                    fgets(s,sizeof(s)-2,fi);
                    onespace(s,s1);
                }
                while(!feof(fi)
                    && strncmp(s1,"}",1) != 0       // 1.07 braces or END
                    && strncmp(s1," }",1) != 0
                    && strncmp(s1,"END",3) != 0
                    && strncmp(s1," END",4) != 0);
            }
        }
    }
    fclose(fi);
    return 1;
}



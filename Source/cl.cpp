#include    "def.h"
#include    <stdio.h>
#include    <string.h>
#include    <time.h>
#include    <io.h>
#include    <dir.h>
#include    <owl\listbox.h>
#include    "cl.h"
#include    "parse.h"
#include    "setupdat.h"
#include    "version.h"

extern SETUPDATA    UserSetup;

//
// Make Class Hdr & Source File
//
int Cproto::AddClass(char *Fh, char *Fc, char *Class, char *base, char *Hincludes[], char *Cincludes[], Proto members[])
{
    BOOL AddH_head = (access(Fh,0) == -1);
    BOOL AddC_head = (access(Fc,0) == -1);

    FILE *fh = fopen(Fh,"at");
    if(!fh)
        return 0;

    FILE *fc = fopen(Fc,"at");
    if(!fc)
    {
        fclose(fh);
        return 0;
    }
    if(AddH_head)
    {
        SlapOnHeader(Fh,UserSetup.HFileHead,Class,base);
    }
    if(AddC_head)
    {
        SlapOnHeader(Fc,UserSetup.CFileHead,Class,base);
    }

    fseek(fh,0L,SEEK_END);
    fseek(fc,0L,SEEK_END);

    for(int k=0; Hincludes[k]; k++)      //seperate Hinclude & Cincludes ! 1.03a
    {
        char *p = Hincludes[k];
        if(strchr(p,'"') == NULL && strchr(p,'<') == NULL)  // IF no <> or ""'s
            fprintf(fh,"#include%s\"%s\"\n",UserSetup.Tabs,p); // add double quotes
        else
            fprintf(fh,"#include%s%s\n",UserSetup.Tabs,p);     // else use supplied delimiters
    }
    for(k=0; Cincludes[k]; k++) //1.03a
    {
        char *p = Cincludes[k];
        if(strchr(p,'"') == NULL && strchr(p,'<') == NULL)  // IF no <> or ""'s
            fprintf(fc,"#include%s\"%s\"\n",UserSetup.Tabs,p); // add double quotes
        else
            fprintf(fc,"#include%s%s\n",UserSetup.Tabs,p);     // else use supplied delimiters
    }

    fprintf(fc,"#include%s\"%s\"\n",UserSetup.Tabs,Fh);    // include "hfile.h"
    ///
    fprintf(fh,"\n\nclass %s",Class);
    if(base && *base)
    {
        fprintf(fh," : public %s",base);
    }
    fprintf(fh,"\n{\n");
    //
    fprintf(fh,"private:%s// Variables\n",UserSetup.Tabs);
    fprintf(fh,"public:%s// Methods\n",UserSetup.Tabs);
    //
    for(int i=0; members[i].func_args; i++)
    {
        if(members[i].retype && strcmp(members[i].retype,"@DECLARE") == 0)
        {
            fprintf(fh,"%s%s(%s);\n",UserSetup.Tabs,members[i].func_args,Class);
        }
        else if(members[i].retype == NULL || members[i].retype[0] != '@')
        {
            // Remove call to base class if prototype in header
            // kill off "Me:Me() : Base()"  --> "Me::Me()"
            char ss[MAXPATHSIZE];
            strcpy(ss,members[i].func_args);
            char *pconstruct = strchr(ss,':');
            if(pconstruct)
            {
                pconstruct--;
                while(*pconstruct == ' ')
                    pconstruct--;
                pconstruct++;
                *pconstruct = NULL;
            }
            fprintf(fh,"%s",UserSetup.Tabs);
            if(members[i].retype)
                fprintf(fh,"%s%-4.4s",UserSetup.Tabs,members[i].retype);
            if(members[i].virt)
                fprintf(fh," %s",members[i].virt);
            fprintf(fh," %s;\n",ss);
        }
    }
    fprintf(fh,"protected:%s// Events\n",UserSetup.Tabs);
    fprintf(fh,"}; // end of class %s\n\n",Class);
    fclose(fh);

    //
    // Add functions to C file
    //
    int makemap = TRUE;
    if(strcmp(base,"TDocument")==0 ) // Tdcoument 's dont get a message map table 1.03a
    {
        makemap = FALSE;
    }

    for(i=0; makemap && members[i].func_args; i++)
    {
        if( members[i].retype && strcmp(members[i].retype,"@DEFINE") == 0)
        {
            fprintf(fc,"\n\n%s\n// MESSAGE MAP FOR %s\n%s\n",UserSetup.FuncDelimeter,Class,UserSetup.FuncDelimeter);
            fprintf(fc,"%s(%s,%s)\n",members[i].func_args,Class,base);
        }
        else if( members[i].retype && strcmp(members[i].retype,"@EV") == 0)
        {
            fprintf(fc,"    %s,\n",members[i].func_args);
        }
        else if( members[i].retype && strcmp(members[i].retype,"@END") == 0)
        {
            fprintf(fc,"%s;\n\n\n",members[i].func_args,Class,base);
        }
        else if( members[i].retype == NULL || members[i].retype[0] != '@')
        { // FUNCTIONS -- probably contructors and destructors
            char fa[512+1]; // function arguments
            UnDoDefaultParms(members[i].func_args, fa);
            //
            fprintf(fc,"%s\n// %s\n%s\n",UserSetup.FuncDelimeter,members[i].desc,UserSetup.FuncDelimeter);
            if( members[i].retype && members[i].retype[0] )
                fprintf(fc,"%-4.4s ",members[i].retype);
            fprintf(fc,"%s::%s\n",Class, fa);
            fprintf(fc,"{\n%s%s\n}\n\n",UserSetup.Tabs,UserSetup.CommentFlag);
        }
    }
    fclose(fc);
    return 1;
}

// ------
// pfa = "(int x, int j=32)" || "int x, int j=32"
// return qfa = "(int x, int j)" || "int x, int j"
// ------
void Cproto::UnDoDefaultParms(char *pfa, char *qfa)
{
    char *qfa1 = qfa;
    // ------
    // make "(int x, int j=32)" --> "(int x, int j)"
    // parens are optional
    // ------
    while(*pfa)
    {
        if(*pfa == '=' )// find EQUAL SIGN
        {
            while(*pfa != ')' && *pfa != ',' && *pfa != NULL)
                pfa++;  //EAT "= 123" until comma, ')' or NULL
        }
        if(qfa > qfa1 && *pfa == ',' && *(qfa-1) == ' ')
            qfa--;  // kill space before comma !
        *qfa++ = *pfa++;
    }
    *qfa = NULL;
    char tmp[MAXPATHSIZE];
    onespace(qfa1,tmp);
    TrimString(tmp);
    strcpy(qfa1,tmp);
}



//
// Add variale in header file
// .h file name , class name, and ppp = "public"
// dtype can be "TListBox *" or any type of string
//
int Cproto::AddHdrVariable(char *Fh, char *Class, char *ppp, char *dtype, char *var)
{
    char ppp2[80+1];
    strncpy(ppp2,ppp,sizeof(ppp2)-2);
    if(strchr(ppp2,':') == 0)
        strcat(ppp2,":");
    char key0[MAXPATHSIZE];
    sprintf(key0,"class%s",Class);  // squish is TRUE!!!
    int sectionexists = GrepFiles(TRUE,key0,"};",ppp2,"","",Fh,s1,s);

    char *BeforeKey = "DECLARE_RESPONSE_TABLE";
    //
    if(!sectionexists)  // missing section
    {
        if( ! GrepFiles(TRUE,key0,"};",BeforeKey,"(",";",Fh,s1,s) )
        {
            BeforeKey = "};"; // insert section before end of class!
        }
    }
    //
    //
    FILE *fh = fopen(Fh,"rt");
    if(!fh)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fh,tempname);
    //
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;

    sprintf(key,"class %s",Class);
    while( !feof(fh) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fh);
        fprintf(ft,"%s",s);

        onespace(s,s1);
        if( strstr(s1,key) )
        {
            if(sectionexists)
            {
                while(!feof(fh) )
                {
                    fgets(s,sizeof(s)-2,fh);
                    fprintf(ft,"%s",s);
                    nospace(s,s1);
                    if(strstr(s1,ppp2))
                        break;  // Insert 1st in section
                }
                s[0] = NULL;    // dont dupliacte line!
            }
            else // we need to add this section 'BeforeKey'
            {
                while(!feof(fh) )
                {
                    fgets(s,sizeof(s)-2,fh);
                    onespace(s,s1);
                    if(strstr(s1,BeforeKey))
                    {
                        fprintf(ft,"%s\n",ppp2);
                        break;  // Insert this section
                    }
                    fprintf(ft,"%s",s);
                }
            }

            fprintf(ft,"%s%s %s;\n",UserSetup.Tabs,dtype,var);
            //
            if(s[0])    // remaining line?
                fprintf(ft,"%s",s); // continute original code
            //
            CopyFile(fh,Fh, ft,tempname);
            return 1;
        }
    }
    fclose(fh);
    fclose(ft);
    remove(tempname);
    return 0;
}



int Cproto::RemoveHdrVariable(char *Fh, char *Class, char *dtype, char *var)
{
    FILE *fh = fopen(Fh,"rt");
    if(!fh)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fh,tempname);

    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;
    //
    sprintf(key,"class %s",Class);
    while( !feof(fh) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fh);
        fprintf(ft,"%s",s);
        onespace(s,s1);

        if( strstr(s,key) )
        {
            while(!feof(fh) )
            {
                s[0]=NULL;
                fgets(s,sizeof(s)-2,fh);
                fprintf(ft,"%s",s);
                if(strstr(s,"{") )
                {
                    while(!feof(fh) && strncmp(s,"};",2)!=0 )   // not EOF & not end of class
                    {
                        fgets(s,sizeof(s)-2,fh);
                        //
                        onespace(s,s1);
                        if(strstr(s1,dtype) && strstr(s1,var) && strchr(s1,';') )
                        {
                            CopyFile(fh,Fh, ft,tempname);
                            return 1;
                        }
                        else
                        {
                            fprintf(ft,"%s",s);
                        }
                    }
                }
            }
        }
    }
    fclose(fh);
    fclose(ft);
    remove(tempname);
    return 0;
}



//
// Write code in function that exists...
//
int Cproto::WriteCode(char *Fc, char *Class, char *func, char *code, int append)
{
    char s0[sizeof(s1)];
    if(code == NULL || *code == NULL)
        return 0;   //
    //
    FILE *fc = fopen(Fc,"rt");
    if(!fc)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fc,tempname);
    //
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;

    sprintf(key,"%s::%s(",Class,func);
    while( !feof(fc) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fc);
        fprintf(ft,"%s",s);

        onespace(s,s1);
        nospace(s,s0);
        if( strstr(s0,key) )
        {
            while(!feof(fc) )
            {
                s[0]=NULL;
                fgets(s,sizeof(s)-2,fc);
                //
                if( (!append && s[0] == '{') || (append && s[0]=='}') )
                {
                    if(!append)
                        fprintf(ft,"%s",s);
                    //
                    fprintf(ft,"%s",code);
                    int n = strlen(code);
                    if(n && code[n-1] != '\n')
                        fprintf(ft,"\n");
                    //
                    if(append)
                        fprintf(ft,"%s\n",s);

                    CopyFile(fc,Fc, ft,tempname);
                    return 1;
                }
                else
                {
                    fprintf(ft,"%s",s);
                }
            }
        }
    }
    fclose(fc);
    fclose(ft);
    remove(tempname);
    return 0;
}

//
// AddFunction( HdrFile, CppFile, Class, ppp=privete|protected|public
//              virt=NULL|virtual, retype, func, args, comment, codebody);
// Note: Args can be like "int x, int y, int z = 0)
//
// If Section ppp is not found it is created !
//
int Cproto::AddFunction(char *Fh, char *Fc, char *Class, char *ppp, char *virt, char *retype, char *func, char *args, char *comment, char *code)
{
    char ppp2[80+1];
    strncpy(ppp2,ppp,sizeof(ppp2)-2);
    if(strchr(ppp2,':') == 0)
        strcat(ppp2,":");
    char key0[MAXPATHSIZE];
    sprintf(key0,"class%s",Class);  // squish is TRUE!!!
    int sectionexists = GrepFiles(TRUE,key0,"};",ppp2,"","",Fh,s1,s);

    char *BeforeKey = "DECLARE_RESPONSE_TABLE";
    //
    if(!sectionexists)  // missing section
    {
        if( ! GrepFiles(TRUE,key0,"};",BeforeKey,"(",";",Fh,s1,s) )
        {
            BeforeKey = "};"; // insert section before end of class!
        }
    }
    //
    //
    FILE *fh = fopen(Fh,"rt");
    if(!fh)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fh,tempname);
    //
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;

    sprintf(key,"class %s",Class);
    while( !feof(fh) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fh);
        fprintf(ft,"%s",s);

        onespace(s,s1);
        if( strstr(s1,key) )
        {
            if(sectionexists)
            {
                while(!feof(fh) )
                {
                    fgets(s,sizeof(s)-2,fh);
                    fprintf(ft,"%s",s);
                    nospace(s,s1);
                    if(strstr(s1,ppp2))
                        break;  // Insert 1st in section
                }
                s[0] = NULL;    // dont dupliacte line!
            }
            else // we need to add this section 'BeforeKey'
            {
                while(!feof(fh) )
                {
                    fgets(s,sizeof(s)-2,fh);
                    onespace(s,s1);
                    if(strstr(s1,BeforeKey))
                    {
                        fprintf(ft,"%s\n",ppp2);
                        break;  // Insert this section
                    }
                    fprintf(ft,"%s",s);
                }
            }

            fprintf(ft,"%s",UserSetup.Tabs,retype);
            if(virt && *virt)
                fprintf(ft,"%s ",virt);

            fprintf(ft,"%-4.4s %s(%s);\n",retype,func,args);
            //
            if(s[0])    // remaining line?
                fprintf(ft,"%s",s); // continute original code
            //
            CopyFile(fh,Fh, ft,tempname);
            //
            // C++ Code
            // ADD MEMBER FUNCTION
            //
            FILE *fc = fopen(Fc,"at");
            if(fc)
            {
                fseek(fc,0L,SEEK_END);
                if(comment && *comment)
                    fprintf(fc,"\n%s\n// %s\n%s\n",UserSetup.FuncDelimeter,comment,UserSetup.FuncDelimeter);
                else
                    fprintf(fc,"\n%s\n//\n%s\n",UserSetup.FuncDelimeter,UserSetup.FuncDelimeter);
                //
                char CppArgs[MAXPATHSIZE];
                UnDoDefaultParms(args, CppArgs);
                fprintf(fc,"%s %s::%s(%s)\n",retype,Class,func,CppArgs);
                fprintf(fc,"{\n%s%s\n",UserSetup.Tabs,UserSetup.CommentFlag);
                if(code && *code)
                {
                    fprintf(fc,"%s",code);
                    int n = strlen(code);
                    if(n && code[n-1] != '\n')
                        fprintf(fc,"\n");
                }
                fprintf(fc,"}\n\n");
                fclose(fc);
            }
            return 1;
        }
    }
    fclose(fh);
    fclose(ft);
    remove(tempname);
    return 0;
}


int Cproto::RemoveFunction(char *Fh, char *Fc, char *Class, char *func, char *args)
{
    FILE *fh = fopen(Fh,"rt");
    if(!fh)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fh,tempname);
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;
    //
    sprintf(key,"class %s",Class);
    while( !feof(fh) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fh);
        fprintf(ft,"%s",s);
        onespace(s,s1);

        if( strstr(s,key) )
        {
            while(!feof(fh) )
            {
                s[0]=NULL;
                fgets(s,sizeof(s)-2,fh);
                fprintf(ft,"%s",s);
                if(strstr(s,"{") )
                {
                    while(!feof(fh) && strncmp(s,"};",2)!=0 )   // not EOF & not end of class
                    {
                        fgets(s,sizeof(s)-2,fh);
                        //
                        char args1[MAXPATHSIZE];
                        onespace(args,args1);
                        onespace(s,s1);
                        if(strstr(s1,func)
                           && strchr(s1,'(') && strchr(s1,')') && strchr(s1,';')
                           && strstr(s1,args1) )  // FUNCTION FOUND ?
                        {
                            CopyFile(fh,Fh, ft,tempname);
                            /// #if 0 COMMENT OUT MEMBER FUNCTION
                            CommentOut(Fc,Class,func,args);
                            return 1;
                        }
                        else
                        {
                            fprintf(ft,"%s",s);
                        }
                    }
                }
            }
        }
    }
    fclose(fh);
    fclose(ft);
    remove(tempname);
    return 0;
}



int Cproto::CommentOut(char *Fc, char *Class, char *func, char *args)
{
    FILE *fc = fopen(Fc,"rt");
    if(!fc)
        return 0;

    char tempname[MAXPATHSIZE];
    TempPathName(Fc,tempname);

    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;
    //
    char key2[MAXPATHSIZE];
    sprintf(key2,"%s::%s(%s)",Class,func,args);
    onespace(key2,key);

    while( !feof(fc) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fc);
        onespace(s,s1);
        //
        if( strstr(s1,key) )
        {
            time_t now;
            time(&now);
            char nows[80+1];
            strcpy(nows,ctime(&now));
            fprintf(ft,"#if 0 //DELETE_BEGIN %s\n//DELETED %s\n",nows,s);
            while(!feof(fc) )
            {
                fgets(s,sizeof(s)-2,fc);
                fprintf(ft,"%s",s);

                if(s[0] == '}')
                {
                    fprintf(ft,"#endif //DELETE_END\n");
                    CopyFile(fc,Fc, ft,tempname);
                    return 1;
                }
            }
        }
        fprintf(ft,"%s",s);
    }
    fclose(fc);
    fclose(ft);
    remove(tempname);
    return 0;
}


//
// Add Event Macro as:
//      EV_WM_PAINT,
//      EV_BN_CLICKED(ID_THEBUTTON,EvClickThebutton),
// Note: EV_MW macros have no parameters
//
int Cproto::AddEvent(char *Fc, char *Class, char *EvMacro, char *Id, char *Func)
{
    char tempname[MAXPATHSIZE];
    TempPathName(Fc,tempname);
    //
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;
    FILE* fc = fopen(Fc,"rt");
    if(!fc)
        return 0;

    while( !feof(fc) )
    {
        char s[MAXPATHSIZE];
        char s1[MAXPATHSIZE];
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fc);
        fprintf(ft,"%s",s);

        onespace(s,s1);
        if( strstr(s1,"DEFINE_RESPONSE_TABLE") && strstr(s1,Class) )
        {
            while(!feof(fc) )
            {
                fgets(s,sizeof(s)-2,fc);
                onespace(s,s1);

                if( strstr(s1,"END_RESPONSE_TABLE") && strchr(s1,';' ) )
                {
                    if(strncmp(EvMacro,"EV_WM_",6)== 0)
                    {
                        fprintf(ft,"    %s,\n",EvMacro);
                    }
                    else // parameters
                    {
                        if(Id && *Id)
                            fprintf(ft,"    %s(%s,%s),\n",EvMacro,Id,Func);
                        else
                            fprintf(ft,"    %s(%s),\n",EvMacro,Func);
                        //
                    }
                    fprintf(ft,"%s",s); // put back end-table
                    CopyFile(fc,Fc, ft,tempname);
                    return 1;
                }
                else
                {
                    fprintf(ft,"%s",s);
                }
            }
        }
    }
    fclose(fc);
    fclose(ft);
    remove(tempname);
    return 0;
}

//
// if inc = "header.h" in double quotes
// then append it at the end of the list
// if the name is in angle brackets put it before
// the 1st double quoted include. this ensures
// that <owl/tlistbox.h> will be BEFORE
// "myclass.h" so that variable in myclass.h will
// know what a TListBox is!
//
int Cproto::AppendIncludeFile(char *fname,char *inc)
{
    FILE *fi = fopen(fname,"rt");
    if(!fi)
        return 0;

    int bracketed = FALSE;
    if(inc[0] == '<' && strchr(inc,'>') )
        bracketed = TRUE;
    //
    char s[MAXPATHSIZE];
    char s1[MAXPATHSIZE];
    long LastInc = 0;
    long line = 0;
    int stop = FALSE;
    while(!feof(fi) && !stop)
    {
        fgets(s,sizeof(s),fi);
        onespace(s,s1);

        line++;
        char *p = strstr(s1,"#include");
        if(p)
        {
            int flagq = FALSE;
            char inc1[MAXPATHSIZE];
            inc1[0] = NULL;
            sscanf(s1,"#include %s",inc1);
            p = strchr(s1,'"');
            if(p)
                flagq = TRUE;
            else
                p = strchr(s1,'<');
            if(p)
            {
                char *p2 = strchr(p,'>');
                if(!p2)
                    p2 = strchr(p,'"');
                if(p2)
                {
                    p2++;
                    *p2 = NULL;
                }

                if(strcmp(p,inc)==0)    // compare strings i.e. "<owl\somefile.h>"
                {
                    fclose(fi); // already exists!
                    return 0;
                }
                // if bracketed then insert before quoted include ?
                if(bracketed && flagq)  // bracket file before quoted ?
                    stop = TRUE;
                else
                    LastInc = line;
            }
        }
    }

    char tempname[MAXPATHSIZE];
    TempPathName(fname,tempname);
    //
    FILE* fo = fopen(tempname,"wt");
    rewind(fi);
    CopyFileLines(fi,fo,LastInc);
    //
    if( bracketed || strchr(inc,'"') )
        fprintf(fo,"#include%s%s\n",UserSetup.Tabs,inc);
    else    // assume to use quote delimiters
        fprintf(fo,"#include%s\"%s\"\n",UserSetup.Tabs,inc);
    //
    CopyFile(fi,fname,fo,tempname);
    return 1;
}

void Cproto::CopyFileLines(FILE *fi,FILE *fo, long line)
{
    while(line--)
    {
        fgets(s,sizeof(s),fi);
        fprintf(fo,"%s",s);
    }
}

void Cproto::CopyFile(FILE *fs, char *src, FILE *fd, char *dest)
{
    // ==========================
    // Copy remainder of file ...
    // ==========================
    char fb[4096];
    int n;
    do
    {
        n = fread(fb,1,sizeof(fb),fs);
        fwrite(fb,1,n,fd);
        //
    }while( sizeof(fb) == n);
    //

    fclose(fs);
    fclose(fd);
    //
    remove(src);
    rename(dest,src);
}

//
// Delete a line of text from a file.
// The first matching line (ignoring the LF) is deleted
//
// return 0 = error nothing deleted - 1 = deleted
//
int Cproto::DeleteLineFromFile(char *fname,char *matchingline)
{
    if(*matchingline == NULL)
        return 0;
    ///
    FILE* fi = fopen(fname,"rt");
    if(!fi)
        return 0;
    //
    char tempname[MAXPATHSIZE];
    TempPathName(fname,tempname);
    //
    FILE* ft = fopen(tempname,"wt");
    if(!ft)
        return 0;

    removeNL(matchingline);

    while(!feof(fi) )
    {
        fgets(s,sizeof(s),fi);
        strcpy(s1,s);
        removeNL(s1);
        if(strcmp(s1,matchingline) == 0)
        {
            CopyFile(fi,fname,ft,tempname);
            return 1;
        }
        fprintf(ft,"%s",s);
    }
    fclose(fi);
    fclose(ft);
    remove(tempname);
    return 0;
}


//
// Scan For variables as  "MyListBox = new TListBox(this,ID_MYBOX);"
//  add string to listbox
//
int Cproto::ScanVariables(char *Fh, char *Fc, char *Class, TListBox *Lb)
{
    FILE *fc = fopen(Fc,"rt");
    if(!fc)
        return 1;

    sprintf(key,"%s::%s",Class,Class);      // look for Constructor
    while( !feof(fc) )
    {
        s[0]=NULL;
        fgets(s,sizeof(s)-2,fc);
        nospace(s,s0);
        onespace(s,s1);
        //
        if( strstr(s0,key) ) // Look For Constructor
        {
            while(!feof(fc) )
            {
                fgets(s,sizeof(s)-2,fc);

                if(s[0] == '{') // inside function ?
                {
                    while(!feof(fc) )
                    {
                        fgets(s,sizeof(s)-2,fc);
                        onespace(s,s1);
                        if(s[0] == '}')
                        {
                            fclose(fc);
                            return(0);
                        }
                        /// INSIDE FUNCTION !!!
                        char *Tok[] =
                        {
                            "=",
                            "new",
                            "(",
                            "this",
                            ",",
                            ")",
                            ";",
                            NULL
                        };
                        char *Tok2[] = { "=",";",NULL };
                        //
                        if(StringOrder(s1,Tok) )
                        {
                            char *p = strstr(s1,"new");
                            if(p)
                            {
                                char ty[MAXPATHSIZE];
                                char sym[MAXPATHSIZE];
                                GetThisSymbol(s1,sym);
                                GetThisSymbol(p+4,ty);
                                char fname[MAXPATHSIZE];
                                char s2[MAXPATHSIZE];
                                if( GrepFiles(FALSE,Class,"};",sym,ty,";",Fh,fname,s2) )
                                {
                                    removeNL(s1);
                                    p = s1;
                                    while(*p == ' ')
                                        p++;
                                    Lb->InsertString(p,-2);
                                }
                            }
                        }
                        else if(StringOrder(s1,Tok2) )
                        {
                            char *p = strstr(s1,"=");
                            if(p)
                            {
                                char sym[MAXPATHSIZE];
                                GetThisSymbol(s1,sym);
                                char fname[MAXPATHSIZE];
                                char s2[MAXPATHSIZE];
                                if( GrepFiles(FALSE,Class,"};",sym," ",";",Fh,fname,s2) )
                                {
                                    removeNL(s1);
                                    p = s1;
                                    while(*p == ' ')
                                        p++;
                                    Lb->InsertString(p,-2);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fclose(fc);
    return 0;
}

// ---------------------------------------------
// Slap on the .\_head_.h or _head_.cpp file
// at the beginnering of a newly created file.
// expand macros i.e. $names --  '$time" = ctime()
// $class = MyClassName  -- $base == my base class
// ---------------------------------------------
void SlapOnHeader(char *fname, char *src, char *Class, char *base)
{
    if(!src || *src == '\0')
    {
        return;
    }
    FILE *fs = fopen(src,"rt");
    if(!fs)
        return;

    FILE *fo = fopen(fname,"at");
    if(!fo)
    {
        fclose(fs);
        return;
    }

    while( !feof(fs) )
    {
        char s[MAXPATHSIZE];
        s[0] = NULL;
        fgets(s,sizeof(s)-1, fs);
        char ss[sizeof(s) + 100];
        ExpandDollar(s,ss,fname,Class,base);
        fprintf(fo,"%s",ss);
    }
    fclose(fs);
    fclose(fo);
}

extern char MyHomePath[];

// ------------------------------------------------
// $time = "Thu Oct 12 23:54:54 1995"
// $file = MYFILE.H or MYFILE.CPP
// $path = current path as "D:\pro-owl\os2"
// $home = "D:\pr103a\"
// $product Professor Owl 1.07
// $base    TDialog
// $class   TMyClass
// ------------------------------------------------
void ExpandDollar(char *in, char *s, char *fname,char *Class, char *BaseClass)
{
    char basefname[MAXPATHSIZE];
    strcpy(basefname,fname);
    char *p = strchr(basefname,'.');
    if(p)
        *p = NULL;

    char tail[512+1];
    strcpy(s, in);

    p = s;
    while( (p= strchr(p,'$')) != NULL )
    {
        if( p && strnicmp(p,"$file",5)==0)
        {
            int uc = isupper(*(p+1) );
            strcpy(tail,p+5);
            strcpy(p,basefname);
            if(uc)
                strupr(p);
            else
                strlwr(p);
            strcat(p,tail);
            p += strlen(basefname);
        }
        else if( p && strnicmp(p,"$time",5) == 0)
        {
            int uc = isupper(*(p+1) );
            strcpy(tail,p+5);
            time_t now;
            time(&now);
            char nows[80+1];
            strcpy(nows,ctime(&now));
            int l = strlen(nows);
            if(l > 1 && nows[l-1] == '\n')
            {
                nows[l-1] = NULL;
            }
            l = sprintf(p,"%s",nows );
            if(uc)
                strupr(p);
            else
                strlwr(p);
            strcat(p,tail);
            p += l;
        }
        else if( p && strnicmp(p,"$path",5) == 0)
        {
            int uc = isupper(*(p+1) );
            strcpy(tail,p+5);
            char path[257+1];
            getcwd(path,sizeof(path)-1);
            int l = strlen(path);
            if(l > 1 && path[l-1] == '\\')
            {
                path[l-1] = NULL;
            }
            sprintf(p,"%s",path );
            if(uc)
                strupr(p);
            else
                strlwr(p);
            strcat(p,tail);
            p += strlen(path);
        }
        else if( p && strnicmp(p,"$home",5) == 0)
        {
            int uc = isupper(*(p+1) );
            strcpy(tail,p+5);
            sprintf(p,"%s",MyHomePath );
            if(uc)
                strupr(p);
            else
                strlwr(p);
            strcat(p,tail);
            p += strlen(MyHomePath);
        }
        else if( p && strnicmp(p,"$product",8) == 0)
        {
            strcpy(tail,p+8);
            int l = sprintf(p,"%s",PROFESSOR_OWL_VERSION );
            strcat(p,tail);
            p += l;
        }
        else if( p && strnicmp(p,"$class",6) == 0)
        {
            strcpy(tail,p+6);
            int l = sprintf(p,"%s",Class);
            strcat(p,tail);
            p += l;
        }
        else if( p && strnicmp(p,"$base",5) == 0)
        {
            strcpy(tail,p+5);
            int l = sprintf(p,"%s",BaseClass);
            strcat(p,tail);
            p += l;
        }
        else
        {
            p++; // avoid dead loop if $unknown
        }
    }
}

//
// Make a Temp file name in the same directory
// as the real file name - so when we delete the
// real name and rename the tempname to the real
// name it works - and we are not crossing drives
// or paths or leaving temp$.$$$ files in the
// current dir! added for 1.07
//
void TempPathName(char *real, char *tempname)
{
    char thisdir[MAXPATHSIZE];
    getcwd(thisdir,sizeof(thisdir));        // save directory we are in
    //
    char ThePath[MAXPATHSIZE];
    char TheFile[MAXPATHSIZE];
    PathSplit(ThePath,TheFile, real);       // find out where oter file is
    //
    ChPath(ThePath);                        // go to other files path
    tmpnam(tempname);                       // make a temp file name there
    ChPath(thisdir);                        // go back we we started
    //
    strcat(ThePath,tempname);               // return path to tempfile
    strcpy(tempname,ThePath);               // as f:\somedir\tmp$.$00
}





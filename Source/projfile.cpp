//
// READ  PROFOWL.OWL Project File The Easy Way
//
#include    "def.h"
#include    <os2.h>
//
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    "projfile.h"
#include    "parse.h"


extern char MyProjectPath[];


ProjFile::ProjFile(void)
{
    sprintf(FileName,"%s\\PROJECT.OWL",MyProjectPath);
    Fi = fopen(FileName,"rt");
}


ProjFile::~ProjFile()
{
    Close();
}


void ProjFile::Rewind(void)
{
    rewind(Fi);
}

void ProjFile::getrawstring(char *s)
{
    *s = NULL;
    if(Fi)
    {
        fgets(s,256,Fi);
    }
}

void ProjFile::GetFile(char *s)
{
    *s = NULL;
    if(!Fi)
    {
        return;
    }
    // GET A VALID DRIVE:\PATH\FILENAME
    while( !feof(Fi) )
    {
        getrawstring(s);
        if(s[1] == ':' && s[2] == '\\')
        {
            TrimString(s);
            // All LOWERCASE & a little magic
            PrettyPathFile(s);
            return;
        }
    }
    *s = NULL;
}


int ProjFile::IsEof(void)
{
    if(!Fi || feof(Fi) )
        return(TRUE);
    return(FALSE);
}


void ProjFile::AppendString(char *add1, char *add2)
{
    char TempName[MAXPATHSIZE];
    sprintf(TempName,"%s\\PROJECT.~WL",MyProjectPath);
    FILE *fo = fopen(TempName,"wt");
    if(!fo)
        return;     // OOPS! cant open temp file !
    //
    char s[MAXPATHSIZE];
    while( ! IsEof() )
    {
        getrawstring(s);
        if(s[0] == '{')     // look for {end}
        {
            fprintf(fo,"%s\n",add1);
            fprintf(fo,"%s\n",add2);
        }
        if(s[0])
        {
            fprintf(fo,"%s",s);
        }
    }
    fclose(Fi);
    Fi = NULL;
    fclose(fo);
    remove(FileName);
    rename(TempName,FileName);
    Fi = fopen(FileName,"rt");
}


void ProjFile::Close(void)
{
    if(Fi)
    {
        fclose(Fi);
        Fi = NULL;
    }
}




//
// looks like this 'C:\Mydir\Subdir\File.cpp'
// (extension is always lowercase)
//
void PrettyPathFile(char *s)
{
    strlwr(s);
    s[0] = toupper(s[0]); // Uppcase 1st Drive 'C':\ or Filename.cpp
    // Format it nice uppcase 1st letter
    // of Dir's & Files names
    //
    char *p = s;
    while(*p)
    {
        if(*p++ ==  '\\')
        {
            if(*p)
            {
                *p = toupper(*p);
            }
        }
    }
}



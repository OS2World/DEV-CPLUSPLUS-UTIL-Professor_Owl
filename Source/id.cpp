#include    "def.h"
#include    <stdio.h>
#include	<string.h>
#include	<time.h>
#include	<dos.h>
#include	<dir.h>
//
#include    "id.h"

void ID::ScanRCmask(char *fmask)
{
	struct ffblk dta;
	int done = findfirst(fmask, &dta, FA_DIREC + FA_SYSTEM + FA_HIDDEN);
	while( !done )
	{
		ScanRCfile(dta.ff_name);
		done = findnext(&dta);
	}
}

int ID::ScanRCfile(char *fname)
{
	FILE *fi = fopen(fname,"rt");
	if(!fi)
		return 0;
	//
	while(!feof(fi))
	{
		char hdr[80+1];
		hdr[0] = NULL;
		fgets(s,sizeof(s)-2,fi);
		if( (strstr(s,"#include")
			&& (sscanf(s,"#include \"%s",hdr) == 1 || sscanf(s,"#include <%s",hdr) == 1) ) )
		{
			int n = strlen(hdr);
			if(n && ( hdr[n-1] == '"' || hdr[n-1] == '>') )
				hdr[n-1] = NULL;	// remove closing quote
			ScanHfile(hdr);
		}
	}
	fclose(fi);
}


int ID::ScanHfile(char *fname)
{
	FILE *fi = fopen(fname,"rt");
	if(!fi)
		return 0;

	while(!feof(fi))
	{
		char con[80+1];
		con[0] = NULL;
		fgets(s,sizeof(s)-2,fi);
		if(strstr(s,"#define") && strstr(s,"ID") && sscanf(s,"#define %s",con) == 1)
		{
		}
	}
	fclose(fi);
}


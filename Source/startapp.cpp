#include    "def.h"
#define INCL_DOSSESMGR
#define INCL_PM
#define NULL 0
#include    <os2.h>
#include    <stdio.h>
#include    <mem.h>
#include    <string.h>
#include    <stdlib.h>
#include    "parse.h"
#include    "startapp.h"

//
// Start application and send message to WINDOW that
// application is done when app is exitted...
//
long StartApp(HWND hwnd, char *title, char *dir, char *app, char *parm1)
{
    char parms[MAXPATHSIZE];
    memset(parms,0,sizeof(parms)-1);
    strcpy(parms,parm1);
    PROGDETAILS  Details;
    Details.Length          = sizeof(PROGDETAILS);
    Details.progt.progc     = PROG_DEFAULT;
    Details.progt.fbVisible = SHE_VISIBLE;
    Details.pszTitle        = title;
    Details.pszExecutable   = app;
    Details.pszParameters   = parms;
    Details.pszStartupDir   = dir;
    Details.pszIcon         = NULL;
    Details.pszEnvironment  = NULL;   //
    Details.swpInitial.fl   = SWP_ACTIVATE; // window positioning
    Details.swpInitial.cy   = 0;      // width of window
    Details.swpInitial.cx   = 0;      // height of window
    Details.swpInitial.y    = 0;      // lower edge of window
    Details.swpInitial.x    = 0;      // left edge of window
    Details.swpInitial.hwndInsertBehind = HWND_TOP;
    Details.swpInitial.hwnd             = hwnd;
    Details.swpInitial.ulReserved1      = 0;
    Details.swpInitial.ulReserved2      = 0;
    return WinStartApp( hwnd, &Details, NULL, NULL, SAF_INSTALLEDCMDLINE);
}


////////////////////////
extern char MyName[];
extern char MyHomePath[];

void Help(char *section)
{
    static char MyInf[MAXPATHSIZE];
    strcpy(MyInf,MyName);
    ForceExt(MyInf,".INF");
    if(section && *section)
    {
        strcat(MyInf," ");
        strcat(MyInf,section);
    }
    StartApp(NULLHANDLE,"Professor Owl Help",MyHomePath,"view.exe",MyInf);
}



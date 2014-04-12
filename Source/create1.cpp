#include    "def.h"
#include    <stdio.h>
#include    <owl\listbox.h>
#include    <owl\dialog.h>
#include    <stdio.h>
#include    "cl.h"
#include    "bigmess.h"
#include    "parse.h"
#include    "create1.h"
#include    "setupdat.h"
#include    "version.h"
#include    "startapp.h"


extern SETUPDATA    UserSetup;
//
// return 0 = no errror
//
int CreateClassTDialog(char *Hfile, char *Cfile,char *MyClass,char *MyId,char *incfile)
{
    char *BaseClass = "TDialog";
    char constructor[80+1];
    char destructor[80+1];
    char *virt1 = "virtual";

    Proto z[] =
    {
            NULL,NULL,"@DEFINE","DEFINE_RESPONSE_TABLE1",
            NULL,NULL,"@EV",    "EV_BN_CLICKED(IDOK,EvClickOk)",
            NULL,NULL,"@EV",    "EV_BN_CLICKED(IDCANCEL,EvClickCancel)",
            NULL,NULL,"@END",   "END_RESPONSE_TABLE",
            //
            "Constructor",NULL, NULL,constructor,
            "Destructor",virt1,  NULL,destructor,
            "Init Dialog","virtual", "BOOL","EvInitDialog(HWND hWndFocus)",
            "Cancel Button Pressed",NULL,"void","EvClickCancel(void)",
            "OK Button Pressed",NULL,"void","EvClickOk(void)",
            //
            NULL,NULL,"@DECLARE",   "DECLARE_RESPONSE_TABLE",
            NULL,NULL,NULL,NULL
    };

    sprintf(constructor,"%s(TWindow *parent) : TDialog(parent, %s)",MyClass,MyId);
    sprintf(destructor,"~%s()",MyClass);

    char owl_header[80+1];
    GetHeaderForClass(owl_header, BaseClass);
    char *Hincludes[] =
    {
        owl_header,
        NULL
    };

    char *Cincludes[] =
    {
        incfile,
        NULL
    };


    Cproto X;
    X.AddClass(Hfile,Cfile,MyClass,BaseClass,Hincludes,Cincludes,z);
    char cd1[256+1];
    sprintf(cd1,"%sBOOL RetValue = TDialog::EvInitDialog(hWndFocus);",UserSetup.Tabs);
    X.WriteCode(Cfile,MyClass,"EvInitDialog",cd1);
    sprintf(cd1,"%s%s",UserSetup.Tabs,UserSetup.CommentFlag);
    X.WriteCode(Cfile,MyClass,"EvInitDialog",cd1);
    sprintf(cd1,"%sreturn(RetValue);",UserSetup.Tabs);
    X.WriteCode(Cfile,MyClass,"EvInitDialog",cd1);
    ///
    ///
    ///
    sprintf(cd1,"%sCmOk();",UserSetup.Tabs);
    X.WriteCode(Cfile,MyClass,"EvClickOk",cd1,TRUE);
    sprintf(cd1,"%sCmCancel();",UserSetup.Tabs);
    X.WriteCode(Cfile,MyClass,"EvClickCancel",cd1,TRUE);
    //
    return 0;
}

//
//
//
//
int CreateClassTWindow(char *Hfile, char *Cfile,char *MyClass,char *incfile)
{
    char *BaseClass = "TWindow";
    char constructor[80+1];
    char destructor[80+1];
    char *virt1 = "virtual";
    //
    Proto z[] =
    {
            NULL,NULL,"@DEFINE","DEFINE_RESPONSE_TABLE1",
            NULL,NULL,"@END",   "END_RESPONSE_TABLE",
            //
            "Constructor",NULL,NULL,constructor,
            "Destructor",virt1,NULL,destructor,
            //
            NULL,NULL,"@DECLARE","DECLARE_RESPONSE_TABLE",
            NULL,NULL,NULL,NULL
    };

    sprintf(constructor,"%s(TWindow *parent) : %s(parent, 0)",MyClass,BaseClass);
    sprintf(destructor,"~%s()",MyClass);

    char owl_header[80+1];
    GetHeaderForClass(owl_header, BaseClass);
    char *Hincludes[] =
    {
        owl_header,
        NULL
    };
    char *Cincludes[] =
    {
        incfile,
        NULL
    };

    Cproto X;
    X.AddClass(Hfile,Cfile,MyClass,BaseClass,Hincludes,Cincludes,z);
    return 0;
}

//
//
int CreateClassTgeneric(char *Hfile, char *Cfile,char *MyClass,char *BaseClass,char *MyId,char *incfile)
{
    char constructor[512+1];    // they can be long !
    char destructor[128+1];
    //
    char *virt1 = NULL;
    if(IsVirtualDestructorNeeded(BaseClass))
        virt1 = "virtual";
    //
    Proto z[] =
    {
            NULL,NULL,"@DEFINE","DEFINE_RESPONSE_TABLE1",
            NULL,NULL,"@END",   "END_RESPONSE_TABLE",
            //
            "Constructor",NULL, NULL,constructor,
            "Destructor",virt1, NULL,destructor,
            //
            NULL,NULL,"@DECLARE","DECLARE_RESPONSE_TABLE",
            NULL,NULL,NULL,NULL
    };

    GetConstructorForClass(constructor,MyClass,BaseClass);
    sprintf(destructor,"~%s()",MyClass);

    char owl_header[80+1];
    GetHeaderForClass(owl_header, BaseClass);
    char *Hincludes[] =
    {
        owl_header,
        NULL
    };
    char *Cincludes[] =
    {
        incfile,
        NULL
    };


    Cproto X;
    X.AddClass(Hfile,Cfile,MyClass,BaseClass,Hincludes,Cincludes,z);
    //
    if(strncmp(BaseClass,"TView",5) != 0
        && strncmp(BaseClass,"TDocu",5) != 0
        && strncmp(BaseClass,"TFileDocu",9) != 0 )
    {   // NOT A DOC or VIEW class then Call Destroy() in desturctor() !
        char des[80+1];
        sprintf(des,"~%s",MyClass);
        char destroy[80+1];
        sprintf(destroy,"%sDestroy();",UserSetup.Tabs);
        X.WriteCode(Cfile,MyClass,des,destroy,FALSE);
    }
    return 0;
}


//
// Return TRUE if TApplication not found
//
char *TestNewAppNeeded(void)
{
    static char line[256+1];
    char fname2[256+1];
    GrepFiles(FALSE,NULL,NULL,"class",
                        "TApplication",
                        "public",
                        "*.c*",fname2,line);
    return line;
}


void WriteHexBlock(FILE *fo, int bytes, unsigned char p[])
{
    while(bytes)
    {
        fprintf(fo,"    '");
        for(int x=0; bytes && x<16; x++)
        {
            if(x > 0)
                fprintf(fo," ");
            fprintf(fo,"%02.02X",*p++);
            bytes--;
        }
        fprintf(fo,"'\n");
    }
}




// --------------------------------------------------------------------------
// Create all files necessary for application
// MYAPP.CPP                    FApp
// RESOURCE.RC  RESOURCE.H      FRes
// MAINDLG.CPP  MAINDLG.H       FMain
// MAINWIN.H    MAINWIN.CPP     FMain
// ABOUT.H      ABOUT.CPP       FAbout
// --------------------------------------------------------------------------
int CreateApplication(TWindow *Win, int Flags,char *FApp,char *FMain,char *FAbout,char *FRes)
{
    strlwr(FApp);
    strlwr(FMain);
    strlwr(FAbout);
    strlwr(FRes);
    //////////////////
    ForceExt(FApp,".cpp");
    //
    char FMainH[80+1];
    char FMainCPP[80+1];
    strcpy(FMainH,FMain);
    strcpy(FMainCPP,FMain);
    ForceExt(FMainCPP,".cpp");
    ForceExt(FMainH,".h");
    //
    char FAboutH[80+1];
    char FAboutCPP[80+1];
    strcpy(FAboutH,FAbout);
    strcpy(FAboutCPP,FAbout);
    ForceExt(FAboutH,".h");
    ForceExt(FAboutCPP,".cpp");
    //
    char FResH[80+1];
    char FResRC[80+1];
    strcpy(FResH,FRes);
    strcpy(FResRC,FRes);
    ForceExt(FResH,".h");
    ForceExt(FResRC,".rc");
    //
    ///
    int decorated = (Flags & ( NEWAPPFLAG_CONTROLBAR | NEWAPPFLAG_STATUSBAR ) );
    int retCode = 0;
    //
    //
    FILE *fMyApp = fopen(FApp,"wt");
    if(!fMyApp)
    {
        return 1;
    }
    fprintf(fMyApp,"#include <%s\\owlpch.h>\n",UserSetup.OwlPath);
    fprintf(fMyApp,"#include <%s\\applicat.h>\n",UserSetup.OwlPath);

    if(decorated)
    {
        if(Flags & NEWAPPFLAG_STATUSBAR)
        {
            fprintf(fMyApp,"#include <%s\\statusba.h>\n",UserSetup.OwlPath);
        }
        if(Flags & NEWAPPFLAG_CONTROLBAR)
        {
            fprintf(fMyApp,"#include <%s\\controlb.h>\n",UserSetup.OwlPath);
            fprintf(fMyApp,"#include <%s\\buttonga.h>\n",UserSetup.OwlPath);
        }
        fprintf(fMyApp,"#include <%s\\decframe.h>\n",UserSetup.OwlPath);
    }
    else // non decorated
    {
        fprintf(fMyApp,"#include <%s\\framewin.h>\n",UserSetup.OwlPath);
    }

    fprintf(fMyApp,"#include \"%s\"\n",FResH);
    fprintf(fMyApp,"#include <%s\\dialog.h>\n",UserSetup.OwlPath);
    //
    fprintf(fMyApp,"#include \"%s\"\n",FMainH);
    fprintf(fMyApp,"#include \"%s\"\n",FAboutH);
    fprintf(fMyApp,"//\n");
    fprintf(fMyApp,"//\n");
    fprintf(fMyApp,"// My Application Class\n");
    fprintf(fMyApp,"//\n");
    fprintf(fMyApp,"class TMyApp : public TApplication\n");
    fprintf(fMyApp,"{\n");
    fprintf(fMyApp,"public:\n");
    fprintf(fMyApp,"%sTMyApp(void);\n",UserSetup.Tabs);
    fprintf(fMyApp,"protected:\n");
    fprintf(fMyApp,"%svoid InitMainWindow(void);\n",UserSetup.Tabs);
    fprintf(fMyApp,"};\n\n\n");
    ///
    fprintf(fMyApp,"// My Application Constructor\n");
    fprintf(fMyApp,"TMyApp::TMyApp(void) : TApplication()\n");
    fprintf(fMyApp,"{\n");
    fprintf(fMyApp,"}\n\n");
    //
    fprintf(fMyApp,"void TMyApp::InitMainWindow(void)\n");
    fprintf(fMyApp,"{\n");

    if(decorated)
    {   // USE DECORATED FRAME
        if(Flags & NEWAPPFLAG_DIALOG)
            fprintf(fMyApp,"%sTDecoratedFrame *Frame = new TDecoratedFrame(0, \"My Decorated Frame Application\", new MainDlg((TWindow*)0));\n",UserSetup.Tabs);
        if(Flags & NEWAPPFLAG_SDI)
            fprintf(fMyApp,"%sTDecoratedFrame *Frame = new TDecoratedFrame(0, \"My Decorated Frame Application\", new MainWin((TWindow*)0));\n",UserSetup.Tabs);

        if(Flags & NEWAPPFLAG_CONTROLBAR)
        {
            fprintf(fMyApp,"%sTControlBar *Cbar = new TControlBar(Frame);\n",UserSetup.Tabs);
            fprintf(fMyApp,"%sCbar->Insert(*new TButtonGadget(BM_ABOUT,CM_ABOUT,TButtonGadget::Command) );\n",UserSetup.Tabs);
            fprintf(fMyApp,"%sFrame->Insert(*Cbar, TDecoratedFrame::Top);\n",UserSetup.Tabs);
        }
        if(Flags & NEWAPPFLAG_STATUSBAR)
        {
            fprintf(fMyApp,"%sTStatusBar *Sbar = new TStatusBar(Frame,TGadget::Embossed);\n",UserSetup.Tabs);
            fprintf(fMyApp,"%sFrame->Insert(*Sbar, TDecoratedFrame::Bottom);\n",UserSetup.Tabs);
        }
        fprintf(fMyApp,"%sSetMainWindow( Frame );\n",UserSetup.Tabs);
    }
    else // No Decorated Frame
    {
        if(Flags & NEWAPPFLAG_DIALOG)
            fprintf(fMyApp,"%sSetMainWindow( new TFrameWindow(0, \"My Dialog Application\", new MainDlg((TWindow*)0) ));\n",UserSetup.Tabs);
        else if(Flags & NEWAPPFLAG_SDI)
            fprintf(fMyApp,"%sSetMainWindow( new TFrameWindow(0, \"My Window Application\", new MainWin(0) ));\n",UserSetup.Tabs);
        //
        fprintf(fMyApp,"%sMainWindow->SetIcon(this,ID_MAINICON);\n",UserSetup.Tabs);
        //
    }
    if(Flags & NEWAPPFLAG_DIALOG)
    {
        fprintf(fMyApp,"%sMainWindow->Attr.Style &= ~WS_MAXIMIZEBOX;\n",UserSetup.Tabs);
    }
    if(Flags & NEWAPPFLAG_MENU)
    {
        fprintf(fMyApp,"%sGetMainWindow()->AssignMenu(ID_MAINMENU);\n",UserSetup.Tabs);
    }
    fprintf(fMyApp,"}\n\n");
    fprintf(fMyApp,"int OwlMain(int argc, char *argv[])\n");
    fprintf(fMyApp,"{\n");
    fprintf(fMyApp,"%sreturn TMyApp().Run();\n",UserSetup.Tabs);
    fprintf(fMyApp,"}\n");
    fclose(fMyApp);
    // -----------------------------------

    FILE *fR = fopen(FResH,"wt");
    if(!fR)
    {
        return 2;
    }
    fprintf(fR,"// Resouce ID Constants\n");
    fprintf(fR,"#define%sID_MAINICON%s1\n",UserSetup.Tabs,UserSetup.Tabs);
    fprintf(fR,"#define%sCM_ABOUT%s997\n",UserSetup.Tabs,UserSetup.Tabs);
    fprintf(fR,"#define%sID_ABOUTDIALOG%s1002\n",UserSetup.Tabs,UserSetup.Tabs);

    if(Flags & NEWAPPFLAG_DIALOG)
    {
        fprintf(fR,"#define%sID_MAINDIALOG%s1001\n",UserSetup.Tabs,UserSetup.Tabs);
    }
    if(Flags & NEWAPPFLAG_MENU)
    {
        fprintf(fR,"#define%sID_MAINMENU%s101\n",UserSetup.Tabs,UserSetup.Tabs);
    }

    if(Flags & (NEWAPPFLAG_MENU | NEWAPPFLAG_DIALOG) )
    {
        fprintf(fR,"#define%sCM_HELP%s301\n",UserSetup.Tabs,UserSetup.Tabs);
        fprintf(fR,"#define%sCM_HELPINDEX%s302\n",UserSetup.Tabs,UserSetup.Tabs);
    }
    if(Flags & NEWAPPFLAG_CONTROLBAR)
    {
        fprintf(fR,"#define%sBM_ABOUT%s398\n",UserSetup.Tabs,UserSetup.Tabs);
    }
    fclose(fR);
    ///-----------------------------------
    fR = fopen(FResRC,"wt");
    if(!fR)
    {
        return 2;
    }
    fprintf(fR,"#include \"%s\"\n",FResH);
    fprintf(fR,"#include <%s\\edit.rh>\n",UserSetup.OwlPath);
    fprintf(fR,"#include <%s\\docview.rh>\n",UserSetup.OwlPath);
    fprintf(fR,"#include <%s\\mdi.rh>\n",UserSetup.OwlPath);
    fprintf(fR,"\n");
    if(Flags & NEWAPPFLAG_DIALOG)
    {
        fprintf(fR,"DLGTEMPLATE ID_MAINDIALOG\n");
        fprintf(fR,"BEGIN\n");
        fprintf(fR,"    DIALOG \"\", ID_MAINDIALOG, 0, 0, 172, 113, FS_NOMOVEWITHOWNER | NOT FS_DLGBORDER | NOT WS_SAVEBITS | WS_VISIBLE | WS_CLIPSIBLINGS, FCF_NOBYTEALIGN\n");
        fprintf(fR,"    BEGIN\n");
        fprintf(fR,"        DEFPUSHBUTTON \"Close\", DID_OK, 6, 4, 51, 14\n");
        fprintf(fR,"        PUSHBUTTON \"~About\", CM_ABOUT, 60, 4, 51, 14\n");
        fprintf(fR,"        PUSHBUTTON \"~Help\", CM_HELP, 114, 4, 51, 14, BS_HELP\n");
        fprintf(fR,"    END\n");
        fprintf(fR,"END\n\n");
    }

    fprintf(fR,"DLGTEMPLATE ID_ABOUTDIALOG\n");
    fprintf(fR,"BEGIN\n");
    fprintf(fR,"    DIALOG \"About This Application\", ID_ABOUTDIALOG, 50, 79, 163, 67, WS_VISIBLE, FCF_SYSMENU | FCF_TITLEBAR\n");
    fprintf(fR,"    BEGIN\n");
    fprintf(fR,"        DEFPUSHBUTTON \"OK\", DID_OK, 104, 4, 51, 14\n");
    fprintf(fR,"        CONTROL \"Use Resource Workshop To\\n");
    fprintf(fR,"Put Your Product Name\\n");
    fprintf(fR,"And Version Number Here\", 101, 8, 28, 144, 32, WC_STATIC, SS_TEXT | DT_CENTER | DT_TOP | DT_WORDBREAK | DT_MNEMONIC | WS_VISIBLE\n");
    fprintf(fR,"    END\n");
    fprintf(fR,"END\n\n");

    if(Flags & NEWAPPFLAG_MENU)
    {
        fprintf(fR,"MENU ID_MAINMENU\n");
        fprintf(fR,"BEGIN\n");
        fprintf(fR,"    SUBMENU \"~File\", 100\n");
        fprintf(fR,"    BEGIN\n");
        fprintf(fR,"        MENUITEM \"~New...\", CM_FILENEW\n");
        fprintf(fR,"        MENUITEM \"~Open...\", CM_FILEOPEN\n");
        fprintf(fR,"        MENUITEM SEPARATOR\n");
        fprintf(fR,"        MENUITEM \"~Save...\", CM_FILESAVE\n");
        fprintf(fR,"        MENUITEM \"Save ~As...\", CM_FILESAVEAS\n");
        fprintf(fR,"        MENUITEM SEPARATOR\n");
        fprintf(fR,"        MENUITEM \"~Exit...\", CM_EXIT\n");
        fprintf(fR,"    END\n");
        fprintf(fR,"\n");
        fprintf(fR,"    SUBMENU \"~Edit\", 200\n");
        fprintf(fR,"    BEGIN\n");
        fprintf(fR,"        MENUITEM \"Cu~t\\tShift+Del\", CM_EDITCUT\n");
        fprintf(fR,"        MENUITEM \"~Copy\\tCtrl+Ins\", CM_EDITCOPY\n");
        fprintf(fR,"        MENUITEM \"~Paste\\tShift+Ins\", CM_EDITPASTE\n");
        fprintf(fR,"        MENUITEM \"C~lear\\tDel\", CM_EDITDELETE\n");
        fprintf(fR,"    END\n");
        fprintf(fR,"\n");
        fprintf(fR,"    SUBMENU \"~Help\", 300\n");
        fprintf(fR,"    BEGIN\n");
        fprintf(fR,"        MENUITEM \"~General help...\", CM_HELP\n");
        fprintf(fR,"        MENUITEM \"Help ~index...\", CM_HELPINDEX\n");
        fprintf(fR,"        MENUITEM SEPARATOR\n");
        fprintf(fR,"        MENUITEM \"~About...\", CM_ABOUT\n");
        fprintf(fR,"    END\n");
        fprintf(fR,"END\n");
        fprintf(fR,"\n");
    }


    fprintf(fR,"\n");
    fprintf(fR,"POINTER ID_MAINICON\n");
    fprintf(fR,"BEGIN\n");
    extern unsigned char MyIconData[];
    WriteHexBlock(fR,64*16+2,MyIconData);
    fprintf(fR,"END\n\n");

    //
    // CM_ABOUT ICON
    //
    if(Flags & NEWAPPFLAG_CONTROLBAR)
    {
        fprintf(fR,"\n");
        fprintf(fR,"BITMAP BM_ABOUT\n");
        fprintf(fR,"BEGIN\n");
        extern unsigned char CmAboutData[];
        WriteHexBlock(fR,(42*16)-4,CmAboutData);
        fprintf(fR,"END\n\n");
    }
    fclose(fR);

    if(Flags & NEWAPPFLAG_DIALOG)
    {
        FILE *fc = fopen(FMainCPP,"wt");
        if(!fc)
            return 3;
        fprintf(fc,"#define  INCL_WINMESSAGEMGR\n");
        fprintf(fc,"#include <os2.h>\n");
        fprintf(fc,"#include <%s\\dialog.h>\n",UserSetup.OwlPath);
        fprintf(fc,"#include \"%s\"\n",FAboutH);
        fclose(fc);

        retCode = CreateClassTDialog(FMainH, FMainCPP,  "MainDlg",
                                        "ID_MAINDIALOG",FResH);

        char cd1[256+1];
        // When Dialog is destroyed - take parent with it !
        Cproto X;
        sprintf(cd1,"%sWinPostMsg(GetParent(), WM_CLOSE, 0, 0);",UserSetup.Tabs);
        X.WriteCode(FMainCPP,"MainDlg","EvClickOk",cd1,FALSE);
        //
        X.WriteCode(FMainCPP,"MainDlg","EvClickCancel",cd1,FALSE);
        X.AddEvent(FMainCPP,"MainDlg","EV_BN_CLICKED","CM_ABOUT","EvClickAbout");
        sprintf(cd1,"%sAboutDlg MyAbout(this);\n%sMyAbout.Execute();\n",UserSetup.Tabs,UserSetup.Tabs);
        X.AddFunction(FMainH,FMainCPP, "MainDlg", "protected",
            "","void","EvClickAbout","void","Clicked On About Button/Menu",cd1);
    }

    if(Flags & NEWAPPFLAG_SDI)
    {
        retCode = CreateClassTWindow(FMainH, FMainCPP,
                                        "MainWin",FResH);
        Cproto X;
        X.AddEvent(FMainCPP,"MainWin","EV_COMMAND","CM_ABOUT","EvClickAbout");
        char cd1[256+1];
        sprintf(cd1,"%sAboutDlg MyAbout(this);\n%sMyAbout.Execute();\n",UserSetup.Tabs,UserSetup.Tabs);
        X.AddFunction(FMainH,FMainCPP, "MainWin", "protected",
            "","void","EvClickAbout","void","Clicked On About Button/Menu",cd1);
        X.AppendIncludeFile(FMainCPP,FAboutH);
        GetHeaderForClass(cd1,"TDialog");
        X.AppendIncludeFile(FMainCPP,cd1);
    }

    retCode = CreateClassTDialog(FAboutH,FAboutCPP,"AboutDlg","ID_ABOUTDIALOG",FResH);

    char *ListOfFiles[4] = {    FApp, FAboutCPP, FMainCPP, NULL };
    char makename[MAXPATHSIZE+1];
    strcpy(makename,FApp);
    ForceExt(makename,"");
    CreateMakeFile(makename,FResRC,ListOfFiles,".","/wdpl /Toe /aa");
    ///
    char msg[128+1];
    sprintf(msg," ( make -f%s.mak )\n\n"
                "Would You Like To Start\n"
                "The Compiler and Linker\n"
                "To Build %s.EXE Now ?",makename,makename);
    if( Win->MessageBox(msg,"BUILD APPLICATION",MB_YESNOno) == IDYES)
    {
        sprintf(msg,"-f%s.MAK",makename);
        StartApp(0, "Professor Owl Make",".\\","MAKE.EXE",msg);
    }
    return(retCode);
}


////////////////////////////
/// GENERATE MAKE FILE !
////////////////////////////
int CreateMakeFile(char *makename, char *ResourceFile, char* ListOfFiles[], char *objdir, char *LinkSwitches)
{
    int fObjPath=FALSE;
    if(strcmp(".",objdir) != 0)
        fObjPath = TRUE;

    char FMake[MAXPATHSIZE];
    strcpy(FMake,makename);
    //
    ForceExt(FMake,".MAK");
    FILE *fMak = fopen(FMake,"wt");
    if(!fMak)
    {
        return 1;
    }

    //
    char FCfg[MAXPATHSIZE];
    strcpy(FCfg,FMake);
    ForceExt(FCfg,".CFG");

    char *commentdash = "#=============================================================\n";
    char *commentline = "#\n";
    char buf[128+1];
    fprintf(fMak,commentdash);
    fprintf(fMak,commentline);
    fprintf(fMak,"#   %s  MakeFile Generated by %s\n",FMake,PROFESSOR_OWL_VERSION);
    ExpandDollar("$TIME",buf,"");
    fprintf(fMak,"#       Created on %s\n",buf);
    fprintf(fMak,commentline);
    fprintf(fMak,"%s\n",commentdash);
    fprintf(fMak,".AUTODEPEND\n\n");

    if(fObjPath)
    {
        fprintf(fMak,"# OBJ Files Are Located In Another Directory\n");
        fprintf(fMak,"OBJ_DIR = %s\n",objdir);
        fprintf(fMak,".PATH.obj = $(OBJ_DIR)\n\n");
    }

    fprintf(fMak,commentdash);
    fprintf(fMak,"#       Translator Definitions\n");
    fprintf(fMak,commentdash);
    fprintf(fMak,"CC = bcc +%s\n",FCfg);
    fprintf(fMak,"TASM = tasm\n");
    fprintf(fMak,"TLIB = tlib\n");
    fprintf(fMak,"TLINK = tlink\n");
    fprintf(fMak,"RC = brcc.exe\n");
    char LibPath[128+1];
    strcpy(LibPath,UserSetup.IncludePath);
    strupr(LibPath);
    char *p = strstr(LibPath,"\\INCLUDE");
    if(p)
    {
        strcpy(p,"\\LIB");
    }
    else
    {
        strcpy(LibPath,"C:\\BCOS2\\LIB");
    }
    fprintf(fMak,"LIBPATH = %s\n",LibPath);
    fprintf(fMak,"INCLUDEPATH = %s\n\n\n",UserSetup.IncludePath);
    fprintf(fMak,commentdash);
    fprintf(fMak,"#       Implicit Rules\n");
    fprintf(fMak,commentdash);
    fprintf(fMak,".c.obj:\n");
    fprintf(fMak,"  $(CC) -c {$< }\n");
    fprintf(fMak,"\n");
    fprintf(fMak,".cpp.obj:\n");
    fprintf(fMak,"  $(CC) -c {$< }\n");
    fprintf(fMak,"\n");
    fprintf(fMak,".asm.obj:\n");
    fprintf(fMak,"  $(TASM) -Mx $*.asm,$*.obj\n");
    fprintf(fMak,"\n");
    fprintf(fMak,".rc.res:\n");
    fprintf(fMak,"  $(RC) -r $*.rc\n");
    fprintf(fMak,"\n");
    fprintf(fMak,commentdash);
    fprintf(fMak,"#       List Macros\n");
    fprintf(fMak,commentdash);
    /////////////////////////////
    ForceExt(ResourceFile,"");
    fprintf(fMak,"LINK_EXCLUDE = \\\n %s.RES\n\n",ResourceFile);
    fprintf(fMak,"LINK_INCLUDE = \\\n");
    //
    for(int index=0; ListOfFiles[index]!= NULL; index++)
    {
        char ss[MAXPATHSIZE];
        strcpy(ss,ListOfFiles[index]);
        ForceExt(ss,".OBJ");
        fprintf(fMak," %s",ss,objdir);
        if(ListOfFiles[index+1] != NULL)
            fprintf(fMak," \\\n");
    }
    fprintf(fMak,"\n\n");
    /////////////////////////////
    fprintf(fMak,commentdash);
    fprintf(fMak,"#       Explicit Rules\n");
    fprintf(fMak,commentdash);
    ForceExt(FMake,"");
    fprintf(fMak,"%s.exe: %s.cfg $(LINK_INCLUDE) $(LINK_EXCLUDE)\n",FMake,FMake);
    fprintf(fMak,"  $(TLINK) %s /L$(LIBPATH) @&&|\n",LinkSwitches);
    fprintf(fMak,"%s\\C02.OBJ+\n",LibPath);
    //
    for(index=0; ListOfFiles[index]!= NULL; index++)
    {
        char s[MAXPATHSIZE];
        strcpy(s,ListOfFiles[index]);
        ForceExt(s,".OBJ");

        if(fObjPath)
        {
            char sp[MAXPATHSIZE];
            char sf[MAXPATHSIZE];
            PathSplit(sp,sf,s);
            fprintf(fMak,"$(OBJ_DIR)\\%s",sf); // macro path & file name
        }
        else
        {
            fprintf(fMak,"%s",s);       // path/filename.obj
        }
        if(ListOfFiles[index+1] != NULL)
            fprintf(fMak,"+\n");
        else
            fprintf(fMak,"\n");
    }
    fprintf(fMak,"%s,%s\n",FMake,FMake);
    fprintf(fMak,"$(LIBPATH)\\BIDS2.LIB+\n");
    fprintf(fMak,"$(LIBPATH)\\OWLPM.LIB+\n");
    fprintf(fMak,"$(LIBPATH)\\C2MT.LIB+\n");
    fprintf(fMak,"$(LIBPATH)\\OS2.LIB\n");
    fprintf(fMak,"\n");
    fprintf(fMak,"|\n");
    fprintf(fMak,"  RC.EXE %s.RES %s.exe\n\n",ResourceFile,FMake);
    /////////////////////////////
    fprintf(fMak,commentdash);
    fprintf(fMak,"#       Individual File Dependencies\n");
    fprintf(fMak,commentdash);
    fprintf(fMak,"%s.RES: %s.cfg %s.RC\n",ResourceFile,FMake,ResourceFile);
    fprintf(fMak,"    $(RC) -R -I$(INCLUDEPATH) -FO %s.RES %s.RC\n\n",ResourceFile,ResourceFile);
    //
    //
    for(index=0; ListOfFiles[index]!= NULL; index++)
    {
        char s[MAXPATHSIZE];
        strcpy(s,ListOfFiles[index]);
        ForceExt(s,"");
        fprintf(fMak,"%s.obj: %s.cfg %s.cpp\n\n",s,FMake,s);
    }
    /////////////////////////////
    fprintf(fMak,commentdash);
    fprintf(fMak,"#       Compiler Configuration File\n");
    fprintf(fMak,commentdash);
    fprintf(fMak,"%s.cfg: %s.mak\n",FMake,FMake);
    fprintf(fMak,"  copy &&|\n");
    fprintf(fMak,"-R\n");
    fprintf(fMak,"-L$(LIBPATH)\n");
    fprintf(fMak,"-I$(INCLUDEPATH)\n");
    ExpandDollar("$PATH",buf,"");
    fprintf(fMak,"-H=%s\\%s.CSM\n",buf,FMake);
    fprintf(fMak,"-vi-\n");
    fprintf(fMak,"-sm\n");
    fprintf(fMak,"-y\n");
    fprintf(fMak,"-v\n");
    fprintf(fMak,"-N\n");
    if(fObjPath)
    {
        fprintf(fMak,"-n$(OBJ_DIR)\n");
    }
    fprintf(fMak,"| %s.CFG\n",FMake);
    fprintf(fMak,commentdash);
    fclose(fMak);
    ///
    return 0;
}






//
// Big Message handler of strings -- Taken from BCOS2's windowev.h
//
#include    "def.h"
#include    <owl\listbox.h>
#include    "bigmess.h"
#include    "stdio.h"
#include    "parse.h"
#include    "setupdat.h"
#include    "vfun.h"

extern SETUPDATA UserSetup;

#if 0
 "EV_MESSAGE",      "method",
 "EV_CHILD_NOTIFY",     "??",
 "EV_CHILD_NOTIFY_AND_CODE",        "??",
 "EV_CHILD_NOTIFY_ALL_CODES",       "??",
 "EV_NOTIFY_AT_CHILD",      "method",
 "EV_COMMAND_AND_ID",       "??",
 "EV_COMMAND_ENABLE",       "??",
 "EV_OWLDOCUMENT",      "??",
 "EV_OWLVIEW",      "??",
 "EV_OWLNOTIFY",        "??",
#endif



struct BigMess2 MessEV_WM[] =
{
 {"EV_WM_ACTIVATE",         "void EvActivate(UINT active, BOOL minimized, HWND hWndOthers)"},
 {"EV_WM_ACTIVATEAPP",      "void EvActivateApp(BOOL active, HTASK threadId)"},
 {"EV_WM_ASKCBFORMATNAME",  "void EvAskCBFormatName(UINT bufLen, char far *buffer)"},
 {"EV_WM_CANCELMODE",       "void EvCancelMode(void)"},
 {"EV_WM_CHANGECBCHAIN",    "void EvChangeCBChain(UINT bufLen, char far *buffer)"},
 {"EV_WM_CHAR",             "void EvChar(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_CHARTOITEM",       "void EvCharToItem(UINT key, HWND hWndListBox, UINT caretPos)"},
 {"EV_WM_CHILDACTIVATE",    "void EvChildActivate(void)"},
 {"EV_WM_CHILDINVALID",     "void EvChildInvalid(HWND hWnd)"},
 {"EV_WM_CLOSE",            "void EvClose(void)"},
 {"EV_WM_COMMNOTIFY",       "void EvCommNotify(UINT commId, UINT status)"},
 {"EV_WM_COMPACTING",       "void EvCompacting(UINT compactRatio)"},
 {"EV_WM_COMPAREITEM",      "void EvCompareItem(UINT ctrlId, COMPAREITEMSTRUCT far& compareInfo)"},
 {"EV_WM_CREATE",           "void EvCreate(CREATESTRUCT far& createStruct)"},
 {"EV_WM_CTLCOLOR",         "void EvCtlColor(HDC hDC, HWND hWndChild, UINT ctlType)"},
 {"EV_WM_DEADCHAR",         "void EvDeadChar(UINT deadKey, UINT repeatCount, UINT flags)"},
 {"EV_WM_DELETEITEM",       "void EvDeleteItem(UINT ctrlId, DELETEITEMSTRUCT far& deleteInfo)"},
 {"EV_WM_DESTROY",          "void EvDestroy(void)"},
 {"EV_WM_DESTROYCLIPBOARD", "void EvDestroyClipboard(void)"},
 {"EV_WM_DEVMODECHANGE",    "void EvDevModeChange(char far* devName)"},
 {"EV_WM_DRAWCLIPBOARD",    "void EvDrawClipboard(void)"},
 {"EV_WM_DRAWITEM",         "void EvDrawItem(UINT ctrlId, DRAWITEMSTRUCT far& drawInfo)"},
 {"EV_WM_DROPFILES",        "void EvDropFiles(TDropInfo dropInfo)"},
 {"EV_WM_ENABLE",           "void EvEnable(BOOL enabled)"},
 {"EV_WM_ENDSESSION",       "void EvEndSession(BOOL endSession)"},
 {"EV_WM_ENTERIDLE",        "void EvEnterIdle(UINT source, HWND hWndDlg)"},
 {"EV_WM_ERASEBKGND",       "void EvEraseBkgnd(HDC)"},
 {"EV_WM_FONTCHANGE",       "void EvFontChange(void)"},
 {"EV_WM_GETDLGCODE",       "void EvGetDlgCode(void)"},
 {"EV_WM_GETMINMAXINFO",    "void EvGetMinMaxInfo(MINMAXINFO far& minmaxinfo)"},
 {"EV_WM_GETTEXT",          "void EvGetText(void)"},
 {"EV_WM_GETTEXTLENGTH",    "void EvGetTextLength(void)"},
 {"EV_WM_HOTKEY",           "void EvHotKey(int idHotKey)"},
 {"EV_WM_HSCROLL",          "void EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)"},
 {"EV_WM_HSCROLLCLIPBOARD", "void EvHScrollClipboard(HWND hWndCBViewer, UINT scrollCode, UINT pos)"},
 {"EV_WM_ICONERASEBKGND",   "void EvIconEraseBkgnd(HDC)"},
 {"EV_WM_INITDIALOG",       "void EvInitDialog(void)"},
 {"EV_WM_INITMENU",         "void EvInitMenu(HMENU hMenu)"},
 {"EV_WM_INITMENUPOPUP",    "void EvInitMenuPopup(HMENU hPopupMenu, UINT  index, BOOL  sysMenu)"},
 {"EV_WM_KEYDOWN",          "void EvKeyDown(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_KEYUP",            "void EvKeyUp(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_KILLFOCUS",        "void EvKillFocus(HWND hWndGetFocus)"},
 {"EV_WM_LBUTTONDBLCLK",    "void EvLButtonDblClk(UINT modKeys, TPoint& point)"},
 {"EV_WM_LBUTTONDOWN",      "void EvLButtonDown(UINT modKeys, TPoint& point)"},
 {"EV_WM_LBUTTONUP",        "void EvLButtonUp(UINT modKeys, TPoint& point)"},
 {"EV_WM_MBUTTONDBLCLK",    "void EvMButtonDblClk(UINT modKeys, TPoint& point)"},
 {"EV_WM_MBUTTONDOWN",      "void EvMButtonDown(UINT modKeys, TPoint& point)"},
 {"EV_WM_MBUTTONUP",        "void EvMButtonUp(UINT modKeys, TPoint& point)"},
 {"EV_WM_MDIACTIVATE",      "void EvMDIActivate(HWND hWndActivated, HWND hWndDeactivated)"},
 {"EV_WM_MDICREATE",        "void EvMDICreate(MDICREATESTRUCT far& createStruct)"},
 {"EV_WM_MDIDESTROY",       "void EvMDIDestroy(HWND hWnd)"},
 {"EV_WM_MEASUREITEM",      "void EvMeasureItem(UINT ctrlId, MEASUREITEMSTRUCT far& measureInfo)"},
 {"EV_WM_MEASUREITEM",      "void EvMeasureItem(UINT ctrlId, MEASUREITEMSTRUCT far& measureInfo)"},
 {"EV_WM_MENUCHAR",         "void EvMenuChar(UINT nChar, UINT menuType, HMENU hMenu)"},
 {"EV_WM_MENUSELECT",       "void EvMenuSelect(UINT menuItemId, UINT flags, HMENU hMenu)"},
 {"EV_WM_MENUSELECT",       "void EvMenuSelect(UINT menuItemId, UINT flags, HMENU hMenu)"},
 {"EV_WM_MOUSEACTIVATE",    "void EvMouseActivate(HWND hWn, UINT hit, UINT msg)"},
 {"EV_WM_MOUSEMOVE",        "void EvMouseMove(UINT modKeys, TPoint& point)"},
 {"EV_WM_MOVE",             "void EvMove(TPoint& clientOrigin)"},
 {"EV_WM_NCACTIVATE",       "void EvNCActivate(BOOL active)"},
 {"EV_WM_NCCALCSIZE",       "void EvNCCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS far& params)"},
 {"EV_WM_NCCREATE",         "void EvNCCreate(CREATESTRUCT far& createStruct)"},
 {"EV_WM_NCDESTROY",        "void EvNCDestroy(void)"},
 {"EV_WM_NCHITTEST",        "void EvNCHitTest(BOOL activevoid)"},
 {"EV_WM_NCLBUTTONDBLCLK",  "void EvNCLButtonDblClk(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCLBUTTONDOWN",    "void EvNCLButtonDown(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCLBUTTONUP",      "void EvNCLButtonUp(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCMBUTTONDBLCLK",  "void EvNCMButtonDblClk(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCMBUTTONDOWN",    "void EvNCMButtonDown(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCMBUTTONUP",      "void EvNCMButtonUp(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCMOUSEMOVE",      "void EvNCMouseMove(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCPAINT",          "void EvNCPaint(void)"},
 {"EV_WM_NCRBUTTONDBLCLK",  "void EvNCRButtonDblClk(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCRBUTTONDOWN",    "void EvNCRButtonDown(UINT hitTest, TPoint& point)"},
 {"EV_WM_NCRBUTTONUP",      "void EvNCRButtonUp(UINT hitTest, TPoint& point)"},
 {"EV_WM_NEXTDLGCTL",       "void EvNextDlgCtl(UINT hctlOrDir, UINT isHCtl)"},
 {"EV_WM_PAINT",            "void EvPaint(void)"},
 {"EV_WM_PAINTCLIPBOARD",   "void EvPaintClipboard(HWND hWnd, HANDLE hPaintStruct)"},
 {"EV_WM_PAINTICON",        "void EvPaintIcon(void)"},
 {"EV_WM_PALETTECHANGED",   "void EvPaletteChanged(HWND hWndPalChg)"},
 {"EV_WM_PALETTEISCHANGING","void EvPaletteIsChanging(HWND hWndPalChg)"},
 {"EV_WM_PARENTNOTIFY",     "void EvParentNotify(UINT event, UINT childHandle)"},
 {"EV_WM_POWER",            "void EvPower(UINT powerEvent)"},
 {"EV_WM_QUERYDRAGICON",    "void EvQueryDragIcon(void)"},
 {"EV_WM_QUERYENDSESSION",  "void EvQueryEndSession(void)"},
 {"EV_WM_QUERYNEWPALETTE",  "void EvQueryNewPalette(void)"},
 {"EV_WM_QUERYOPEN",        "void EvQueryOpen(void)"},
 {"EV_WM_QUEUESYNC",        "void EvQueueSync(void)"},
 {"EV_WM_RBUTTONDBLCLK",    "void EvRButtonDblClk(UINT modKeys, TPoint& point)"},
 {"EV_WM_RBUTTONDOWN",      "void EvRButtonDown(UINT modKeys, TPoint& point)"},
 {"EV_WM_RBUTTONUP",        "void EvRButtonUp(UINT modKeys, TPoint& point)"},
 {"EV_WM_RENDERALLFORMATS", "void EvRenderAllFormats(void)"},
 {"EV_WM_RENDERFORMAT",     "void EvRenderFormat(UINT dataFormat)"},
 {"EV_WM_SETCURSOR",        "void EvSetCursor(HWND hWndCurs, UINT hitTest, UINT mouseMsgid)"},
 {"EV_WM_SETFOCUS",         "void EvSetFocus(HWND hWndLostFocus)"},
 {"EV_WM_SETFONT",          "void EvSetFont(HFONT hFont, BOOL redraw)"},
 {"EV_WM_SETREDRAW",        "void EvSetRedraw(BOOL redraw)"},
 {"EV_WM_SETTEXT",          "void EvSetText(const char far* text)"},
 {"EV_WM_SHOWWINDOW",       "void EvShowWindow(BOOL show, UINT status)"},
 {"EV_WM_SIZE",             "void EvSize(UINT sizeType, TSize& size)"},
 {"EV_WM_SIZECLIPBOARD",    "void EvSizeClipboard(HWND hWndViewer, HANDLE hRect)"},
 {"EV_WM_SPOOLERSTATUS",    "void EvSpoolerStatus(UINT jobStatus, UINT jobsLeft)"},
 {"EV_WM_SYSCHAR",          "void EvSysChar(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_SYSCOLORCHANGE",   "void EvSysColorChange(void)"},
 {"EV_WM_SYSCOMMAND",       "void EvSysCommand(UINT cmdType, TPoint& point)"},
 {"EV_WM_SYSDEADCHAR",      "void EvSysDeadChar(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_SYSKEYDOWN",       "void EvSysKeyDown(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_SYSKEYUP",         "void EvSysKeyUp(UINT key, UINT repeatCount, UINT flags)"},
 {"EV_WM_SYSTEMERROR",      "void EvSystemError(UINT error)"},
 {"EV_WM_TIMECHANGE",       "void EvTimeChange(void)"},
 {"EV_WM_TIMER",            "void EvTimer(UINT timerId)"},
 {"EV_WM_VKEYTOITEM",       "void EvVKeyToItem(UINT key, HWND hWndListBox, UINT caretPos)"},
 {"EV_WM_VSCROLL",          "void EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)"},
 {"EV_WM_VSCROLLCLIPBOARD", "void EvVScrollClipboard(HWND hWnd, UINT scr, UINT pos)"},
 {"EV_WM_WINDOWPOSCHANGED", "void EvWindowPosChanged(WINDOWPOS far& windowPos)"},
 {"EV_WM_WINDOWPOSCHANGING","void EvWindowPosChanging(WINDOWPOS far& windowPos)"},
 {"EV_WM_WININICHANGE",     "void EvWinIniChange(char far* section)"},
 {   NULL,NULL}
}; //sorted! 1.03a


BigMess2 MessEV_BN[] =
{
    {"EV_BN_CLICKED",       "void ?EvClick(void)"},
    {"EV_BN_DISABLE",       "void ?EvDisable(void)"},
    {"EV_BN_DOUBLECLICKED", "void ?EvDblClick(void)"},
    {"EV_BN_HILITE",        "void ?EvHilite(void)"},
    {"EV_BN_PAINT",         "void ?EvPaint(void)"},
    {"EV_BN_UNHILITE",      "void ?EvUnHilite(void)"},
    {NULL,NULL}
}; //sorted


BigMess2 MessEV_CBN[] =
{
    {"EV_CBN_CLOSEUP",      "void ?EvCloseUp(void)"},
    {"EV_CBN_DBLCLK",       "void ?EvDblClick(void)"},
    {"EV_CBN_DROPDOWN",     "void ?EvDropDown(void)"},
    {"EV_CBN_EDITCHANGE",   "void ?EvEditChange(void)"},
    {"EV_CBN_EDITUPDATE",   "void ?EvEditUpdate(void)"},
    {"EV_CBN_ERRSPACE",     "void ?EvErrSpace(void)"},
    {"EV_CBN_KILLFOCUS",    "void ?EvKillFocus(void)"},
    {"EV_CBN_SELCHANGE",    "void ?EvSelChange(void)"},
    {"EV_CBN_SELENDCANCEL", "void ?EvSelEndCancel(void)"},
    {"EV_CBN_SELENDOK",     "void ?EvSelEndOK(void)"},
    {"EV_CBN_SETFOCUS",     "void ?EvSetFocus(void)"},
    {NULL,NULL}
};//sorted

BigMess2 MessEV_LBN[] =
{
    {"EV_LBN_DBLCLK",       "void ?EvDblClick(void)"},
    {"EV_LBN_ERRSPACE",     "void ?EvErrSpace(void)"},
    {"EV_LBN_KILLFOCUS",    "void ?EvKillFocus(void)"},
    {"EV_LBN_SELCHANGE",    "void ?EvSelChange(void)"},
    {"EV_LBN_SELENDOK",     "void ?EvSelEndOK(void)"},
    {"EV_LBN_SETFOCUS",     "void ?EvSetFocus(void)"},
    {NULL,NULL}
};//sorted

BigMess2 MessEV_EN[] =
{
    {"EV_EN_CHANGE",        "void ?EvChange(void)"    },
    {"EV_EN_ERRSPACE",      "void ?EvErrSpace(void)"   },
    {"EV_EN_HSCROLL",       "void ?EvHScroll(void)"    },
    {"EV_EN_KILLFOCUS",     "void ?EvKillFocus(void)"  },
    {"EV_EN_MAXTEXT",       "void ?EvMaxText(void)"    },
    {"EV_EN_SETFOCUS",      "void ?EvSetFocus(void)"   },
    {"EV_EN_UPDATE",        "void ?EvUpdate(void)"     },
    {"EV_EN_VSCROLL",       "void ?EvVScroll(void)"    },
    {NULL,NULL}
};//sorted

BigMess2 MessEV_SB[] =
{
    {"EV_SB_BEGINTRACK",    "void ?EvBeginTrack(void)"},
    {"EV_SB_BOTTOM",        "void ?EvBottom(void)"},
    {"EV_SB_ENDSCROLL",     "void ?EvEndScroll(void)"},
    {"EV_SB_LINEDOWN",      "void ?EvLineDown(void)"},
    {"EV_SB_LINEUP",        "void ?EvLineUp(void)"},
    {"EV_SB_PAGEDOWN",      "void ?EvPageDown(void)"},
    {"EV_SB_PAGEUP",        "void ?EvPageUp(void)"},
    {"EV_SB_THUMBPOSITION", "void ?EvThumbPosition(void)"},
    {"EV_SB_THUMBTRACK",    "void ?EvThumbTrack(void)"},
    {"EV_SB_TOP",           "void ?EvStop(void)"},
    {NULL,NULL}
};//sorted

BigMess2 MessEV_COMMAND[] =
{
    {"EV_COMMAND",          "void ?EvCm(void)"},
    {"EV_COMMAND_ENABLE",   "void ?EvCe(TCommandEnabler &CommandEnabler)"},
    {NULL,NULL}
};


BigTypeMess ClassTypeMess[] =
{
    {   "TWindow",      MessEV_WM   },
    //
    {   "PUSHBUTTON",   MessEV_BN   },
    {   "WC_BUTTON",    MessEV_BN   },
    //
    {   "TComboBox",    MessEV_CBN  },
    {   "WC_COMBOBOX",  MessEV_CBN  },
    //
    {   "TListBox",     MessEV_CBN  },
    {   "WC_LISTBOX",   MessEV_LBN  },
    //
    {   "TEdit",        MessEV_EN  },
    {   "WC_ENTRYFIELD",MessEV_EN  },
    {   "WC_MLE",       MessEV_EN  },
    //
    {   "TStatic",      MessEV_EN  },
    {   "WC_STATIC",    MessEV_EN  },
    //
    {   "TScrollBar",   MessEV_SB },
    {   "TSlider",      MessEV_SB },
    {   "THSlider",     MessEV_SB },
    {   "TVSlider",     MessEV_SB },
    {   "WC_SLIDER",    MessEV_SB },
    {   "EV_COMMAND",   MessEV_COMMAND },
    //
    { NULL,NULL }
};


//
// Add Event name and handler name in list box
//
void AddEventTypeMess(char *Class, TListBox *pLB)
{
    BigTypeMess *p = ClassTypeMess;
    while( p->Class )
    {
        if(strcmp(Class,p->Class) == 0)
        {
            char s[128+1];
            BigMess2 *bm = p->BigMessPtr;
            while(bm->Event)
            {
                sprintf(s,"%-18s %s",bm->Event,bm->Func);
                pLB->AddString(s);
                bm++;
            }
            if( bm != MessEV_WM )
            {
                while(bm->Event)
                {
                    sprintf(s,"%-18s %s",bm->Event,bm->Func);
                    pLB->AddString(s);
                    bm++;
                }
            }
            pLB->SetSelIndex(0);
            return;
        }
        p++;
    }
    // otherwise a class derived from TWindow get EV_WM_XXXXX
    if( IsClassFromTWindow(Class) )
    {
        AddEventTypeMess("TWindow",pLB);
    }
}



//
// Look Up EV_WM_name and return return_type, FunctionName, Arguments
//
//
void LookUpEV_WM(char *ev, char *rRet, char *rFunc, char *rArg)
{
    if(rRet)
        *rRet = NULL;
    if(rFunc)
        *rFunc = NULL;
    if(rArg)
        *rArg = NULL;

    BigMess2 *p = MessEV_WM;
    while( p->Event && strcmp(p->Event,ev) != 0 )
        p++;

    if(p->Event)    // Found Event Macro Mane
    {
        char s1[128+1];
        char s2[128+1];
        char s3[128+1];
        ParseRetFunArg(p->Func,s1,s2,s3);
        if(rRet)
            strcpy(rRet,s1);
        if(rFunc)
            strcpy(rFunc,s2);
        if(rArg)
            strcpy(rArg,s3);
    }
}

//
// Look Up Function & args and return Event Handler Macro name
//
//
void ReverseLookUpEV_WM(char *Func, char *Arg, char *rEv)
{
    *rEv = NULL;

    char Arg1[MAXPATHSIZE];
    strcpy(Arg1,Arg);
    onespace(Arg,Arg1);

    BigMess2 *p = MessEV_WM;
    while( p->Event )
    {
        char s31[128+1];
        char s2[128+1];
        char s3[128+1];
        memset(s3,NULL,sizeof(s3));
        sscanf(p->Func,"%s %s %80c",s31,s2,s3); // s31 is trashed unused

        onespace(s3,s31);   // one spaced arg list in case table is weird
        if(strcmp(Func, s2) == 0 && strcmp(Arg1,s31) == 0)
        {
            strcpy(rEv,p->Event);   // Event Macro found!
            return;
        }
        p++;
    }
}


//
// Given a class name return the owl header that must be included
//
void GetHeaderForClass(char *hdr, char *Class)
{
    struct
    {
        char *Class;
        char *Hdr;
    }
    ClassHdrs[] ={  // prefix "\owl\????" and sufix ".h" assumed
        {   "TApplication",     "applicat"      },
        {   "TBitmap",          "gdiobject"     },
        {   "TBitmapGadget",    "bitmapga"      },
        {   "TBrush",           "gdiobject"     },
        {   "TButton",          "button"        },
        {   "TButtonGadget",    "buttonga"      },
        {   "TButtongadgetEnabler",  "buttonga" },
        {   "TCellArray",       "celarray"      },
        {   "TCheckBox",        "checkbox"      },
        {   "TClientDC",        "dc"            },
        {   "TClipboard",       "clipboar"      },
        {   "TColor",           "color"         },
        {   "TComboBox",        "combobox"      },
        {   "TComboBoxData",    "combobox"      },
        {   "TCommandEnabler",  "window"        },
        {   "TControlBar",      "controlb"      },
        {   "TControlGadget",   "controlg"      },
        {   "TControl",         "control"       },
        {   "TCreatedDC",       "dc"            },
        {   "TCursor",          "gdiobject"     },
        {   "TDC",              "dc"            },
        {   "TDesktopDC",       "dc"     },
        {   "TDialog",          "dialog"        },

        {   "TDocManager",      "docmanag"      },
        {   "TDocument",        "docview"       },

        {   "TEdit",            "edit"          },
        {   "TEditFile",        "editfile"      },
        {   "TEditSearch",      "editsear"      },
        {   "TEditView",        "editview"      },
        {   "TFileDocument",    "filedoc"       },
        {   "TFrameWidnow",     "framewin"      },
        {   "TGadget",          "gadget"        },
        {   "TGadgetWindow",    "gadgetwi"      },
        {   "TGadgetWindowFont","gadgetwi"      },
        {   "TGauge",           "gauge"         },
        {   "TGdiObject",       "gdiobjec"      },
        {   "TGroupBox",        "groupbox"      },
        {   "TIcon",            "gdiobjec"      },
        {   "TLayoutWindow",    "layoutwi"      },

        {   "TListBox",         "listbox"       },
        {   "TListBoxData",     "listbox"       },
        {   "TListView",        "listview"      },
        {   "TLookupValidator", "validate"      },

        {   "TMDIChild",        "mdichild"      },
        {   "TMDIClient",       "mdi"           },
        {   "TMDIFrame",        "mdi"           },
        {   "TMeasurementUnits","layoutco"      },
        {   "TMemoryDC",        "dc"            },
        {   "TMenu",            "menu"          },
        {   "TMenuDescr",       "menu"          },
        {   "TMenuEnabler",     "framewin"      },
        {   "TMessageBar",      "messageb"      },

        {   "TMetaFileDC",      "dc"            },
        {   "TMetaFilePict",    "metafile"      },

        {   "TModule",          "module"        },
        // !!!Note: Missing a few classes here!

        {   "TRadioButton",     "radiobut"      },
        {   "TScreenDC",        "dc"            },
        {   "TScrollBar",       "scrollba"      },
        {   "TScrollBarData",   "scrollba"      },
        {   "TScroller",        "scroller"      },
        {   "TSeparatorGadget", "gadget"        },
        {   "TSlider",          "slider"        },
        {   "THSlider",         "slider"        },
        {   "TVSlider",         "slider"        },
        {   "TStatic",          "static"        },

        {   "TStatusBar",       "statusba"      },
        {   "TSystemMenu",      "menu"          },
        {   "TTinyCaption",     "tinycapt"      },
        {   "TToolBox",         "toolbox"       },

        {   "TWindow",          "window"        },
        {   "TWindowView",      "docview"       },
        {   "TView",            "docview"       },
        {   "TWindowDC",        "dc"            },

        {   "TValidator",       "validate"      },
        {   "TValidator",       "validate"      },
        NULL,NULL
    };

    for(int i=0; ClassHdrs[i].Class; i++)
    {
        if(strcmp(Class,ClassHdrs[i].Class) == 0)
        {
            sprintf(hdr,"<%s\\%s.h>",UserSetup.OwlPath,ClassHdrs[i].Hdr);
            return;
        }
    }
    sprintf(hdr,"<%s\\owlall.h> // '%s' ",UserSetup.OwlPath,Class);    // Default !!!
}

//
// If this class destructor should
//  get a VIRTUAL desctructor - return TRUE
//
int IsVirtualDestructorNeeded(char *baseclass)
{
    char *vd[] =
    {
        "TCelArray",
        "TDC",
        "TDocManager",
        "TDocument",
        "TGadget",
        "TMenu",
        "TModule",
        "TPrinter",
        "TPrintout",
        "TScroller",
        "TValidator",
        "TView",
        "TWindow",
        "TXOwl",
        NULL
    };

    for(int i=0; vd[i] != NULL; i++)
    {
        if(strcmp(baseclass,vd[i]) == 0)
            return TRUE;
    }
    return FALSE;
}



//
// Given a class name return Constructor c++ parameter string
//
void GetConstructorForClass(char *con, char *MyClass, char *BaseClass)
{
    struct
    {
        char *Class;
        char *Constr;
    }
  Cons[] ={
    {   "TButton",
                    "(TWindow* parent, int id, const char far* text, int X, int Y, int W, int H, BOOL isDefault = 0, TModule* module = 0):"
                    "TButton(parent, id, text, X, Y, W, H, isDefault, module)"  },
    {   "TCheckBox",
                    "(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, TGroupBox* group=0, TModule* module = 0):"
                    "TCheckBox(parent, id, title, x, y, w, h, group, module)"   },
    {   "TComboBox",
                    "(TWindow* parent, int id, int x, int y, int w, int h, DWORD style, UINT textLen, TModule* module = 0):"
                    "TComboBox(parent, id, x, y, w, h, style, textLen, module)" },
    {   "TControl",
                    "(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, TModule* module = 0):"
                    "TControl(parent, id, title, x, y, w, h, module)"   },
    {   "TControlBar",
                    "(TWindow* parent = 0, TTileDirection direction= Horizontal, TFont* font = new TGadgetWindowFont, TModule* module = 0):"
                    "TControlBar(parent, direction, font, module)"  },
    {   "TDocument",
                    "(TDocument* parent=0):  TDocument(parent)"       },
    {   "TEdit",
                "(TWindow* parent, int id, const char far* text, int x, int y, int w, int h, UINT textLen, BOOL multiline=0, TModule* module = 0):"
                "TEdit(parent, id, text, x, y, w, h, textLen, multiline, module)"   },
    {   "TEditFile",
                    "(TWindow* parent = 0, int id = 0, const char far* text = 0, int x = 0, int y = 0, int w = 0, int h = 0, const char far* fileName = 0, TModule* module = 0):"
                    "TEditFile(parent, id, text, x, y, w, h, fileName, module)" },
    {   "TEditSearch",
                    "(TWindow* parent = 0, int id = 0, const char far* text = 0, int x = 0, int y = 0, int w = 0, int h = 0, TModule* module = 0):"
                    "TEditSearch(parent, id, text, x, y, w, h, module)" },
    {   "TEditView",
                    "(TDocument& doc, TWindow* parent=0):TEditView(doc, parent)"  },
    {   "TFileDocument",
                    "(TDocument* parent=0):TFileDocument(parent)" },
    {   "TFloatingFrame",
                    "(TWindow* parent, char* title = 0, TWindow* clientWnd = 0, BOOL shrinkToClient = 0, int captionHeight = DefaultCaptionHeight, BOOL popupPalette = 0, TModule* module = 0):"
                    "TFloatingFrame(parent, title, clientWnd, shrinkToClient, captionHeight, popupPalette, module)" },
    {   "TGadgetWindow",
                    "(TWindow* parent = 0, TTileDirection direction = Horizontal, TFont* font = new TGadgetWindowFont, TModule* module = 0):"
                    "TGadgetWindow(parent, direction, font, module)"    },
    {   "TGauge",
                    "(TWindow* parent, const char far* title, int id, int X, int Y, int W, int H, BOOL isHorizontal = 1, int margin = 0, TModule* module = 0):"
                    "TGauge(parent, title, id, X, Y, W, H, isHorizontal, margin, module)"   },
    {   "TListBox",
                    "(TWindow* parent, int id, int x, int y, int w, int h, TModule* module = 0):"
                    "TListBox(parent, id, x, y, w, h, module)"  },
    {   "TLayoutWindow",
                    "(TWindow* parent, const char far* title = 0, TModule* module = 0):"
                    "TLayoutWindow(parent, title, module)"  },
    {   "TListView",
                    "(TDocument& doc, TWindow* parent = 0):"
                    "TListView(doc, parent)"  },
    {   "TMDIChild",
                    "(TMDIClient& parent, const char far* title = 0, TWindow* clientWnd = 0, BOOL shrinkToClient = FALSE, TModule* module = 0):"
                    "TMDIChild(parent, title, clientWnd, shrinkToClient, module)"    },
    {   "TMessageBar",
                    "(TWindow* parent = 0, TFont* font = new TGadgetWindowFont, TModule* module = 0):"
                    "TMessageBar(parent, font, module)" },
    {   "TRadioButton",
                    "(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, TGroupBox* group = 0, TModule* module = 0):"
                    "TRadioButton(parent, id, title, x, y, w, h, group, module)"    },
    {   "TScrollBar",
                    "(TWindow* parent, int id, int x, int y, int w, int h, BOOL isHScrollBar, TModule* module = 0):"
                    "TScrollBar(parent, id, x, y, w, h, isHScrollBar, module)"  },
    {   "TStatic",
                    "(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, UINT textLen=0, TModule* module = 0):"
                    "TStatic(parent, id, title, x, y, w, h, textLen, module)"   },
    {   "TStatusBar",
                    "(TWindow* parent = 0, TGadget::TBorderStyle borderStyle = TGadget::Recessed, UINT modeIndicators = 0, TFont* font = new TGadgetWindowFont, TModule* module = 0):"
                    "TStatusBar(parent, borderStyle, modeIndicators, font, module)" },
    {   "TTinyCaption",
                    "(void):TTinyCaption(void)" },
    {   "TToolBox",
                    "(TWindow* parent, int numColumns = 2, int numRows = AS_MANY_AS_NEEDED, TTileDirection direction = Horizontal,  TModule* module = 0):"
                    "TToolBox(parent, numColumns, numRows, direction, module)"  },
    {   "TView",
                    "(TDocument& doc):TView(doc)"   },
    {   "TWindowView",
                    "(TDocument& doc, TWindow* parent=0):TWindowView(doc, parent)"    },
    {   NULL,NULL   }
  };

    for(int i=0; Cons[i].Class; i++)
    {
        if(strcmp(BaseClass,Cons[i].Class) == 0)
        {
            sprintf(con,"%s%s",MyClass,Cons[i].Constr);
            return;
        }
    }
    sprintf(con,"s%s",MyClass,"(void)");   // deafult constructor
}



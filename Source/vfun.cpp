//
// Table Of All Virtual Functions for OWL classes
//
#include    <owl\listbox.h>
#include    "vfun.h"
#include    "stdio.h"
#include    "def.h"
//
//

struct VFuncs VfTApplication[] = {
    {   "BOOL",     "CanClose(void)"            },
    {   "LRESULT",  "Dispatch(TEventInfo& info, WPARAM wp, LPARAM lp)"   },
    {   "BOOL",     "Find(TEventInfo&, TEqualOperator)" },
    {   "BOOL",     "IdleAction(long idleCount)"},
    {   "void",     "InitApplication(void)"     },
    {   "void",     "InitInstance(void)"        },
    {   "void",     "InitMainWindow(void)"      },
    {   "int",      "MessageLoop(void)"         },
    {   "void",     "PreProcessMenu(HMENU)"     },
    {   "BOOL",     "ProcessAppMsg(MSG& msg)"   },
    {   "int",      "Run(void)"                 },
    {   "int ",     "TermInstance(int status)"  },
    {    NULL,   NULL   }
};//sorted

struct VFuncs VfTWindow[] = {
    {    "BOOL",     "CanClose(void)"   },
    {    "void",     "CleanupWindow(void)"      },
    {    "void",     "CloseWindow(int retVal)"  },
    {    "BOOL",     "Create(void)" },
    {    "LRESULT",  "DefWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)"    },
    {    "void",     "Destroy(int retVal)"  },
    {    "LRESULT",  "Dispatch(TEventInfo& info, WPARAM wp, LPARAM lp)" },
    {    "void",     "Enable(BOOL enable)"   },
    {    "BOOL",     "EnableUpdate(BOOL enable)"    },
    {    "BOOL",     "EnableWindow(BOOL enable)"    },
    {    "LRESULT",  "EvCommand(UINT id, HWND hWndCtl, UINT notifyCode)"    },
    {    "void",     "EvCommandEnable(TCommandEnabler& ce)" },
    {    "char far*","GetClassName(void)"       },
    {    "void",     "GetWindowClass(WNDCLASS& wndClass)"   },
    {    "BOOL",     "HoldFocusHWnd(HWND hWndLose, HWND hWndGain)"  },
    {    "BOOL",     "IdleAction(long idleCount)"   },
    {    "void",     "Paint(TDC& dc, BOOL erase, TRect& rect)"  },
    {    "void",     "PerformCreate(int menuOrId)"  },
    {    "BOOL",     "PreProcessMsg(MSG& msg)"  },
    {    "BOOL",     "Register(void)"   },
    {    "void",     "RemoveChild(TWindow* child)"  },
    {    "void",     "SetCheck(int check)"  },
    {    "BOOL",     "SetDocTitle(const char far* docname, int index)"  },
    {    "void",     "SetParent(TWindow* newParent)"    },
    {    "void",     "SetText(const char far* text)"    },
    {    "void",     "SetupWindow(void)"        },
    {    "BOOL",     "ShowWindow(int cmdShow)"  },
    {    "UINT",     "Transfer(void* buffer, TTransferDirection direction)" },
    {    "void",     "TransferData(TTransferDirection direction)"   },
    {    "LRESULT",  "WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)"   },

#if 0 // unsorted
    {    "void",     "Enable(BOOL enable)"   },
    {    "void",     "SetText(const char far* text)"    },
    {    "void",     "SetCheck(int check)"  },
    {    "BOOL",     "Register(void)"   },
    {    "BOOL",     "Create(void)" },
    {    "void",     "PerformCreate(int menuOrId)"  },
    {    "void",     "Destroy(int retVal)"  },
    {    "void",     "CloseWindow(int retVal)"  },
    {    "LRESULT",  "Dispatch(TEventInfo& info, WPARAM wp, LPARAM lp)" },
    {    "BOOL",     "PreProcessMsg(MSG& msg)"  },
    {    "BOOL",     "IdleAction(long idleCount)"   },
    {    "BOOL",     "HoldFocusHWnd(HWND hWndLose, HWND hWndGain)"  },
    {    "void",     "SetParent(TWindow* newParent)"    },
    {    "BOOL",     "SetDocTitle(const char far* docname, int index)"  },
    {    "BOOL",     "CanClose(void)"   },
    {    "LRESULT",  "WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)"   },
    {    "LRESULT",  "DefWindowProc(UINT msg, WPARAM wParam, LPARAM lParam)"    },
    {    "LRESULT",  "EvCommand(UINT id, HWND hWndCtl, UINT notifyCode)"    },
    {    "void",     "EvCommandEnable(TCommandEnabler& ce)" },
    {    "void",     "Paint(TDC& dc, BOOL erase, TRect& rect)"  },
    {    "UINT",     "Transfer(void* buffer, TTransferDirection direction)" },
    {    "void",     "TransferData(TTransferDirection direction)"   },
    {    "BOOL",     "EnableWindow(BOOL enable)"    },
    {    "BOOL",     "EnableUpdate(BOOL enable)"    },
    {    "BOOL",     "ShowWindow(int cmdShow)"  },
    {    "void",     "GetWindowClass(WNDCLASS& wndClass)"   },
    {    "char far*","GetClassName(void)"       },
    {    "void",     "SetupWindow(void)"        },
    {    "void",     "CleanupWindow(void)"      },
    {    "void",     "RemoveChild(TWindow* child)"  },
#endif
    {    NULL,   NULL   }
};//sorted

struct VFuncs VfTDialog[] = {
    {    "BOOL",     "Create(void)"     },
    {    "BOOL",     "DialogFunction(UINT message, WPARAM wParam, LPARAM lParam)"   },
    {    "HWND",     "DoCreate(void)"   },
    {    "int",      "DoExecute(void)"  },
    {    "BOOL",     "EvInitDialog(HWND hWndFocus)" },
    {    "int",      "Execute(void)"    },
    {    NULL,   NULL   }
};//sorted

struct VFuncs VfTButtonGadget[] = {
    {   "void ",    "Activate(TPoint& p)"   },
    {   "void ",    "BeginPressed(TPoint& p)"   },
    {   "void ",    "BuildCelArray(void)"   },
    {   "void ",    "CancelPressed(TPoint& p)"  },
    {   "TDib*",    "GetGlyphDib(void)" },
    {   "void ",    "ReleaseGlyphDib(TDib* glyph)"  },
    {    NULL,   NULL   }
};//sorted

struct VFuncs VfTComboBox[] = {
        {   "int",      "AddString(const char far* str)"    },
        {   "void",     "ClearList(void)"   },
        {   "int",      "DeleteString(int index)"   },
        {   "int",      "DirectoryList(UINT attrs, const char far* fileSpec)"   },
        {   "int",      "FindString(const char far* find, int indexStart) const"    },
        {   "int",      "GetCount(void)"  },
        {   "DWORD",    "GetItemData(int index) const"  },
        {   "int",      "GetSelIndex(void)"   },
        {   "int",      "GetString(char far* str, int index) const" },
        {   "int",      "GetStringLen(int index) const" },
        {   "int",      "InsertString(const char far* str, int index)"  },
        {   "int",      "SetItemData(int index, DWORD itemData)"    },
        {   "int",      "SetSelIndex(int index)"    },
        {   "int",      "SetSelString(const char far* findStr, int indexStart)" },
#if 0 // unsorted
        {   "int",      "AddString(const char far* str)"    },
        {   "int",      "InsertString(const char far* str, int index)"  },
        {   "int",      "DeleteString(int index)"   },
        {   "void",     "ClearList(void)"   },
        {   "int",      "DirectoryList(UINT attrs, const char far* fileSpec)"   },
        {   "int",      "GetCount(void)"  },
        {   "int",      "FindString(const char far* find, int indexStart) const"    },
        {   "int",      "GetStringLen(int index) const" },
        {   "int",      "GetString(char far* str, int index) const" },
        {   "int",      "GetSelIndex(void)"   },
        {   "int",      "SetSelIndex(int index)"    },
        {   "int",      "SetSelString(const char far* findStr, int indexStart)" },
        {   "DWORD",    "GetItemData(int index) const"  },
        {   "int",      "SetItemData(int index, DWORD itemData)"    },
#endif
        {    NULL,   NULL   }
};//sorted

struct VFuncs VfTControl[] = {
    {   "int",     "CompareItem(COMPAREITEMSTRUCT far& compareInfo)"   },
    {   "void",    "DeleteItem(DELETEITEMSTRUCT far& deleteInfo)"  },
    {   "void",    "DrawItem(DRAWITEMSTRUCT far& drawInfo)"    },
    {   "void",    "MeasureItem(MEASUREITEMSTRUCT far& measureInfo)"   },
    {   "void",    "ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)"   },
    {   "void",    "ODAFocus(DRAWITEMSTRUCT far& drawInfo)"    },
    {    NULL,   NULL   }
};//sorted


struct VFuncs VfTDC[] = {
    {"BOOL"     "DPtoLP(TPoint* points, int count = 1) const"  },
    {"BOOL",    "DrawText(const char far* str, int count, const TRect& r, WORD format=0)",   },
    {"int",     "EnumFontFamilies(const char far* family, FONTENUMPROC proc, void far* data) const" },
    {"int",     "EnumFonts(const char far* faceName, OLDFONTENUMPROC callback, void far* data) const"   },
    {"BOOL",    "ExtTextOut(int x, int y, WORD options, const TRect* r, const char far *string, int count, const int far *dx =0)" },
    {"BOOL",    "GetAspectRatioFilter(TSize& size) const"  },
    {"TColor",  "GetBkColor(void) const"    },
    {"int",     "GetBkMode(void) const"   },
    {"BOOL",    "GetCharABCWidths(UINT firstChar, UINT lastChar, ABC* abc)"    },
    {"BOOL",    "GetCharWidth(UINT firstChar, UINT lastChar, int* buffer)" },
    {"int",     "GetClipBox(TRect& rect) const"   },
    {"BOOL",    "GetDCOrg(TPoint& point) const" },
    {"int",     "GetDeviceCaps(int index) const"  },
    {"DWORD",   "GetFontData(DWORD table, DWORD offset, void* buffer, long data)"  },
    {"DWORD",   "GetGlyphOutline(UINT chr, UINT format, GLYPHMETRICS far& gm, DWORD buffSize, void far* buffer, const MAT2 far& mat2)"  },
    {"int",     "GetKerningPairs(int pairs, KERNINGPAIR far* krnPair)"    },
    {"int",     "GetMapMode(void) const"  },
    {"TColor",  "GetNearestColor(TColor Color) const"   },
    {"DWORD",   "GetOutlineTextMetrics(UINT data, OUTLINETEXTMETRIC far& otm)" },
    {"WORD",    "GetOutlineTextMetrics(UINT data, OUTLINETEXTMETRIC far& otm)" },
    {"int",     "GetPolyFillMode(void) const" },
    {"int",     "GetROP2(void) const" },
    {"int",     "GetStretchBltMode(void) const"   },
    {"BOOL",    "GetTabbedTextExtent(const char far* str, int stringLen, int numPositions, const int far* positions, TSize& size) const"    },
    {"UINT",    "GetTextAlign(void) const" },
    {"int",     "GetTextCharacterExtra(void) const" },
    {"TColor",  "GetTextColor(void) const"  },
    {"BOOL",    "GetTextExtent(const char far* str, int stringLen, TSize& size) const" },
    {"int",     "GetTextFace(int count, char far* facename) const" },
    {"BOOL",    "GetTextMetrics(TEXTMETRIC far& metrics) const" },
    {"BOOL",    "GetViewportExt(TSize& extent) const"  },
    {"BOOL",    "GetViewportOrg(TPoint& point) const"  },
    {"BOOL",    "GetWindowExt(TSize& extent) const"    },
    {"BOOL"     "GetWindowOrg(TPoint& point) const"    },
    {"BOOL",    "GrayString(const TBrush& brush, GRAYSTRINGPROC outputFunc, const char far* str, int count, const TRect& r)" },
    {"BOOL",    "ModifyWorldTransform(XFORM far& xform, DWORD mode)"    },
    {"BOOL",    "OffsetViewportOrg(const TPoint& delta, TPoint far* oldOrg=0)" },
    {"BOOL",    "OffsetWindowOrg(const TPoint& delta, TPoint far* oldOrg=0)"   },
    {"int",     "RealizePalette(void)"    },
    {"void",    "RestoreFont(void)"    },
    {"BOOL",    "ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom, TSize far *oldExtend=0)" },
    {"BOOL",    "ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom, TSize *oldExtent=0)"   },
    {"void",    "SelectObject(const TFont& font)"  },
    {"void",    "SelectObject(const TPalette& palette, BOOL forceBackground=FALSE)"    },
    {"void",    "SelectStockObject(int index)" },
    {"TColor",  "SetBkColor(TColor color)"  },
    {"int",     "SetBkMode(int mode)" },
    {"int",     "SetMapMode(int mode)"    },
    {"DWORD",   "SetMapperFlags(DWORD flag)"    },
    {"BOOL",    "SetMiterLimit(float newLimit, float* oldLimit=0)"  },
    {"int",     "SetPolyFillMode(int mode)"   },
    {"int",     "SetROP2(int mode)"   },
    {"int",     "SetStretchBltMode(int mode)" },
    {"UINT",    "SetTextAlign(UINT flags)" },
    {"int",     "SetTextCharacterExtra(int extra)" },
    {"TColor",  "SetTextColor(TColor color)"    },
    {"BOOL",    "SetTextJustification(int breakExtra, int breakCount)" },
    {"BOOL",    "SetViewportExt(const TSize& extent, TSize far* oldExtent=0)"  },
    {"BOOL",    "SetViewportOrg(const TPoint& origin, TPoint far* oldOrg=0)"   },
    {"BOOL",    "SetWindowExt(const TSize& extent, TSize far* oldExtent=0)"    },
    {"BOOL",    "SetWindowOrg(const TPoint& origin, TPoint far* oldOrg=0)" },
    {"BOOL",    "SetWorldTransform(XFORM far& xform)"   },
    {"BOOL",    "TabbedTextOut(const TPoint& p, const char far* str, int count, int numPositions, const int far* positions, int tabOrigin, TSize& s)" },
    {"BOOL",    "TextOut(int x, int y, const char far* str, int count=-1)"  },

#if 0
    {"BOOL",    "GetDCOrg(TPoint& point) const" },
    {"int",     "GetDeviceCaps(int index) const"  },
    {"void",    "SelectObject(const TFont& font)"  },
    {"void",    "SelectObject(const TPalette& palette, BOOL forceBackground=FALSE)"    },
    {"void",    "SelectStockObject(int index)" },
    {"void",    "RestoreFont(void)"    },
    {"TColor",  "GetNearestColor(TColor Color) const"   },
    {"int",     "RealizePalette(void)"    },
    {"TColor",  "GetBkColor(void) const"    },
    {"TColor",  "SetBkColor(TColor color)"  },
    {"int",     "GetBkMode(void) const"   },
    {"int",     "SetBkMode(int mode)" },
    {"int",     "GetPolyFillMode(void) const" },
    {"int",     "SetPolyFillMode(int mode)"   },
    {"int",     "GetROP2(void) const" },
    {"int",     "SetROP2(int mode)"   },
    {"int",     "GetStretchBltMode(void) const"   },
    {"int",     "SetStretchBltMode(int mode)" },
    {"TColor",  "GetTextColor(void) const"  },
    {"TColor",  "SetTextColor(TColor color)"    },
    {"BOOL",    "SetMiterLimit(float newLimit, float* oldLimit=0)"  },
    {"int",     "GetMapMode(void) const"  },
    {"int",     "SetMapMode(int mode)"    },
    {"BOOL",    "SetWorldTransform(XFORM far& xform)"   },
    {"BOOL",    "ModifyWorldTransform(XFORM far& xform, DWORD mode)"    },
    {"BOOL",    "GetViewportOrg(TPoint& point) const"  },
    {"BOOL",    "SetViewportOrg(const TPoint& origin, TPoint far* oldOrg=0)"   },
    {"BOOL",    "OffsetViewportOrg(const TPoint& delta, TPoint far* oldOrg=0)" },
    {"BOOL",    "GetViewportExt(TSize& extent) const"  },
    {"BOOL",    "SetViewportExt(const TSize& extent, TSize far* oldExtent=0)"  },
    {"BOOL",    "ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom, TSize far *oldExtend=0)" },
    {"BOOL"     "GetWindowOrg(TPoint& point) const"    },
    {"BOOL",    "SetWindowOrg(const TPoint& origin, TPoint far* oldOrg=0)" },
    {"BOOL",    "OffsetWindowOrg(const TPoint& delta, TPoint far* oldOrg=0)"   },
    {"BOOL",    "GetWindowExt(TSize& extent) const"    },
    {"BOOL",    "SetWindowExt(const TSize& extent, TSize far* oldExtent=0)"    },
    {"BOOL",    "ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom, TSize *oldExtent=0)"   },
    {"BOOL"     "DPtoLP(TPoint* points, int count = 1) const"  },
    {"int",     "GetClipBox(TRect& rect) const"   },
    {"BOOL",    "TextOut(int x, int y, const char far* str, int count=-1)"  },
    {"BOOL",    "ExtTextOut(int x, int y, WORD options, const TRect* r, const char far *string, int count, const int far *dx =0)" },
    {"DWORD",   "SetMapperFlags(DWORD flag)"    },
    {"BOOL",    "GetAspectRatioFilter(TSize& size) const"  },
    {"UINT",    "SetTextAlign(UINT flags)" },
    {"UINT",    "GetTextAlign(void) const" },
    {"int",     "GetTextCharacterExtra(void) const" },
    {"int",     "SetTextCharacterExtra(int extra)" },
    {"int",     "GetTextFace(int count, char far* facename) const" },
    {"BOOL",    "SetTextJustification(int breakExtra, int breakCount)" },
    {"BOOL",    "GetTextMetrics(TEXTMETRIC far& metrics) const" },
    {"int",     "GetKerningPairs(int pairs, KERNINGPAIR far* krnPair)"    },
    {"BOOL",    "GetTextExtent(const char far* str, int stringLen, TSize& size) const" },
    {"DWORD",   "GetOutlineTextMetrics(UINT data, OUTLINETEXTMETRIC far& otm)" },
    {"WORD",    "GetOutlineTextMetrics(UINT data, OUTLINETEXTMETRIC far& otm)" },
    {"BOOL",    "GetCharWidth(UINT firstChar, UINT lastChar, int* buffer)" },
    {"DWORD",   "GetFontData(DWORD table, DWORD offset, void* buffer, long data)"  },
    {"BOOL",    "GetCharABCWidths(UINT firstChar, UINT lastChar, ABC* abc)"    },
    {"BOOL",    "TabbedTextOut(const TPoint& p, const char far* str, int count, int numPositions, const int far* positions, int tabOrigin, TSize& s)" },
    {"BOOL",    "DrawText(const char far* str, int count, const TRect& r, WORD format=0)",   },
    {"BOOL",    "GrayString(const TBrush& brush, GRAYSTRINGPROC outputFunc, const char far* str, int count, const TRect& r)" },
    {"BOOL",    "GetTabbedTextExtent(const char far* str, int stringLen, int numPositions, const int far* positions, TSize& size) const"    },
    {"DWORD",   "GetGlyphOutline(UINT chr, UINT format, GLYPHMETRICS far& gm, DWORD buffSize, void far* buffer, const MAT2 far& mat2)"  },
    {"int",     "EnumFonts(const char far* faceName, OLDFONTENUMPROC callback, void far* data) const"   },
    {"int",     "EnumFontFamilies(const char far* family, FONTENUMPROC proc, void far* data) const" },
#endif
    { NULL, NULL }
};


struct VFuncs VfTDecoratedFrame[] = {

    {"void",    "Insert(TWindow& decoration, TLocation = Top)"  },
    { NULL, NULL }
};

struct VFuncs VfTEditView[] = {
    { NULL, NULL }
};

struct VFuncs VfTEdit[] = {
    { NULL, NULL }
};

struct VFuncs VfTListView[] = {
    { NULL, NULL }
};

struct VFuncs VfTStatic[] = {
    { "void","Clear(void)"  },
    { NULL, NULL }
};

struct VFuncs VfTEditFile[] = {
    {"BOOL",    "CanClear(void)"    },
    {"BOOL",    "CanClose(void)"    },
    { NULL, NULL }
};

struct VFuncs VfTDocManager[] = {
     {"TDocTemplate*",  "Clone(TModule* module, TDocTemplate*& phead=DocTemplateStaticHead)" },
     {"void",           "CmEnableClose(TCommandEnabler& hndlr)"  },
     {"void",           "CmEnableCreate(TCommandEnabler& hndlr)" },
     {"void",           "CmEnableNew(TCommandEnabler& hndlr)"    },
     {"void",           "CmEnableOpen(TCommandEnabler& hndlr)"   },
     {"void",           "CmEnableRevert(TCommandEnabler& hndlr)" },
     {"void",           "CmEnableSave(TCommandEnabler& hndlr)"   },
     {"void",           "CmEnableSaveAs(TCommandEnabler& hndlr)" },
     {"void",           "CmFileClose(void)"  },
     {"void",           "CmFileNew(void)"    },
     {"void",           "CmFileOpen(void)"   },
     {"void",           "CmFileRevert(void)" },
     {"void",           "CmFileSave(void)"   },
     {"void",           "CmFileSaveAs(void)" },
     {"void",           "CmViewCreate(void)" },
     {"TDocument*",     "CreateAnyDoc(const char far* path, long flags = 0)"  },
     {"TView*",         "CreateAnyView(TDocument& doc,long flags = 0)"    },
     {"TDocument*",     "CreateDoc(const char far* path, long flags = 0)"  },
     {"TView*",         "CreateView(TDocument& doc, long flags = 0)"   },
     {"BOOL",           "FlushDoc(TDocument& doc)"    },
     {"TDocument*",     "GetCurrentDoc(void)" },
     {"const char far*","GetViewName(void)"    },
     {"const char far*","GetViewName(void)"   },
     {"TDocument*",     "IsMyKindOfDoc(TDocument& doc)"    },
     {"TView*",         "IsMyKindOfView(TView& view)"  },
     {"TDocTemplate*",  "MatchTemplate(const char far* path)"   },
     {"UINT",           "PostDocError(TDocument& doc, UINT sid, UINT choice = MB_OK)"   },
     {"void",           "PostEvent(int id, TDocument& doc)" },
     {"void",           "PostEvent(int id, TView& view)"    },
     {"TDocTemplate*",  "SelectAnySave(TDocument& doc, BOOL samedoc = TRUE)"    },
     {"int",            "SelectDocPath(TDocTemplate** tpllist, int tplcount"    },
     {"int",            "SelectDocType(TDocTemplate** tpllist, int tplcount)"   },
     {"int",            "SelectViewType(TDocTemplate** tpllist, int tplcount)"  },

#if 0
     {"TDocument*",     "CreateDoc(const char far* path, long flags = 0)"  },
     {"TView*",         "CreateView(TDocument& doc, long flags = 0)"   },
     {"TDocument*",     "IsMyKindOfDoc(TDocument& doc)"    },
     {"TView*",         "IsMyKindOfView(TView& view)"  },
     {"const char far*","GetViewName(void)"   },
     {"TDocTemplate*",  "Clone(TModule* module, TDocTemplate*& phead=DocTemplateStaticHead)" },
     {"const char far*","GetViewName(void)"    },
     {"TDocument*",     "CreateAnyDoc(const char far* path, long flags = 0)"  },
     {"TView*",         "CreateAnyView(TDocument& doc,long flags = 0)"    },
     {"TDocTemplate*",  "SelectAnySave(TDocument& doc, BOOL samedoc = TRUE)"    },
     {"TDocTemplate*",  "MatchTemplate(const char far* path)"   },
     {"TDocument*",     "GetCurrentDoc(void)" },
     {"BOOL",           "FlushDoc(TDocument& doc)"    },
     {"void",           "CmFileOpen(void)"   },
     {"void",           "CmFileNew(void)"    },
     {"void",           "CmFileClose(void)"  },
     {"void",           "CmFileSave(void)"   },
     {"void",           "CmFileSaveAs(void)" },
     {"void",           "CmFileRevert(void)" },
     {"void",           "CmViewCreate(void)" },
     {"UINT",           "PostDocError(TDocument& doc, UINT sid, UINT choice = MB_OK)"   },
     {"void",           "PostEvent(int id, TDocument& doc)" },
     {"void",           "PostEvent(int id, TView& view)"    },
     {"int",            "SelectDocPath(TDocTemplate** tpllist, int tplcount"    },
     {"int",            "SelectDocType(TDocTemplate** tpllist, int tplcount)"   },
     {"int",            "SelectViewType(TDocTemplate** tpllist, int tplcount)"  },
     {"void",           "CmEnableNew(TCommandEnabler& hndlr)"    },
     {"void",           "CmEnableOpen(TCommandEnabler& hndlr)"   },
     {"void",           "CmEnableSave(TCommandEnabler& hndlr)"   },
     {"void",           "CmEnableSaveAs(TCommandEnabler& hndlr)" },
     {"void",           "CmEnableRevert(TCommandEnabler& hndlr)" },
     {"void",           "CmEnableClose(TCommandEnabler& hndlr)"  },
     {"void",           "CmEnableCreate(TCommandEnabler& hndlr)" },
#endif
    { NULL, NULL }
};//sorted

struct VFuncs VfTDocView[] = {
     {"int",        "FindProperty(const char far* name)"   },
     {"LPCSTR",     "GetViewName(void)"    },
     {"TWindow*",   "GetWindow(void)"    },
     {"int",        "GetProperty(int index, void far* dest, int textlen=0)"    },
     {"int",        "PropertyCount(void)"  },
     {"int",        "PropertyFlags(int index)" },
     {"const char*","PropertyName(int index)"    },
     {"BOOL",       "SetDocTitle(LPCSTR docname, int index)"   },
     {"BOOL",       "SetProperty(int index, const void far* src)"  },
    { NULL, NULL }
};//sorted

struct VFuncs VfTFrameWindow[] = {
     {"BOOL",          "AssignMenu(TResId menuResId)"  },
     {"TWindow*",      "GetClientWindow(void)" },
     {"HWND",          "GetCommandTarget(void)"    },
     {"TWindow*",      "SetClientWindow(TWindow* clientWnd)"   },
     {"BOOL",          "SetMenu(HMENU newMenu)"    },
    { NULL, NULL }
};//sorted




struct VFuncs VfTDocument[] = {
     {"void",       "AttachStream(TStream& strm)"   },
     {"BOOL",       "CanClose(void)"   },
     {"BOOL",       "Close(void)"  },
     {"BOOL",       "Commit(BOOL force=FALSE)" },
     {"void",       "DetachStream(TStream& strm)"   },
     {"int",        "FindProperty(const char far* name)"   },
     {"int",        "GetProperty(int index, void far* dest, int textlen=0)"    },
     {"TInStream*", "InStream(int mode, LPCSTR strmId=0)"  },
     {"BOOL",       "IsDirty(void)"    },
     {"BOOL",       "IsOpen(void)" },
     {"BOOL",       "Open(int mode, LPCSTR path=0)" },
     {"TOutStream*","OutStream(int mode, LPCSTR strmId=0)" },
     {"UINT",       "PostError(UINT sid, UINT choice = MB_OK)" },
     {"int",        "PropertyCount(void)"   },
     {"int",        "PropertyFlags(int index)" },
     {"const char*","PropertyName(int index)"    },
     {"BOOL",       "Revert(BOOL clear=FALSE)" },
     {"TDocument&", "RootDocument(void)"   },
     {"BOOL",       "SetDocPath(LPCSTR path)"  },
     {"BOOL",       "SetProperty(int index, const void far* src)"  },
     {"void",       "SetTitle(LPCSTR title)"   },
#if 0
     {"TInStream*", "InStream(int mode, LPCSTR strmId=0)"  },
     {"TOutStream*","OutStream(int mode, LPCSTR strmId=0)" },
     {"BOOL",       "Open(int mode, LPCSTR path=0)" },
     {"BOOL",       "Close(void)"  },
     {"BOOL",       "Commit(BOOL force=FALSE)" },
     {"BOOL",       "Revert(BOOL clear=FALSE)" },
     {"TDocument&", "RootDocument(void)"   },
     {"BOOL",       "SetDocPath(LPCSTR path)"  },
     {"void",       "SetTitle(LPCSTR title)"   },
     {"BOOL",       "IsDirty(void)"    },
     {"BOOL",       "IsOpen(void)" },
     {"BOOL",       "CanClose(void)"   },
     {"UINT",       "PostError(UINT sid, UINT choice = MB_OK)" },
     {"int",        "PropertyCount(void)"   },
     {"int",        "FindProperty(const char far* name)"   },
     {"int",        "PropertyFlags(int index)" },
     {"const char*","PropertyName(int index)"    },
     {"int",        "GetProperty(int index, void far* dest, int textlen=0)"    },
     {"BOOL",       "SetProperty(int index, const void far* src)"  },
     {"void",       "AttachStream(TStream& strm)"   },
     {"void",       "DetachStream(TStream& strm)"   },
#endif
    { NULL, NULL }
};//sorted

struct VFuncs VfTGadget[] = {
     {"void",      "CommandEnable(void)"  },
     {"void",      "GetDesiredSize(TSize& size)"  },
     {"void",      "Inserted(void)"   },
     {"void",      "LButtonDown(UINT modKeys, TPoint& point)" },
     {"void",      "LButtonUp(UINT modKeys, TPoint& point)"   },
     {"void",      "MouseEnter(UINT modKeys, TPoint& point)"  },
     {"void",      "MouseLeave(UINT modKeys, TPoint& point)"  },
     {"void",      "MouseMove(UINT modKeys, TPoint& point)"   },
     {"void",      "Paint(TDC& dc)"   },
     {"void",      "PaintBorder(TDC& dc)" },
     {"BOOL",      "PtIn(TPoint& point)"  },
     {"void",      "Removed(void)"    },
     {"void",      "SetBounds(TRect& rect)"   },
     {"void",      "SetEnabled(BOOL)" },
     {"void",      "SysColorChange(void)" },
#if 0
     {"void",      "SetEnabled(BOOL)" },
     {"void",      "GetDesiredSize(TSize& size)"  },
     {"void",      "SetBounds(TRect& rect)"   },
     {"void",      "CommandEnable(void)"  },
     {"void",      "SysColorChange(void)" },
     {"void",      "Inserted(void)"   },
     {"void",      "Removed(void)"    },
     {"BOOL",      "PtIn(TPoint& point)"  },
     {"void",      "Paint(TDC& dc)"   },
     {"void",      "PaintBorder(TDC& dc)" },
     {"void",      "MouseMove(UINT modKeys, TPoint& point)"   },
     {"void",      "MouseEnter(UINT modKeys, TPoint& point)"  },
     {"void",      "MouseLeave(UINT modKeys, TPoint& point)"  },
     {"void",      "LButtonDown(UINT modKeys, TPoint& point)" },
     {"void",      "LButtonUp(UINT modKeys, TPoint& point)"   },
#endif
    { NULL, NULL }
};//sorted


struct VFuncs VfTGadgetWindow[] = {
     {"void",     "GetDesiredSize(TSize& size)"   },
     {"void",     "GetInnerRect(TRect& rect)" },
     {"void",     "Insert(TGadget& gadget, TPlacement = After, TGadget* sibling = 0)" },
     {"void",     "LayoutSession(void)"   },
     {"void",     "PaintGadgets(TDC& dc, BOOL erase, TRect& rect)"    },
     {"void",     "PositionGadget(TGadget* previous, TGadget* next, TPoint& point)"   },
     {"TGadget*", "Remove(TGadget& gadget)"   },
     {"void",     "SetDirection(TTileDirection direction)"    },
     {"TRect",    "TileGadgets(void)" },
    { NULL, NULL }
};//sorted


struct VFuncs VfTGauge[] = {
    { "void",   "PaintBorder(TDC& dc)"   },
    { NULL, NULL }
};

struct VFuncs VfTGroupBox[] = {
    {   "void", "SelectionChanged(int controlId)"   },
    { NULL, NULL }
};

struct VFuncs VfTListBox[] = {
     {"int",       "AddString(const char far* str)"    },
     {"void",      "ClearList(void)"   },
     {"int",       "DeleteString(int index)"   },
     {"int",       "DirectoryList(UINT attrs, const char far* fileSpec)"   },
     {"int",       "FindString(const char far* str, int index) const"  },
     {"int",       "GetCount(void) const"  },
     {"DWORD",     "GetItemData(int index) const"  },
     {"int",       "GetItemHeight(int index) const"    },
     {"int",       "GetSelIndex(void) const"   },
     {"int",       "GetString(char far* str, int index) const" },
     {"int",       "GetStringLen(int index) const" },
     {"int",       "InsertString(const char far* str, int index)"  },
     {"int",       "SetItemData(int index, DWORD itemData)"    },
     {"int",       "SetItemHeight(int index, int height)"  },
     {"int",       "SetSelIndex(int index)"    },
#if 0
     {"int",       "GetCount(void) const"  },
     {"int",       "FindString(const char far* str, int index) const"  },
     {"int",       "GetStringLen(int index) const" },
     {"int",       "GetString(char far* str, int index) const" },
     {"DWORD",     "GetItemData(int index) const"  },
     {"int",       "SetItemData(int index, DWORD itemData)"    },
     {"int",       "GetItemHeight(int index) const"    },
     {"int",       "SetItemHeight(int index, int height)"  },
     {"void",      "ClearList(void)"   },
     {"int",       "DirectoryList(UINT attrs, const char far* fileSpec)"   },
     {"int",       "AddString(const char far* str)"    },
     {"int",       "InsertString(const char far* str, int index)"  },
     {"int",       "DeleteString(int index)"   },
     {"int",       "GetSelIndex(void) const"   },
     {"int",       "SetSelIndex(int index)"    },
#endif
    { NULL, NULL }
};//sorted

struct VFuncs VfTMDIClient[] = {
     {"void",           "ArrangeIcons(void)"  },
     {"TMDIChild*",     "InitChild(void)" },
     {"void",           "CascadeChildren(void)"   },
     {"BOOL",           "CloseChildren(void)" },
     {"TWindow*",       "CreateChild(void)"   },
     {"void",           "TileChildren(int tile = MDITILE_VERTICAL)"   },
     { NULL, NULL },
};//sorted


struct VFuncs VfTMenu[] = {
     {"void",   "DrawItem(DRAWITEMSTRUCT far& drawItem)"    },
     {"void",   "MeasureItem(MEASUREITEMSTRUCT far& measureItem)"   },
    { NULL, NULL }
};

struct VFuncs VfTMessageBar[] = {
    {"void",   "SetHintText(const char* text)" },
    { NULL, NULL }
};

struct VFuncs VfTModule[] = {
     {"void",   "Error(int errorCode)"  },
     {"int",    "Error(xmsg& x, unsigned captionResId, unsigned promptResId=0)" },
    { NULL, NULL }
};//sorted

struct VFuncs VfTToolBox[] = {
    {"void",    "SetDirection(TTileDirection direction)"    },
    { NULL, NULL }
};


struct VFuncs VfTPrintPreviewDC[] = {
     {"void",     "ReOrg(void)"    },
     {"void",     "ReScale(void)"  },
     {"void",     "SyncFont(void)" },
    { NULL, NULL }
};//sorted

struct VFuncs VfTPrintout[] = {
     {"void",     "BeginDocument(int startPage, int endPage, unsigned flags)"  },
     {"void",     "BeginPrinting(void)"    },
     {"void",     "EndDocument(void)"  },
     {"void",     "EndPrinting(void)"  },
     {"void",     "GetDialogInfo(int& minPage, int& maxPage, int& selFromPage, int& selToPage)"    },
     {"BOOL",     "HasPage(int pageNumber)"    },
     {"void",     "PrintPage(int page, TRect& rect, unsigned flags)"   },
     {"void",     "SetPrintParams(TPrintDC* dc, TSize pageSize)"   },
    { NULL, NULL }
};//sorted

struct VFuncs VfTPrinter[] = {
     {"void",     "ClearDevice(void)"  },
     {"TWindow*", "CreateAbortWindow(TWindow* parent, TPrintout& printout)"    },
     {"BOOL",     "ExecPrintDialog(TWindow* parent)" },
     {"void",     "GetDefaultPrinter(void)"    },
     {"BOOL",     "Print(TWindow* parent, TPrintout& printout, BOOL prompt)"   },
     {"void",     "ReportError(TWindow* parent, TPrintout& printout)"  },
     {"void",     "SetPrinter(const char* driver, const char* device, const char* output)" },
     {"void",     "Setup(TWindow* parent)" },
    { NULL, NULL }
};//sorted

struct VFuncs VfTScrollBar[] = {
     {"int",        "DeltaPos(int delta)"   },
     {"int",        "GetPosition(void)" },
     {"void",       "GetRange(int& min, int& max)"  },
     {"void",       "SBBottom(void)"    },
     {"void",       "SBEndScroll(void)" },
     {"void",       "SBLineDown(void)"  },
     {"void",       "SBLineUp(void)"    },
     {"void",       "SBPageDown(void)"  },
     {"void",       "SBPageUp(void)"    },
     {"void",       "SBThumbPosition(int thumbPos)" },
     {"void",       "SBThumbTrack(int thumbPos)"    },
     {"void",       "SBTop(void)"   },
     {"void",       "SetPosition(int thumbPos)" },
     {"void",       "SetRange(int min, int max)"    },
#if 0
     {"void",       "GetRange(int& min, int& max)"  },
     {"int",        "GetPosition(void)" },
     {"void",       "SetRange(int min, int max)"    },
     {"void",       "SetPosition(int thumbPos)" },
     {"int",        "DeltaPos(int delta)"   },
     {"void",       "SBLineUp(void)"    },
     {"void",       "SBLineDown(void)"  },
     {"void",       "SBPageUp(void)"    },
     {"void",       "SBPageDown(void)"  },
     {"void",       "SBThumbPosition(int thumbPos)" },
     {"void",       "SBThumbTrack(int thumbPos)"    },
     {"void",       "SBTop(void)"   },
     {"void",       "SBBottom(void)"    },
     {"void",       "SBEndScroll(void)" },
#endif
    { NULL, NULL }
};//sorted

struct VFuncs VfTScroller[] = {
     {"void",      "AutoScroll(void)"  },
     {"void",      "BeginView(TDC& dc, TRect& rect)"   },
     {"void",      "EndView(void)" },
     {"void",      "HScroll(UINT scrollEvent, int thumbPos)"   },
     {"BOOL",      "IsAutoMode(void)"  },
     {"void",      "ScrollTo(long x, long y)"  },
     {"void",      "SetPageSize(void)" },
     {"void",      "SetRange(long xRange, long yRange)"    },
     {"void",      "SetSBarRange(void)"    },
     {"void",      "SetUnits(int xUnit, int yUnit)"    },
     {"void",      "VScroll(UINT scrollEvent, int thumbPos)"   },
    { NULL, NULL }
};//sorted

struct VFuncs VfTSlider[] = {
       {"int",      "HitTest(TPoint& point)"  },
       {"void",     "NotifyParent(int scrollCode, int pos=0)" },
       {"void",     "PaintRuler(TDC& dc)" },
       {"void",     "PaintSlot(TDC& dc)"  },
       {"void",     "PaintThumb(TDC& dc)" },
       {"int",      "PointToPos(TPoint& point)"   },
       {"TPoint",   "PosToPoint(int pos)"    },
       {"void",     "SetupThumbRgn(void)" },
       {"void",     "SlideThumb(TDC& dc, int thumbPos)"   },
    { NULL, NULL }
};//sorted

struct VFuncs VfTCommandEnabler[] = {
     {"void",      "Enable(BOOL enable = 1)"    },
     {"void",      "SetCheck(int check)"   },
     {"void",      "SetText(const char far* text)" },
    { NULL, NULL }
};//sorted


struct VFuncs VfTView[] = {
    {"int",         "FindProperty(const char far* name)" },
    {"LPCSTR",      "GetViewName()" },
    {"TWindow*",    "GetWindow()" },
    {"int",         "GetProperty(int index, void far* dest, int textlen=0)" },
    {"int",         "PropertyCount()" },
    {"int",         "PropertyFlags(int index)" },
    {"const char*", "PropertyName(int index)" },
    {"BOOL",        "SetDocTitle(LPCSTR docname, int index)" },
    {"BOOL",        "SetProperty(int index, const void far* src)" },
    { NULL, NULL }
};//sorted



struct VFunctions TheVFunctions[] = {
    {   "TWindow",          VfTWindow   },
    {   "TDialog",          VfTDialog   },
    {   "TApplication",     VfTApplication },
    {   "TButtonGadget",    VfTButtonGadget },
    {   "TComboBox",        VfTComboBox },
    {   "TControl",         VfTControl },
    {   "TListBox",         VfTListBox },
    {   "TDC",              VfTDC },
    {   "TDecoratedFrame",  VfTDecoratedFrame },
    {   "TEdit",            VfTEdit },
    {   "TEditFile",        VfTEditFile },
    {   "TEditView",        VfTEditView },
    {   "TListView",        VfTListView },
    {   "TStatic",          VfTStatic },
    {   "TDocManager",      VfTDocManager   },
    {   "TDocument",        VfTDocument   },
    {   "TDocView",         VfTDocView   },
    {   "TGadget",          VfTGadget    },
    {   "TGadgetWindow",    VfTGadgetWindow },
    {   "TGauge",           VfTGauge },
    {   "TGroupBox",        VfTGroupBox },
    {   "TListBox",         VfTListBox },
    {   "TMDIClient",       VfTMDIClient },
    {   "TMenu",            VfTMenu },
    {   "TMessageBar",      VfTMessageBar },
    {   "TModule",          VfTModule },
    {   "TToolBox",         VfTToolBox },
    {   "TFrameWindow",     VfTFrameWindow },
    {   "TPrintPreviewDC",  VfTPrintPreviewDC   },
    {   "TPrintout",        VfTPrintout   },
    {   "TPrinter",         VfTPrinter   },
    {   "TScrollBar",       VfTScrollBar  },
    {   "TScroller",        VfTScroller  },
    {   "TCommandEnabler",  VfTCommandEnabler  },
    {   "TView",            VfTView },
    { NULL,NULL }
};


// ------------------------------------------------------------------
ClassTree   TheTree[] = {
    {   "TWindow",          NULL    },
    {   "TView",            NULL    },
    {   "TDocManager",      NULL    },
    {   "TDocTemplate",     NULL    },
    {   "TDocument",        NULL    },
    {   "TModule",          NULL    },
    {   "TScroller",        NULL    },
    {   "TValidator",       NULL    },
    {   "TPrinter",         NULL    },
    {   "TPrintOut",        NULL    },
    ///////
    {   "TApplication",     "TModule"   },
    {   "TWindowView",      "TWindow"   },
    {   "TEditView",        "TView"         },  //and
    {   "TEditView",        "TEditSearch"   },  //and
    {   "TListView",        "TView"     },  // and
    {   "TListView",        "TListBox"  },  // and
    {   "TFileDocument",    "TDocument" },
    {   "TStorageDocument", "TDocument" },
    {   "TDialog",          "TWindow"   },
    {   "TControl",         "TWindow"   },
    {   "TTinyCaption",     "TWindow"   },
    {   "TFrameWindow",     "TWindow"   },
    {   "TLayoutWindow",    "TWindow"   },
    {   "TPrevievPage",     "TWindow"   },
    {   "TMDIClient",       "TWindow"   },
    {   "TGadgetWindow",    "TWindow"   },
    {   "TFloatingFrame",   "TTinyCaption", },  // and
    {   "TFloatingFrame",   "TFrameWindow", },  // and
    {   "TMDIChild",        "TFrameWindow", },
    {   "TMDIFrame",        "TFrameWindow", },
    {   "TDecoratedFrame",  "TFrameWindow", },
    {   "TDecoratedMDIFrame","TMDIFrame", },        // and
    {   "TDecoratedMDIFrame","TDecoratedFrame", },  // and
    {   "TDecoratedFrame",  "TFrameWindow", },
    {   "TToolBox",         "TGadgetWindow" },
    {   "TMessageBar",      "TGadgetWindow" },
    {   "TControlBar",      "TGadgetWindow" },
    {   "TStatusBar",       "TMessageBar"   },
    {   "TFileDocument",    "TDocument" },
    {   "TBitmpaGadget",    "TGadget"   },
    {   "TButtonGadget",    "TGadget"   },
    {   "TTextGadget",      "TGadget"   },
    {   "TSeperatorGadget", "TGadget"   },
    {   "TTControlGadget",  "TGadget"   },
    {   "TScrollBar",       "TControl", },
    {   "TGauge",           "TControl", },
    {   "TTGroupBox",       "TControl", },
    {   "TStatic",          "TControl", },
    {   "TButton",          "TControl", },
    {   "TListBox",         "TControl", },
    {   "TComboxBox",       "TListBox"  },
    {   "TSlider",          "TScrollBar"    },
    {   "THSlider",         "TSlider",      },
    {   "TVSlider",         "TSlider",      },
    {   "TEdit",            "TStatic",      },
    {   "TEditSearch",      "TEdit",        },
    {   "TEditFile",        "TEditSearch",  },
    {   "TCheckBox",        "TButton"       },
    {   "TRadioButton",     "TCheckBox"     },
    // common dialogs
    {   "TInputDialog",     "TDialog"       },
    {   "TPrinterAbortDlg", "TDialog"       },
    {   "TCommonDialog",    "TDialog"       },
    {   "TChooseColorDialog",   "TCommonDialog"   },
    {   "TFindReplaceDialog",   "TCommonDialog"   },
    {   "TChooseFontDialog",    "TCommonDialog"   },
    {   "TPrintDialog",         "TCommonDialog"   },
    {   "TOpenSaveDialog",      "TCommonDialog"   },
    {   "TFindDialog",          "TFindReplaceDialog" },
    {   "TReplaceDialog",       "TFindReplaceDialog" },
    {   "TFileOpenDialog",      "TOpenSaveDialog"    },
    {   "TFileSaveDialog",      "TOpenSaveDialog"    },
    //
    {   "TFilterValidator",     "TValidator"        },
    {   "TPXPictureValidator",  "TValidator"        },
    {   "TLookupValidator",     "TValidator"        },
    {   "TRangeValidator",      "TFilterValidator"      },
    {   "TStringLookupValidator",   "TLookupValidator", },
    //
    {   "TXOwl",                "TXBase"        },
    {   "TXCompatibility",      "TXOwl"         },
    {   "TXOutofMemory"         "TXOwl"         },
    //
    {   "TMenu",                NULL            },
    {   "TSystemMenu",          "TMenu"         },
    {   "TPopupMenu",           "TMenu"         },
    {   "TMenuDescr",           "TMenu"         },
    //
    {   "TCommandEnabler",      NULL    },
    {   "TMenuItemEnabler",     "TCommandEnabler"   },
    {   "TButtonGadgetEnabler", "TCommandEnabler"   },
    //
    {   "TGdiBase",              NULL    },
    {   "TGdiObject",           "TGdiBase"  },
    {   "TIcon",                "TGdiBase"  },
    {   "TCursor",              "TGdiBase"  },
    {   "TDib",                 "TGdiBase"  },
    {   "TReigon",              "TGdiObject"    },
    {   "TBitMap",              "TGdiObject"    },
    {   "TFont",                "TGdiObject"    },
    {   "TPalette",             "TGdiObject"    },
    {   "TBrush",               "TGdiObject"    },
    {   "TPen",                 "TGdiObject"    },
    //
    {   "TGadgetWindowFont",    "TFont"    },
    //
    {   "TDC",                  "TGdiBase"  },
    {   "TWindowDC",            "TDC"  },
    {   "TPaintDC",             "TDC"  },
    {   "TCreateDC",            "TDC"  },
    {   "TMetafileDC",          "TDC"  },
    //
    {   "TDesktopDC",           "TWindowDC" },
    {   "TScreenDC",            "TWindowDC" },
    //
    {   "TDibDC",               "TCreateDC" },
    {   "TPrintDC",             "TCreateDC" },
    {   "TIC",                  "TCreateDC" },
    {   "TMemoryDC",            "TCreateDC" },
    //
    {   "TBitSet",              "TCharSet"  },
    {   NULL, NULL }
};

//
// Given a class  return parent class name
// or return NULL string (empty string)
// example -- Class = "TListBox" ==> Parent1 = "TListView", Parenr2 = ""
//
void FindParentClass(char *Class, char *Parent1, char *Parent2)
{
    ClassTree *p = TheTree;
    while( p->Base )
    {
        if(strcmp(Class,p->Base) == 0)
        {
            if(p->Parent)
            {
                strcpy(Parent1,p->Parent);
                *Parent2 = NULL;
                p++;    // look for other parent
                while( p->Base )
                {
                    if(strcmp(Class,p->Base) == 0)
                    {
                        strcpy(Parent2, p->Parent);
                    }
                    p++;
                }
            }
            else
            {
                *Parent1 = NULL;  // no parent!
                *Parent2 = NULL;  // no parent!
            }
            return;
        }
        p++;
    }
    *Parent1 = NULL;  // no parent!
    *Parent2 = NULL;
}


//
// Add Virtual Functions For
// this class, and parent class, and grandfather class, etc, etc...
//
void AddVirtualFunctionMess(char *BaseClass, TListBox *pLB)
{
    char Class[40+1];
    char Parent1[40+1];
    char Parent2[40+1];
    strncpy(Class,BaseClass,sizeof(Class));

    AddVfs(Class, pLB);

    FindParentClass(Class,Parent1,Parent2);
    if(Parent1[0])
        AddVirtualFunctionMess(Parent1, pLB);
    if(Parent2[0])
        AddVirtualFunctionMess(Parent2, pLB);
}

//
// Add Virtual Functions for this class only
//
void AddVfs(char *Class, TListBox *pLB)
{
    char s[MAXPATHSIZE+2];
    if(Class == NULL || *Class == NULL)
        return;
    //
    VFunctions *p = TheVFunctions;

    while( p->Class )
    {
        if(strcmp(Class,p->Class) == 0)
        {
            VFuncs *v = p->V;
            sprintf(s," <%s>",Class);   // space & "  <TClassName>"
            //
            // Check for <TClass> duplicate already in list box
            //
            pLB->AddString(s);
            while( v->retype )
            {
                sprintf(s,"%-8s %s",v->retype,v->funcparm);       // 1.03a %-8s
                pLB->AddString(s);
                v++;
            }
            pLB->SetSelIndex(0);
            return;
        }
        p++;
    }
}


//
// Return TRUE if class is derived from TWindow (most are)
//
int IsClassFromTWindow(char *BaseClass)
{
    char flag = 0;
    return IsClassFromTWindow(&flag,BaseClass);
}
//
// Recursively walk up class tree looking for TWindow Parent
//
char IsClassFromTWindow(char *flag, char *BaseClass)
{
    if(*flag)
    {
        return(TRUE);
    }
    if(strcmp(BaseClass,"TWindow") == 0)
    {
        *flag = TRUE;
        return(TRUE);
    }
    //
    char Class[40+1];
    char Parent1[40+1];
    char Parent2[40+1];
    strncpy(Class,BaseClass,sizeof(Class));

    FindParentClass(Class,Parent1,Parent2);
    if(Parent1 && Parent1[0])
    {
        return IsClassFromTWindow(Parent1);
    }
    if(Parent2 && Parent2[0])
    {
        return IsClassFromTWindow(Parent2);
    }
    return FALSE;
}



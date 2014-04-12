enum MLESAVE { rdonly=0, neversave, promptsave, autosave };

struct SETUPDATA
{
    char OwlPath[40+1];         // typical "owl\" forced to lowercase
    char IncludePath[256+1];    // "c:\bcos2\include"
    char Memprefix[80+1];       // "m_"
    int  EnableBak;             // Enable User's Backup files
    int  WarnBeep;              // Enable Warning Beeps when true
    int  RescanTool;            // True Rescan when tools is finished. else don't
    int  ShowConstants;         // On Main Dlg show constants in Item ListBox
    char EdBak[4+1];            // typically ".BKU"
    ///
    int  TabIndex;              // index into list box selection
    char Tabs[10+1];            // contains space TAB - used in code gen.
    MLESAVE EdMle;              // enum RdOnly,NeverSave,PromptSave,AutoSave
    ///
    char CommentFlag[80+1];     // "INSERT>> YOUR CODE HERE"
    char FuncDelimeter[128+1];  // usually "//---------------------------"
    char HFileHead[256+1];      // ".\_header_.h"
    char CFileHead[256+1];      // ".\_header_.cpp"
};


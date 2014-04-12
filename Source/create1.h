

int CreateClassTDialog(char *Hfile, char *Cfile,char *MyClass,char *MyId,char *incfile);
int CreateClassTWindow(char *Hfile, char *Cfile,char *MyClass, char *incfile);
int CreateClassTgeneric(char *Hfile, char *Cfile,char *MyClass,char *BaseClass,char *MyId,char *incfile);

#define     NEWAPPFLAG_DIALOG       0x01    // Other bits ognore if this set
#define     NEWAPPFLAG_SDI          0x02    //
#define     NEWAPPFLAG_MDI          0x04    //
#define     NEWAPPFLAG_MENU         0x08    //
#define     NEWAPPFLAG_CONTROLBAR   0x10    //
#define     NEWAPPFLAG_STATUSBAR    0x20    //

int CreateApplication(TWindow *Win, int Flags,char *FApp,char *FMain,char *FAbout,char *FRes);

char *TestNewAppNeeded(void);
void WriteHexBlock(FILE *fo, int bytes, unsigned char p[]);
int  CreateMakeFile(char *makename, char *ResourceFile, char* ListOfFiles[], char *objdir, char *LinkSwitches);

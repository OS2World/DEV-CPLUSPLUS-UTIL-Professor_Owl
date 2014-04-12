#=============================================================
# Professor OWL Open Sourced September 1, 2005
# Ed March  Barrington, Illinois
#
#   POWL.MAK - Makefile for project D:\PRO-OWL\OS2\powl.prj
#       Created on 10/25/95 at 20:55
#
#=============================================================

.AUTODEPEND

#=============================================================
#       Translator Definitions
#=============================================================
CC = bcc +POWL.CFG
TASM = tasm
TLIB = tlib
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = g:\BCOS2\LIB
INCLUDEPATH = g:\BCOS2\INCLUDE


#=============================================================
#       Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#       List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 powl.def \
 browse.obj \
 makedlg.obj \
 projfile.obj \
 projwin.obj\
 setup.obj \
 bigmess.obj \
 ob.obj \
 cl.obj \
 powl.res \
 newclass.obj \
 main.obj \
 dlgid.obj \
 create1.obj \
 parse.obj \
 reskid.obj \
 comment.obj \
 edfunc.obj \
 msgscan.obj \
 vardlg.obj \
 varadd.obj \
 startapp.obj \
 appopt.obj \
 binary.obj \
 misc.obj \
 userfunc.obj \
 about.obj \
 warnbeep.obj \
 tools.obj \
 prjpath.obj \
 vfun.obj \
 settool.obj \
 dlgmsg.obj \
 input.obj

#=============================================================
#       Explicit Rules
#=============================================================
powl.exe: powl.cfg $(EXE_DEPENDENCIES)
  $(TLINK)  /A:4096 /wdef /wimt /wdpl /Toe /aa /L$(LIBPATH) @&&|
g:\BCOS2\LIB\C02.OBJ+
browse.obj+
projfile.obj+
makedlg.obj+
projwin.obj+
setup.obj+
bigmess.obj+
ob.obj+
cl.obj+
newclass.obj+
comment.obj+
edfunc.obj+
msgscan.obj+
vardlg.obj+
varadd.obj+
startapp.obj+
appopt.obj+
binary.obj+
misc.obj+
userfunc.obj+
about.obj+
warnbeep.obj+
tools.obj+
prjpath.obj+
vfun.obj+
main.obj+
dlgid.obj+
create1.obj+
parse.obj+
reskid.obj+
settool.obj+
dlgmsg.obj+
input.obj
powl,powl
g:\BCOS2\LIB\BIDS2.LIB+
g:\BCOS2\LIB\OWLPM.LIB+
g:\BCOS2\LIB\C2MT.LIB+
g:\BCOS2\LIB\OS2.LIB
powl.def
|
  rc.exe powl.res powl.exe

#=============================================================
#       Individual File Dependencies
#=============================================================
Browse.obj:  powl.cfg Browse.Cpp

MakeDlg.obj: powl.cfg MakeDlg.Cpp

ProjFile.obj: powl.cfg ProjFile.Cpp

PROJWIN.obj: powl.cfg PROJWIN.CPP

SETUP.obj: powl.cfg SETUP.CPP


BIGMESS.obj: powl.cfg BIGMESS.CPP

OB.obj: powl.cfg OB.CPP

CL.obj: powl.cfg CL.CPP

powl.res: powl.cfg powl.RC
    brcc.exe -R -I$(INCLUDEPATH) -FO powl.res powl.RC

NEWCLASS.obj: powl.cfg NEWCLASS.CPP

main.obj: powl.cfg main.cpp

DLGID.obj: powl.cfg DLGID.CPP

CREATE1.obj: powl.cfg CREATE1.CPP

PARSE.obj: powl.cfg PARSE.CPP

RESKID.obj: powl.cfg RESKID.CPP

COMMENT.obj: powl.cfg COMMENT.CPP

EDFUNC.obj: powl.cfg EDFUNC.CPP

MSGSCAN.obj: powl.cfg MSGSCAN.CPP

VARDLG.obj: powl.cfg VARDLG.CPP

VARADD.obj: powl.cfg VARADD.CPP

STARTAPP.obj: powl.cfg STARTAPP.CPP

APPOPT.obj: powl.cfg APPOPT.CPP

BINARY.obj: powl.cfg BINARY.CPP

MISC.obj: powl.cfg MISC.CPP

DEMO.obj: powl.cfg DEMO.CPP

USERFUNC.obj: powl.cfg USERFUNC.CPP

ABOUT.obj: powl.cfg ABOUT.CPP

WARNBEEP.obj: powl.cfg WARNBEEP.CPP

TOOLS.obj: powl.cfg TOOLS.CPP

PRJPATH.obj: powl.cfg PRJPATH.CPP

VFUN.obj: powl.cfg VFUN.CPP

SETTOOL.obj: powl.cfg SETTOOL.CPP

DLGMSG.obj: powl.cfg DLGMSG.CPP

INPUT.obj: powl.cfg INPUT.CPP

#=============================================================
#       Compiler Configuration File
#=============================================================
powl.cfg: powl.mak
  copy &&|
-R
-Ob
-L$(LIBPATH)
-I$(INCLUDEPATH)
-H=D:\PRO-OWL\OS2\powl.CSM
-vi-
-sm
-y
-v
-N
| powl.cfg



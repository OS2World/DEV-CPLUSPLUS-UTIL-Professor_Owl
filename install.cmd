/*  Install Professor Owl
    ----------------------------------------------------
    INSTALL PROFESSOR OWL FROM THE CURRENT PATH TO A
    USER SPECIFED DIRECTORY. CREATE THE DIRECTORY
    AND A SHADOW OF THE DIRECTORY ON THE DESK TOP.
    COPY ALL FILE FROM SOURCE PATH TO DESTINATION GIVEN.
    ----------------------------------------------------
    Copyright (c) 1995
    Open Source under GNU GPL License
    Edward J. March Jr.
    1206 South Birch Drive
    Mt. Prospect, IL 60056
*/

echo off
Call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs
Call SysCls

/*
//  Determine the Current path (from where install.cmd is running from)
//  Most likely From Floppy A:\
*/
parse source . . argv0 .
temp = reverse(argv0)
temp = substr(temp, pos('\', temp))
Len = length(temp)
If Len > 3 then Do
	temp = substr(temp, 2)
End

CurrentPath = reverse(temp)
LastChar = Right(CurrentPath,1)
If  LastChar <> '\' then Do
    CurrentPath = CurrentPath'\'
End

Say '       Professor Owl Instalation'
Say '       ========================='
Say ' '
Say ' Installing From ' CurrentPath
Say ' '
Say 'Enter Full Path Of The Destination Directory '
Say 'To Create and install to (i.e  C:\Profowl)  ?'
Pull Destination

temp = SubStr(Destination,2,2)
if temp <> ":\" Then Do
    Say '********** ERROR **********'
    Say 'Invalid Destination Directoy'
    Say 'You Must Include The Drive Letter And Full Path'
    Say 'Press Any Key To Exit...'
	SysGetKey()
	Exit
End

title = Right(Destination,Length(Destination)-3);
FolderId = '<ID_'Destination'>'
Rc = SysCreateObject("WPFolder", title, Left(Destination,3), 'OBJECTID='FolderId, 'U')

If Rc <> 1 then Do
    Say '********** ERROR **********'
    Say 'Could Not Create Folder Directory ' Destination
	Say 'Error Code ' Rc
	Say 'Try another Directory Name'
	Say 'Press Any Key To Exit...'
	SysGetKey()
	Exit
End

Say 'Directory Created: ' Destination
Say '--------------------------------------------------------'
Say 'A Desktop Folder Icon Is Being Created For Professor Owl'
Say '   This Folder Will Contain The Related Files '
Say '--------------------------------------------------------'

/*
// SHADOW THE NEWLY CREATED FOLDER ON THE DESKTOP
*/
Rc = SysCreateShadow( FolderId,  '<WP_DESKTOP>' )
Rc = SysOpenObject(FolderId, 'ICON', 1);
Say 'Copying Files From ' CurrentPath ' to ' Destination 

/*
// LIST OF FILES TO INSTALL
*/
Call CopyTheFile 'ReadMe.txt'
Call CopyTheFile 'License.txt'
Call CopyTheFile 'Powl.inf'
Call CopyTheFile 'ProfOwl.ptr'
Call CopyTheFile 'Powl.exe'


Say '************************'
Say 'Instalation Complete... '
Say 'Press Any Key To Exit...'
SysGetKey()
Exit

/* --------------- COPY THE FILE FROM CurrentPath to Destination --------*/
CopyTheFile:
    Arg Name
    File = CurrentPath''Name
    rc1 = SysCopyObject(File, FolderId)
    if rc1 = 1 Then Do
	Say 'Copied file 'File	
    End
    if rc1 <> 1 Then Do
	Say '**** Error ' rc1 ' Coping file 'File
    End
Return 0


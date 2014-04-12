del powl.csm
cd inf-help
call zipit.cmd
cd ..

zip powl108s.zip *.h *.cpp powl.rc build.cmd powl.mak zipit.cmd license.txt
zip -ug powl108s.zip powlhelp.zip
zip -ug powl108s.zip powl.exe powl.inf
dir /L  *.zip

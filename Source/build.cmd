cd inf-help
call buildH.cmd
cd ..

del *.csm
del powl.exe
make -fpowl.mak
del *.csm
dir *.exe


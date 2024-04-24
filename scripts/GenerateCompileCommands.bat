@ECHO OFF
pushd %~dp0\
call projects\bin\premake5.exe export-compile-commands --file=projects/RedEngine.lua  
popd
PAUSE
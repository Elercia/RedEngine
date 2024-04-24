@ECHO OFF
pushd %~dp0\
call projects\bin\premake5.exe gmake2 --file=projects/RedEngine.lua
popd
PAUSE
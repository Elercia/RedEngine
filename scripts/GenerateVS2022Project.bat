@ECHO OFF
pushd %~dp0\
call projects\bin\premake5.exe vs2022 --file=projects/RedEngine.lua  
popd
PAUSE
REM @ECHO OFF

pushd %~dp0\..

echo %cd%

OpenCppCoverage.exe --sources runtime\ --sources tests\ --working_dir tests\ --substitute_pdb_source_path %cd%\tests\?%cd%\build\bin\Debug-windows-x86_64-vs2022\RedEngineTest --modules RedEngineTest -- tests\RedEngineTest.exe [ECS]

popd

pause

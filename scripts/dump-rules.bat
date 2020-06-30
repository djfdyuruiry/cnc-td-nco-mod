@echo off

pushd "%~dp0..\bin\Win32"

del RULES.INI

TiberianDawn.exe --dump-rules

popd

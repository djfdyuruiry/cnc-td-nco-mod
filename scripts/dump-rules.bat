@echo off

pushd "%~dp0..\bin\Win32"

del RULES-DEFAULT.INI

TiberianDawn.exe --dump-rules

popd

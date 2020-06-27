@echo off

call "%~dp0build-test.bat"

pushd "%~dp0\..\bin\Win32"

START /B TiberianDawn.exe --lua-repl &

popd

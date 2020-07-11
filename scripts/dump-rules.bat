@echo off

pushd "%~dp0..\bin\Win32"

if exist RULES-DEFAULT.INI (
  del RULES-DEFAULT.INI
)

TiberianDawn.exe --dump-rules

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Dumping rules Failed"

  popd
  exit 1
)

popd

@echo off
call "%~dp0lib\log.bat" "INFO" "Running dump-rules.bat"
pushd "%~dp0..\bin\Win32"

if exist RULES-DEFAULT.INI (
  del RULES-DEFAULT.INI
)

call "%~dp0lib\log.bat" "INFO" "TiberianDawn.exe --dump-rules"
TiberianDawn.exe --dump-rules

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Dumping rules Failed"

  popd
  exit 1
)

popd

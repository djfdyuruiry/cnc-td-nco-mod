@echo off
call "%~dp0lib\log.bat" "INFO" "Starting update-default-rules.bat"
call "%~dp0dump-rules.bat"

if exist "%~dp0..\game\RULES-DEFAULT.INI" (
  del "%~dp0..\game\RULES-DEFAULT.INI"
)

copy "%~dp0..\bin\Win32\RULES-DEFAULT.INI" "%~dp0\..\game"

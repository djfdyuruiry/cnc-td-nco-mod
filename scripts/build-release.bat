@echo off

call "%~dp0\build-and-dump-rules.bat"

if exist "%~dp0\..\game\RULES-DUMP.INI" (
  del "%~dp0\..\game\RULES-DUMP.INI"
)

copy "%~dp0\..\bin\Win32\RULES-DUMP.INI" "%~dp0\..\game"

call "%~dp0\build.bat" "Release"

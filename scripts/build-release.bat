@echo off

call "%~dp0build-and-dump-rules.bat"

if exist "%~dp0..\game\RULES-DEFAULT.INI" (
  del "%~dp0..\game\RULES-DEFAULT.INI"
)

copy "%~dp0..\bin\Win32\RULES-DEFAULT.INI" "%~dp0\..\game"

call "%~dp0build.bat" "Release"

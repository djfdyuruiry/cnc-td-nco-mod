@echo off

cls

set config=%~1

call "%~dp0lib\log.bat" "INFO" "Building bin\Win32\TiberianDawn.dll - %config%"

MSBuild.exe ^
  /p:configuration=%config% ^
  /t:clean ^
  /t:build ^
  "%~dp0..\TIBERIANDAWN\TiberianDawn.vcxproj"

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Build Failed"
  exit 1
)

call "%~dp0lib\log.bat" "INFO" "Build complete"

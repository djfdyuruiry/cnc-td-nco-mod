@echo off

cls

set config=%~1

call "%~dp0lib\log.bat" "INFO" "Building bin\Win32\TiberianDawn.dll - %config%"

if exist "%~dp0..\bin\Win32" (
  :: purge previous build output if it exists
  rmdir /q /s "%~dp0..\bin\Win32"
)

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

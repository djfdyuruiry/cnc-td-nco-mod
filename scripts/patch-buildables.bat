@echo off

pwsh "%~dp0patch-buildables.ps1" "%~1"

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Patching mod buildables failed"
  exit 1
)

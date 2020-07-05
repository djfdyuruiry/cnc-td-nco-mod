@echo off

pwsh "%~dp0package.ps1"

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Packaging mod failed"
  exit 1
)

@echo off

call "%~dp0build-release.bat"

pwsh "%~dp0build-debug-rules.ps1"

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Building debug rules failed"
  exit 1
)

call "%~dp0deploy.bat"

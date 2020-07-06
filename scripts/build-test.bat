@echo off

call "%~dp0build.bat" "TestConsole"

if not defined CI (
    :: run the unit tests when building locally
   "%~dp0run-tests.bat"
)

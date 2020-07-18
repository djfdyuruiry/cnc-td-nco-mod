@echo off

call "%~dp0build.bat" "Debug"
call "%~dp0deploy.bat"
call "%~dp0run-cnc.bat"

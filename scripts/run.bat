@echo off

call "%~dp0deploy.bat"

set CNC_PATH=C:\Program Files (x86)\Steam\steamapps\common\CnCRemastered

call "%~dp0lib\log.bat" "INFO" "Running CnC in debug mode at path: %CNC_PATH%"

call "%CNC_PATH%\tiberian_dawn.bat"

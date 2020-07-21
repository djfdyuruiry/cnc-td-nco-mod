@echo off

set "CNC_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\CnCRemastered"

call "%~dp0lib\log.bat" "INFO" "Running CnC in debug mode at path: %CNC_PATH%"

:: make sure game is not running by killing it
call "%~dp0kill-cnc.bat"

pushd "%CNC_PATH%"

:: reset log file
if exist %USERPROFILE%\Documents\CnCRemastered\nco.log (
  del %USERPROFILE%\Documents\CnCRemastered\nco.log
)

:: start game like the launcher does
START /B InstanceServerG.exe GAME_INDEX=0 ^
  CLIENT_PORT=16000 ^
  RUN_AS_APP ^
  NO_AUTO_EXIT ^
  LAUNCH_FROM_CLIENT= &

START /B ClientG.exe GAME_INDEX=0 TIBERIANDAWN &

pushd

cd "%~dp0"

:: make sure game is not running by killing it
Taskkill /IM InstanceServerG.exe /F 2> nul
Taskkill /IM ClientG.exe /F 2> nul

:: start game like the launcher does
START /B InstanceServerG.exe GAME_INDEX=0 ^
  CLIENT_PORT=16000 ^
  RUN_AS_APP ^
  NO_AUTO_EXIT ^
  LAUNCH_FROM_CLIENT= &

ClientG.exe GAME_INDEX=0 TIBERIANDAWN

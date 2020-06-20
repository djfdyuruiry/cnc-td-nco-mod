cd %~dp0

Taskkill /IM InstanceServerG.exe /F
Taskkill /IM ClientG.exe /F

START /B InstanceServerG.exe GAME_INDEX=0 CLIENT_PORT=16000 RUN_AS_APP NO_AUTO_EXIT LAUNCH_FROM_CLIENT= &
ClientG.exe GAME_INDEX=0 TIBERIANDAWN

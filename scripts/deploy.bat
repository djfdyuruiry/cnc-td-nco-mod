@echo off

set REPO_PATH=%~dp0..
set MOD_PATH=%USERPROFILE%\Documents\CnCRemastered\Mods\Tiberian_Dawn\nco

call "%~dp0lib\log.bat" "INFO" "Deploying mod files to path: %MOD_PATH%"

if not exist "%REPO_PATH%\bin" (
  echo "Mod has not been built yet, can't deploy!"
  exit 1
)

:: make sure game does not have a lock on any mod file by killing it
call "%~dp0kill-cnc.bat"

if exist "%MOD_PATH%" (
  rmdir /q /s "%MOD_PATH%"
)

mkdir "%MOD_PATH%"
mkdir "%MOD_PATH%\Data"

copy "%REPO_PATH%\game\ccmod.json" "%MOD_PATH%\ccmod.json"
copy "%REPO_PATH%\game\RULES.INI" "%MOD_PATH%\Data"
copy "%REPO_PATH%\game\lua\test.lua" "%MOD_PATH%\Data"

copy "%REPO_PATH%\bin\Win32\TiberianDawn.dll" "%MOD_PATH%\Data"

@echo off

cd "%~dp0.."

call "%~dp0build-release.bat"

set MOD_PATH=%USERPROFILE%\Documents\CnCRemastered\Mods\Tiberian_Dawn\nco

call "%~dp0lib\log.bat" "INFO" "Deploying mod files to path: %MOD_PATH%"

if exist "%MOD_PATH%" (
  rmdir /q /s "%MOD_PATH%"
)

mkdir "%MOD_PATH%"
mkdir "%MOD_PATH%\Data"

copy "game\ccmod.json" "%MOD_PATH%\ccmod.json"
copy "game\RULES.INI" "%MOD_PATH%\Data"
copy "game\test.lua" "%MOD_PATH%\Data"

copy "bin\Win32\TiberianDawn.dll" "%MOD_PATH%\Data"

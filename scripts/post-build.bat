@echo off

set outDir=%~1

call "%~dp0lib\log.bat" "INFO" "Copying rules, lua scripts and creating log directory..."

if exist %outDir%RULES.INI del %outDir%RULES.INI

copy ..\game\RULES.INI %outDir%

if exist %outDir%log rmdir /Q /S %outDir%log

mkdir %outDir%log

if exist %outDir%test.lua del %outDir%test.lua

copy ..\game\lua\test.lua %outDir%

if exist %outDir%dump-rules.lua del %outDir%dump-rules.lua

copy ..\game\lua\dump-rules.lua %outDir%

if exist %outDir%test-lua-rules.lua del %outDir%test-lua-rules.lua

copy ..\game\lua\test-lua-rules.lua %outDir%

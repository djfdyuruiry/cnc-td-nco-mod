@echo off

set level=%~1
set msg=%~2

if "%msg%"=="" (
  set level=INFO
  set msg=%~1
)

echo ^>^> %date% %time% - %level% - %msg%
echo.

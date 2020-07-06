@echo off

call "%~dp0lib\log.bat" "INFO" "Running tests"

pushd "%~dp0..\tests\Win32\Debug"

vstest.console "%~dp0..\tests\Win32\Debug\Nco.Tests.dll"

popd

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Tests failed"
  exit 1
)

call "%~dp0lib\log.bat" "INFO" "Tests passed"

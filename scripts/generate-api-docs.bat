@echo off

call "%~dp0lib\log.bat" "INFO" "Running generate-api-docs.bat"

pushd "%~dp0..\bin\Win32"

if exist lua-api.md (
  del lua-api.md
)

TiberianDawn.exe --generate-api-docs

if errorlevel 1 (
  call "%~dp0lib\log.bat" "ERROR" "Generating API docs failed"

  popd
  exit 1
)
if not exist "%~dp0..\bin\Win32\lua-api.md" (
  call "%~dp0lib\log.bat" "ERROR" "API docs were not generated!"

  popd
  exit 1
)

popd

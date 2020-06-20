@echo off

cls

call "%~dp0lib\log.bat" "INFO" "Building bin\Win32\TiberianDawn.dll"

"C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin\MSBuild.exe" ^
    /p:configuration=release ^
    /t:clean ^
    /t:build ^
    /verbosity:quiet ^
    "%~dp0..\TIBERIANDAWN\TiberianDawn.vcxproj"

call "%~dp0lib\log.bat" "INFO" "Build complete"

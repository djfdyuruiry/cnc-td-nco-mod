# C&C Tiberian Dawn: New Contruction Options

This is a mod for the remastered version of Tiberian Dawn.

## Features

- Full `rules.ini` support for Tiberian Dawn - all constants are now read from a rules file if present

- Scripting support using `Lua 5.3` - run your code on different menu and in-game events

## Development

**Tools Quick Start**

- Install Visual Sudio 2019 Community
    - Select `Desktop development with C++` in Installer Options
        - Select `MSVC v141 - VS 2017` option
        - Go to individual components and select `C++/CLI support for v141` option
- Download and install Windows 8.1 SDK from [here](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)
- Add `msbuild` location to your path (default is `%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin`)

**Build Quick Start**

- Open `sln` file in Visual Studio
- Select `No Upgrade` for both the Windows SDK and VC++ version prompts (if this appears)
- Select the `TestConsole` configuration
- Run `Local Windows Debugger`
- Files are output to `bin\Win32`
- Logs showing rules being configured can be seen @ `bin\Win32\log\nco.log`

### Troubleshooting

If you get a COM error when trying to open the solution/project, follow the below steps:

- Close Visual Studio
- Open and admin command prompt
- Run
    ```bat
    regsvr32 %SystemRoot%\System32\msxml3.dll
    regsvr32 %SystemRoot%\SysWOW64\msxml3.dll
    ```
- You should not be able to open the project/solution

### Debugging

- Open a command prompt in the repo root
- Run:
    ```batch
    scripts\run.bat
    ```
- Alt-tab out of the game menu once it loads
- Open Visual Studio
    - Click `Debug`
    - Click `Attach to Process...`
    - Select `InstanceServerG.exe`
- DLL will be loaded once you start a mission/skirmish

# Installing

- Open a command prompt in the repo root
- Run:
    ```batch
    script\deploy.bat
    ```

# Progress Log

- Added loading of rules file
- Rules file location can be set using the `TD_RULES_FILE` env var (Default is `RULES.INI`)
- Unit strength configurable for a few infantry unit types
- Added custom logging to a new log file `nco.log`

## Problem Archive

- ~Trying to set max speed but does not seem to work?~ (max value is 255 for that field - added validation logic)

Regex for modifying units

```
static InfantryTypeClass const ([\S\s\r]+?^\s+)(".+?")(.+\/\/ INI name for infantry[\S\s\r]+?^\s+)(\d+)(.+\/\/ Cost of infantry[\S\s\r]+?\);[\n\r])
```

Replace string

```
static InfantryTypeClass const $1$2$3Read_Cost($2, $4)$5
```

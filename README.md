# C&C Tiberian Dawn: New Contruction Options

This is a mod for the remastered version of Tiberian Dawn.

## Features

- Full `rules.ini` support for Tiberian Dawn - all constants are now read from a rules file if present

- Scripting support using `Lua 5.3` - run your code on different menu and in-game events

## Development

**Quick Start**

- Install Visual Sudio 2019 Community
    - Select `Desktop development with C++` in Installer Options
        - Select `MSVC v141 - VS 2017` option
        - Go to individual components and select `C++/CLI support for v141` option
- Download and install Windows 8.1 SDK from [here](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)
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

- Open Visual Studio
    - Build the project
    - Copy the new `dll` to the mod `Data` directory
- Copy the `game\scripts\tiberian_dawn.bat` file to the C&C Remastered install directory
- Double click the `bat` file to start the game
- Open Visual Studio
    - Click `Debug`
    - Click `Attach to Process...`
    - Select `InstanceServerG.exe`

# Installing

- Build the project
- Create a mod directory @ `%USERPROFILE%\Documents\CnCRemastered\Mods\Tiberian_Dawn\nco`
    - Copy `game\ccmod.json` into the mod directory
    - Create a `Data` directory
    - Copy the `bin\Win32\TiberianDawn.dll` into `Data`
- Copy `game\cfg\RULES.INI` to the C&C Remastered install directory

# Progress Log

- Added loading of rules file
- Rules file location can be set using the `TD_RULES_FILE` env var (Default is `RULES.INI`)
- Unit strength configurable for a few infantry unit types

- Trying to set max speed but does not seem to work??

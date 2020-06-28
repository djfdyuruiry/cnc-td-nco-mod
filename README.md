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

### Updating Tiberian Dawn

EA release patches for the C&C Remastered collection which will cause this mod to fail loading due to a version number conflict. To fix this you must cherry-pick from the EA [github repo](https://github.com/electronicarts/CnC_Remastered_Collection). Once you have identified the commit hash of the latest patch, run:

```batch
git fetch ea
git cherry-pick UPDATE_COMMIT_HASH
```

You will need to remove `REDALERT` files and fix merge conflicts. Build and install after this, the mod should load correctly.

# Installing

- Open a command prompt in the repo root
- Run:
    ```batch
    script\deploy.bat
    ```

## Troubleshooting

If you get an error on load of a scenario complaining the dll version is different than what the game expects, check the `CNC_DLL_API_VERSION` define in the `TIBERIANDAWN\DLLInterface.h` file. If it is less than what the game asked for you will need to cherry-pick latest changes from EA.

# Progress Log

- Added loading of rules file
    - Rules available for all units, infantry, aircraft and buildings
    - Mod config embedded in rules
- Rules file is loaded from mod `Data` directory (`RULES.INI`)
- Added custom logging to a new log file `nco.log`
- Lua events and event handler loading form Lua scripts (stored in mod `Data` directory)
    - One event currently: `onScenarioStart`
- Lua API for modifiying rules and getting current rule values
    - `infantry` rules only for now
- Built a LUA REPL (test app cli option) for handy debugging without having to rerun/rebuild
- Added GitHub actions Workflow for building project and running test app on push
- Used embedded Lua library to workaround the need to deploy extra content to the game directory
- Moving all data files into mod `Data` directory to make mod easier to one click install
- Dynamically resolving mod `Data` directory using module handle

## Ideas

- Write method in test console to dump a default RULES INI
- Add more events for different situations (Can_Capture, Scenario_End, On_Win, On_Fail, On_Startup, On_Shutdown)
- Add rules to toggle certain logic for specific scenarios (can build certain units etc.)
- Add rules to toggle hard-wired truths (can't build temple or laser as GDI with nod con yard etc.)
- Use new base/infantry functions created for Lua API to read more rules from file
- Add default mission type for units/infantry and can be repaired for units

## Path to v1

- ✓ add armour and other missing rules
- ✓ add lookups for all rules in object definitions
- ⚠ generate rules ini by reading defaults
- ✓ test on non-dev machine
- ⚠ create publish script
- ⚠ publish alpha to steam workshop
- ⚠ test workshop item on non-dev machine
- ⚠ write full docs
  - rules ini structure
  - rule names
  - valid rule values
  - lua scripting api
  - use id of workshop item in any path hints
  - add weapon wiki links in docs
- ⚠ publish v1 to steam workshop
- ⚠ test v1 workshop item on non-dev machine

## Problems

- All clear :)

## Problem Archive

- Prison text logic never gets called (Building get name)
- Can't build engineer in mission 1 regardless of rules (STRUTF parsing was wrong - fixed, also E6 is code for engineer even though E7 is the reference in code)
- DLL crashes second time a scenario is loaded from main menu (needed to close log file handle before DLL exit)

- ~Trying to set max speed but does not seem to work?~ (max value is 255 for that field - added validation logic)

Regex for modifying units

```
static InfantryTypeClass const ([\S\s\r]+?^\s+)(".+?")(.+\/\/ INI name for infantry[\S\s\r]+?^\s+)(\d+)(.+\/\/ Cost of infantry[\S\s\r]+?\);[\n\r])
```

Replace string

```
static InfantryTypeClass const $1$2$3Read_Cost($2, $4)$5
```

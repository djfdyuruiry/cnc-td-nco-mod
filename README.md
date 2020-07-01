# 🛠 New Contruction Options 🛠

This is a mod for the remastered version of C&C Tiberian Dawn.

It aims to make Tiberian Dawn as customisable as possible through configuration and scripting without having to modify source code directly.

The key goal is to overcome the limitation of only having one `dll` based mod loaded into the game at a time.

- [Features](#Features)
- [How to Use](#How-To-Use)
- [Installing](#Installing)
  - [Using Steam Workshop](#Using-Steam-Workshop)
  - [Manual Install](#Manual-Install)
- [RULES.INI Location](#RULESINI-Location)
  - [Steam Workshop](#Steam-Workshop)
  - [Manual Installation](#Manual-Installation)

---

## Features

- 📃 Full `RULES.INI` support for Tiberian Dawn - majority of constants are now read from a rules file if present

- ⚙ Scripting support using `Lua 5.3` - run your game logic on different menu and in-game events


---

## How to Use

There are several pages written in the wiki that will detail how to edit the `RULES.INI` file and create Lua scripts.

See the [New Construction Options Wiki](https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki)

---

## Installing

### Using Steam Workshop

- Open Steam
- Find C&C Remastered in your Game Library
- Click on `Workshop`
- Search for `New Construction Options`
- Click `Subscribe` 
- Once it has downloaded:
  - Start C&C
  - Enable the `New Contruction Options` mod in the options menu
  - Restart C&C

### Manual Install

- Go the the [Releases Page]()
- Download the latest `Manual Install` zip
- Open the Start menu
- Type `Run`
- Click on the `Run` app that is highlighted
- Copy and paste this: `explorer "%USERPROFILE\Documents\CnCRemastered\Mods\Tiberian_Dawn"`
- Click `OK`
- Unzip the downloaded file into this folder
- Start C&C
- Enable the `New Contruction Options` mod in the options menu
- Restart C&C

---

## RULES.INI Location

The `RULES.INI` file and Lua Scripts are stored inside the mod folder. To find this folder, follow the steps below.

### Steam Workshop

- Open Steam
- Find C&C Remastered in your Game Library
- Click the cog icon:

  <img src="docs/steam_rules_path_0.png" width="600" height="150" />

- Click `Manage`, then click `Browse local files`:

  <img src="docs/steam_rules_path_1.png" width="200" height="`180" />

- Click `Up` twice:

  <img src="docs/steam_rules_path_2.png" width="600" height="150" />

- Open the `workshop` folder
  - next, open `content`
  - next, open `1213210`
  - next, open `2147631402`
  - next, open `NCO`
  - next, open `Data`

    <img src="docs/steam_rules_path_3.png" width="800" height="100" />

### Manual installation

- Open the Start menu
- Type `Run`
- Click on the `Run` app that is highlighted
- Copy and paste this: `explorer "%USERPROFILE\Documents\CnCRemastered\Mods\Tiberian_Dawn\nco\Data"`
- Click `OK`

---

## Path to v1

- ✓ add armour and other missing rules
- ✓ add lookups for all rules in object definitions
- ✓ generate rules ini by reading defaults
- ✓ test on non-dev machine
- ✓ create publish script
- ✓ publish alpha to steam workshop
- ✓ test workshop item on non-dev machine
- ⚠ write full docs
  - ✓ rules ini structure
  - ✓ rule names
  - ✓ valid rule values
  - ⚠ lua scripting api
  - ⚠ use id of workshop item in any path hints
  - ⚠ add weapon wiki links in docs
  - ✓ move dev docs into wiki 
  - ⚠ steam mod getting started with `RULES.INI` and Lua script(s) path in main `README.md`
- ⚠ publish v1 to steam workshop
- ⚠ test v1 workshop item on non-dev machine

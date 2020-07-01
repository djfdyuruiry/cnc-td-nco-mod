# 🛠 New Contruction Options 🛠

This is a mod for the remastered version of C&C Tiberian Dawn.

It aims to make Tiberian Dawn as customisable as possible through configuration and scripting without having to modify source code directly.

The key goal is to overcome the limitation of only having one `dll` based mod loaded into the game at a time.

## Features

- 📃 Full `RULES.INI` support for Tiberian Dawn - majority of constants are now read from a rules file if present

- ⚙ Scripting support using `Lua 5.3` - run your game logic on different menu and in-game events

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

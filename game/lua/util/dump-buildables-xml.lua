local header = [[<?xml version="1.0"?>

<!-- 
  The commands in this file enable all infantry/buildings/units to be buildable 
  when the rules.ini is configured properly
-->

<ModGameCommands>
  <CNCModGameCommandManager>
    <CNCModGameCommands>

]]

local xmlCommandTemplate = [[
      <Command Name="%s">
        <GameCommand>GAME_COMMAND_TD_MOD_CUSTOM_BUILD%d</GameCommand>
      </Command>

]]

local footer = [[
    </CNCModGameCommands>
  </CNCModGameCommandManager>
</ModGameCommands>

]]

local typeAreas =
{
  Infantry = {
    getTypes = getInfantryTypes,
    getRuleNames = getInfantryRuleNames,
    getRuleValue = getInfantryRule,
    rules = {}
  },
  Units = {
    getTypes = getUnitTypes,
    getRuleNames = getUnitRuleNames,
    getRuleValue = getUnitRule,
    rules = {}
  },
  Aircraft = {
    getTypes = getAircraftTypes,
    getRuleNames = getAircraftRuleNames,
    getRuleValue = getAircraftRule,
    rules = {}
  },
  Buildings = {
    getTypes = getBuildingTypes,
    getRuleNames = getBuildingRuleNames,
    getRuleValue = getBuildingRule,
    rules = {}
  }
}

local typeAreaOrder =
{
  "Infantry",
  "Units",
  "Aircraft",
  "Buildings"
}

local outputFileName = "CNCModGameCommands.xml"
local buildablesFile = nil

local function dumpBuildablesForTypeArea(buildablesFile, typeAreaName, typeArea)
  buildablesFile:write(string.format("      <!-- %s -->\n", typeAreaName))
  
  local buildableIndex = 0

  for _, areaType in ipairs(typeArea.getTypes()) do
    buildablesFile:write(
      string.format(
        xmlCommandTemplate,
        areaType,
        buildableIndex
      )
    )

    buildableIndex = buildableIndex + 1
  end
end

local function dumpRules()
  log(string.format("Write XML buildable file to %s", outputFileName))

  buildablesFile = io.open(outputFileName, "a+")

  buildablesFile:write(header);

  for _, areaName in ipairs(typeAreaOrder) do
    dumpBuildablesForTypeArea(buildablesFile, areaName, typeAreas[areaName])
  end

  buildablesFile:write(footer);
end

local function main()
  os.remove(outputFileName)

  local _, err = pcall(dumpRules)

  if buildablesFile then
    buildablesFile:close()
  end

  if err then
    error(string.format("XML buildables dump failed: %s", err))
  end

  log(string.format("XML buildables dump complete, see results @ %s", outputFileName))
end

main()

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

local typeAreas = Nco.Info.getTypeNames()

local outputFileName = "CNCModGameCommands.xml"
local buildablesFile = nil
local lastBuildableIndex = 0

local function dumpBuildablesForTypeArea(buildablesFile, typeAreaName, typeArea)
  buildablesFile:write(string.format("      <!-- %s -->\n", typeAreaName))

  local buildableIndex = typeAreaName == "Units" and lastBuildableIndex or 0

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

  lastBuildableIndex = buildableIndex
end

local function dumpRules()
  Nco.Utils.log("Write XML buildable file to %s", outputFileName)

  buildablesFile = io.open(outputFileName, "a+")

  buildablesFile:write(header);

  for _, areaName in ipairs(typeAreas) do
    dumpBuildablesForTypeArea(buildablesFile, areaName, Nco[areaName])
  end

  buildablesFile:write(footer);
end

local function main()
  os.remove(outputFileName)

  local oldLogLevel = Nco.Utils.getLogLevel()
  Nco.Utils.setLogLevel("info");

  local _, err = pcall(dumpRules)

  if buildablesFile then
    buildablesFile:close()
  end

  Nco.Utils.setLogLevel(oldLogLevel);

  if err then
    error(string.format("XML buildables dump failed: %s", err))
  end

  Nco.Utils.log("XML buildables dump complete, see results @ %s", outputFileName)
end

main()

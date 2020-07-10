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

local outputFileName = "RULES-DEFAULT.INI"
local rulesFile = nil

local function dumpRulesForTypeArea(rulesFile, typeAreaName, typeArea)
  rulesFile:write(string.format("## %s\n", typeAreaName))

  for _, areaType in ipairs(typeArea.getTypes()) do
    local friendlyName = typeArea.getRuleValue(areaType, "FriendlyName")

    rulesFile:write(string.format("; %s\n", friendlyName))
    rulesFile:write(string.format("[%s]\n", areaType))

    for _, ruleName in ipairs(typeArea.getRuleNames()) do
      local ruleValue = typeArea.getRuleValue(areaType, ruleName)

      rulesFile:write(
          string.format(
          "%s=%s\n",
          ruleName,
          tostring(ruleValue):gsub(".0$", "")
          )
      )
    end

    rulesFile:write("\n")
  end
end

local function dumpRules()
  log(string.format("Dumping rules ini to %s", outputFileName))

  if not os.execute(string.format("copy RULES-BASE.INI %s", outputFileName)) then
    error("Failed to copy RULES-BASE.INI")
  end

  rulesFile = io.open(outputFileName, "a+")

  for _, areaName in ipairs(typeAreaOrder) do
    dumpRulesForTypeArea(rulesFile, areaName, typeAreas[areaName])
  end
end

local function main()
  os.remove(outputFileName)

  setLogLevel("info");

  local _, err = pcall(dumpRules)

  if rulesFile then
    rulesFile:close()
  end

  setLogLevel("debug");

  if err then
    error(string.format("Lua rules dump failed: %s", err))
  end

  log(string.format("Rules ini dump complete, see results @ %s", outputFileName))
end

main()

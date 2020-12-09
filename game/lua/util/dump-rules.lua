local typeAreas =
{
  "Warheads",
  "Bullets",
  "Weapons",
  "Buildings",
  "Infantry",
  "Units",
  "Aircraft"
}

local outputFileName = "RULES-DEFAULT.INI"
local rulesFile = nil

local function dumpRulesForTypeArea(rulesFile, typeAreaName, typeArea)
  rulesFile:write(string.format("## %s\n", typeAreaName))
  rulesFile:write("\n")

  rulesFile:write(string.format("[%s]\n", typeAreaName))
  idx=1

  Nco.Utils.log("Creating list %s", typeAreaName)

  for _, areaType in ipairs(typeArea.getTypes()) do
    local isModType = typeArea.getRule(areaType, "IsModType")

    if not isModType then
      rulesFile:write(string.format("%s=%s\n",tostring(idx),areaType))

      idx = idx+1
    end
  end

  rulesFile:write("\n")

  for _, areaType in ipairs(typeArea.getTypes()) do
    Nco.Utils.log("Checking areaType %s", areaType)

    local friendlyName = typeArea.getRule(areaType, "FriendlyName")
    local isModType = typeArea.getRule(areaType, "IsModType")

    if isModType then
      goto areaType
    end

    rulesFile:write(string.format("; %s\n", friendlyName))
    rulesFile:write(string.format("[%s]\n", areaType))

    Nco.Utils.log("[%s]", areaType)

    for _, ruleName in ipairs(typeArea.getRuleNames()) do
      if ruleName == "IsModType" or ruleName == "Owner" or ruleName == "BaseType" or ruleName == "Image" then
        goto areaRule
      end

      local ruleValue = typeArea.getRule(areaType, ruleName)
      local postfix = ""

      if ruleName == "FriendlyName" and ruleValue:find("'") then
        postfix = "\n;'"
      end

      rulesFile:write(
        string.format(
          "%s=%s%s\n",
          ruleName,
          ruleValue,
          postfix
        )
      )

      ::areaRule::
    end

    rulesFile:write("\n")

    ::areaType::
  end
end

local function dumpRules()
  Nco.Utils.log("Dumping rules ini to %s", outputFileName)

  if not os.execute(string.format("copy RULES-BASE.INI %s", outputFileName)) then
    error("Failed to copy RULES-BASE.INI")
  end

  rulesFile = io.open(outputFileName, "a+")

  for _, areaName in ipairs(typeAreas) do
    Nco.Utils.log("Dumping %s", areaName)
    dumpRulesForTypeArea(rulesFile, areaName, Nco[areaName])
  end
end

local function main()
  os.remove(outputFileName)

  Nco.Utils.setLogLevel("info");

  local _, err = pcall(dumpRules)

  if rulesFile then
    rulesFile:close()
  end

  Nco.Utils.setLogLevel("debug");

  if err then
    error(string.format("Lua rules dump failed: %s", err))
  end

  Nco.Utils.log("Rules ini dump complete, see results @ %s", outputFileName)
end

main()

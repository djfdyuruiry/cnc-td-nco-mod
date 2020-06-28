function main()
  local typeAreas =
  {
    infantry = {
      getTypes = getInfantryTypes,
      getRuleNames = getInfantryRuleNames,
      getRuleValue = getInfantryRule,
      rules = {}
    },
    unit = {
      getTypes = getUnitTypes,
      getRuleNames = getUnitRuleNames,
      getRuleValue = getUnitRule,
      rules = {}
    },
    aircraft = {
      getTypes = getAircraftTypes,
      getRuleNames = getAircraftRuleNames,
      getRuleValue = getAircraftRule,
      rules = {}
    },
    building = {
      getTypes = getBuildingTypes,
      getRuleNames = getBuildingRuleNames,
      getRuleValue = getBuildingRule,
      rules = {}
    }
  }

  local outputFile = "RULES-DUMP.INI"

  os.remove(outputFile)

  local rulesFile = io.open(outputFile, "w")

  log(string.format("Dumping rules ini to %s", outputFile))

  rulesFile:write([[
###############
## rules.ini ##
###############]])

  rulesFile:write("\n\n")

  for typeAreaName, typeArea in pairs(typeAreas) do
    rulesFile:write(string.format([[
###############
## %s ##
###############]], typeAreaName))

    rulesFile:write("\n\n")
  
    for _, areaType in ipairs(typeArea.getTypes()) do
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

  rulesFile:close()

  log(string.format("Rules ini dump complete, see results @ %s", outputFile))
end

main()

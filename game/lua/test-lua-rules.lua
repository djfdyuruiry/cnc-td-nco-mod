function main()
  local typeAreas =
  {
    infantry = {
      getTypes = getInfantryTypes,
      getRuleNames = getInfantryRuleNames,
      getRuleValue = getInfantryRule,
      setRuleValue = setInfantryRule,
      rules = {}
    },
    unit = {
      getTypes = getUnitTypes,
      getRuleNames = getUnitRuleNames,
      getRuleValue = getUnitRule,
      setRuleValue = setUnitRule,
      rules = {}
    },
    aircraft = {
      getTypes = getAircraftTypes,
      getRuleNames = getAircraftRuleNames,
      getRuleValue = getAircraftRule,
      setRuleValue = setAircraftRule,
      rules = {}
    },
    building = {
      getTypes = getBuildingTypes,
      getRuleNames = getBuildingRuleNames,
      getRuleValue = getBuildingRule,
      setRuleValue = setBuildingRule,
      rules = {}
    }
  }

  local oldLogLevel = getLogLevel()
  setLogLevel("info");

  log("Testing Lua API get/set rules")

  local areasProcessed = 0
  local typesProcessed = 0
  local rulesRead = 0
  local rulesWritten = 0

  local status, err = pcall(function()
    for area, typeArea in pairs(typeAreas) do

      for _, areaType in ipairs(typeArea.getTypes()) do

        for _, ruleName in ipairs(typeArea.getRuleNames()) do

          local ruleValue = typeArea.getRuleValue(areaType, ruleName)

          rulesRead = rulesRead + 1

          if ruleName == "FriendlyName" then
              goto next
          end

          typeArea.setRuleValue(areaType, ruleName, ruleValue)

          rulesWritten = rulesWritten + 1

          ::next::
        end

        typesProcessed = typesProcessed + 1
      end
      
      areasProcessed = areasProcessed + 1
    end
  end)

  setLogLevel(oldLogLevel);

  log(
    string.format(
      [[Test Lua API rules result: %s -

  Areas processed: %d
  Types processed: %d
  Rules read: %d
  Rules written: %d
]],
      status and "PASS" or "FAIL",
      areasProcessed,
      typesProcessed,
      rulesRead,
      rulesWritten
    )
  )

  if not status then
    error(err)
  end
end

main()

local typeAreas =
  {
    weapons = {
      getTypes = getWeaponTypes,
      getRuleNames = getWeaponRuleNames,
      getRuleValue = getWeaponRule,
      setRuleValue = setWeaponRule
    },
    warheads = {
      getTypes = getWarheadTypes,
      getRuleNames = getWarheadRuleNames,
      getRuleValue = getWarheadRule,
      setRuleValue = setWarheadRule
    },
    infantry = {
      getTypes = getInfantryTypes,
      getRuleNames = getInfantryRuleNames,
      getRuleValue = getInfantryRule,
      setRuleValue = setInfantryRule
    },
    unit = {
      getTypes = getUnitTypes,
      getRuleNames = getUnitRuleNames,
      getRuleValue = getUnitRule,
      setRuleValue = setUnitRule
    },
    aircraft = {
      getTypes = getAircraftTypes,
      getRuleNames = getAircraftRuleNames,
      getRuleValue = getAircraftRule,
      setRuleValue = setAircraftRule
    },
    building = {
      getTypes = getBuildingTypes,
      getRuleNames = getBuildingRuleNames,
      getRuleValue = getBuildingRule,
      setRuleValue = setBuildingRule
    }
  }

function main()
  local oldLogLevel = getLogLevel()
  setLogLevel("info");

  log(">Testing Lua API get/set rules")

  local areasProcessed = 0
  local typesProcessed = 0
  local rulesRead = 0
  local rulesWritten = 0
  local gameRulesRead = 0
  local gameRulesWritten = 0
  local validationErrors = 0
  local gameRuleValidationErrors = 0

  local validationFailed = false

  local status, err = pcall(function()
    log(">>Testing get/set game rules")

    for _, gameRule in ipairs(getGameRuleNames()) do
        log(string.format("Game rule: %s", gameRule))

        local ruleValue = getGameRule(gameRule)

        gameRulesRead = gameRulesRead + 1

        setGameRule(gameRule, ruleValue)

        gameRulesWritten = gameRulesWritten + 1

        if ruleValue ~= getGameRule(gameRule) then
          showError(string.format("Validation of game rule value failed %s", gameRule))

          validationFailed = true
          gameRuleValidationErrors = gameRuleValidationErrors + 1
        end
    end

    for area, typeArea in pairs(typeAreas) do
      log(string.format(">>Testing %s get/set rules", area))

      for _, areaType in ipairs(typeArea.getTypes()) do
        log(string.format(">>>Testing type %s get/set rules", areaType))

        for _, ruleName in ipairs(typeArea.getRuleNames()) do
          local ruleValue = typeArea.getRuleValue(areaType, ruleName)

          rulesRead = rulesRead + 1

          typeArea.setRuleValue(areaType, ruleName, ruleValue)

          rulesWritten = rulesWritten + 1

          if ruleValue ~= typeArea.getRuleValue(areaType, ruleName) then
            showError(string.format("Validation of type rule value failed %s -> %s", areaType, ruleName))

            validationFailed = true
            validationErrors = validationErrors + 1
          end
        end

        typesProcessed = typesProcessed + 1
      end

      areasProcessed = areasProcessed + 1
    end
  end)

  status  = status and not validationFailed

  setLogLevel(oldLogLevel);

  log(
    string.format(
      [[Test Lua API rules result: %s

  Areas processed: %d
  Types processed: %d
  Rules read: %d
  Rules written: %d

  Game rules read: %d
  Game rules written: %d

  Validation errors: %d
]],
      status and "PASS" or "FAIL",
      areasProcessed,
      typesProcessed,
      rulesRead,
      rulesWritten,
      gameRulesRead,
      gameRulesWritten,
      validationErrors
    )
  )

  if not status then
    error(err)
  end
end

main()

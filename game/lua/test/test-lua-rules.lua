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

function main()
  local oldLogLevel = Nco.Utils.getLogLevel()
  Nco.Utils.setLogLevel("info");

  Nco.Utils.log(">Testing Lua API get/set rules")

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
    Nco.Utils.log(">>Testing get/set game rules")

    for _, gameRule in ipairs(Nco.Game.getRuleNames()) do
        Nco.Utils.log("Game rule: %s", gameRule)

        local ruleValue = Nco.Game.getRule(gameRule)

        gameRulesRead = gameRulesRead + 1

        Nco.Game.setRule(gameRule, ruleValue)

        gameRulesWritten = gameRulesWritten + 1

        local newRuleValue = Nco.Game.getRule(gameRule)

        if newRuleValue ~= ruleValue then
          showError("Validation of game rule value failed %s\nExpected: %s - Got: %s", gameRule, tostring(ruleValue), tostring(newRuleValue))

          validationFailed = true
          gameRuleValidationErrors = gameRuleValidationErrors + 1
        end
    end

    for _, area in ipairs(typeAreas) do
      Nco.Utils.log(">>Testing %s get/set rules", area)

      local typeArea = Nco[area]

      for _, areaType in ipairs(typeArea.getTypes()) do
        Nco.Utils.log(">>>Testing type %s get/set rules", areaType)

        for _, ruleName in ipairs(typeArea.getRuleNames()) do
          local ruleValue = typeArea.getRule(areaType, ruleName)

          rulesRead = rulesRead + 1

          if ruleValue == nil then
            log (">>>WARNING: Rule '%s' returned nil, skipping set rule validation test", ruleName)

            goto areaRule
          end

          typeArea.setRule(areaType, ruleName, ruleValue)

          rulesWritten = rulesWritten + 1

          local newRuleValue = typeArea.getRule(areaType, ruleName) 

          if newRuleValue ~= ruleValue then
            showError("Validation of type rule value failed %s -> %s\nExpected: %s - Got: %s", areaType, ruleName, tostring(ruleValue), tostring(newRuleValue))

            validationFailed = true
            validationErrors = validationErrors + 1
          end

          ::areaRule::
        end

        typesProcessed = typesProcessed + 1
      end

      areasProcessed = areasProcessed + 1
    end
  end)

  status  = status and not validationFailed

  Nco.Utils.setLogLevel(oldLogLevel);

  Nco.Utils.log(
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

  if not status then
    error(err)
  end
end

main()

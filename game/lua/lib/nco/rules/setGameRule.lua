local setGameRule = function (ruleName, ruleValue)
  local gameRules = getGameRuleNames()
  local gameRuleTypes = getGameRuleTypeNames()

  for idx, rule in ipairs(gameRules) do
    if rule == ruleName then
      if gameRuleTypes[idx] == 'string' then
        setStringGameRule(rule, ruleValue)
      elseif gameRuleTypes[idx] == 'int' then
        setIntGameRule(rule, ruleValue)
      elseif gameRuleTypes[idx] == 'double' then
        setDoubleGameRule(rule, ruleValue)
      else
        setBoolGameRule(rule, ruleValue)
      end
    end
  end
end

return setGameRule

local setGameRule = function (ruleName, ruleValue)
  if type(ruleName) ~= "string" then
    error("ruleName passed to setGameRule was not a string")
  elseif ruleName == "" then
    error("ruleName passed to setGameRule was empty")
  elseif ruleValue == nil then
    error("ruleValue passed to setGameRule was nil")
  elseif tostring(ruleName) == "" then
    error("ruleValue passed to setGameRule was empty")
  end

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

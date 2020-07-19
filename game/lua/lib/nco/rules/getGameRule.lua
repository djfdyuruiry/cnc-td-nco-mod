local getGameRule = function (ruleName)
  if type(ruleName) ~= "string" then
    error("ruleName passed to getGameRule was not a string")
  elseif ruleName == "" then
    error("ruleName passed to getGameRule was empty")
  end

  local gameRules = getGameRuleNames()
  local gameRuleTypes = getGameRuleTypeNames()

  for idx, rule in ipairs(gameRules) do
    if rule == ruleName then
      local value

      if gameRuleTypes[idx] == 'string' then
        value = getStringGameRule(rule)
      elseif gameRuleTypes[idx] == 'int' then
        value = getIntGameRule(rule)
      elseif gameRuleTypes[idx] == 'double' then
        value = getDoubleGameRule(rule)
      else
        value = getBoolGameRule(rule)
      end

      return value
    end
  end

  error(string.format("ruleName passed to getGameRule was not a valid game rule: %s", ruleName))
end

return getGameRule

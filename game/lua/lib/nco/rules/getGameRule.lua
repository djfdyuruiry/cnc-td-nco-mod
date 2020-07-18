local getGameRule = function (ruleName)
  if ruleName == nil then
    -- TODO: throw error on incorrect type
    return nil
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

  return nil
end

return getGameRule

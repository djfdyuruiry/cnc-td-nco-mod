local function TypeInstanceRuleProxy(getRule, setRule, ruleName)
  return setmetatable(
    {},
    {
      __call = function(_, ruleValue)
        if ruleValue == nil then
          return getRule(ruleName)
        end

        return setRule(ruleName, ruleValue)
      end
    }
  )
end

local function TypeInstanceProxy(typeApi, instanceName)  
  local function getRule(...)
    return typeApi.getRule(instanceName, ...)
  end

  local function setRule(...)
    return typeApi.setRule(instanceName, ...)
  end

  return setmetatable(
    {
      getRule = getRule,
      setRule = setRule
    },
    {
      __index = function(_, ruleName)
        return TypeInstanceRuleProxy(getRule, setRule, ruleName)
      end
    }
  )
end

local function TypeApiProxy(typeApi)
  return setmetatable(
    typeApi,
    {
      __index = function(_, typeInstance)
        return TypeInstanceProxy(typeApi, typeInstance)
      end
    }
  )
end

return TypeApiProxy

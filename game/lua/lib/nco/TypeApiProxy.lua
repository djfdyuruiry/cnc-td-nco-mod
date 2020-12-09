local TypeInstanceRuleProxy = function(typeApi, instanceName, ruleName)
  return setmetatable(
    {},
    {
      __call = function(_, ruleValue)
        if ruleValue == nil then
          return typeApi.getRule(instanceName, ruleName)
        end

        return typeApi.setRule(instanceName, ruleName, ruleValue)
      end
    }
  )
end

local TypeInstanceProxy = function(typeApi, instanceName)
  return setmetatable(
    {
      getRule = function(...)
        return typeApi.getRule(instanceName, ...)
      end,
      setRule = function(...)
        return typeApi.setRule(instanceName, ...)
      end
    },
    {
      __index = function(_, ruleName)
        return TypeInstanceRuleProxy(typeApi, instanceName, ruleName)
      end
    }
  )
end

local TypeApiProxy = function(typeApi)
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

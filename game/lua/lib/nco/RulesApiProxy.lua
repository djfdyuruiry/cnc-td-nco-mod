local function RulesSectionRuleProxy(getRule, setRule, ruleName)
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

local function RulesSectionProxy(api, sectionName)  
  local function getRule(...)
    return api.getRule(sectionName, ...)
  end

  local function setRule(...)
    return api.setRule(sectionName, ...)
  end

  return setmetatable(
    {
      getRule = getRule,
      setRule = setRule
    },
    {
      __index = function(_, ruleName)
        return RulesSectionRuleProxy(getRule, setRule, ruleName)
      end
    }
  )
end

local function RulesApiProxy(api)
  return setmetatable(
    api,
    {
      __index = function(_, sectionName)
        return RulesSectionProxy(api, sectionName)
      end
    }
  )
end

return RulesApiProxy

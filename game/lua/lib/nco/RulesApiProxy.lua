local function RulesSectionRuleProxy(getRule, setRule, ruleName, sectionName)
  return setmetatable(
    {
      name = ruleName,
      sectionName = sectionName
    },
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

  local function getRuleNames(...)
    return Nco.Rules.getRuleNames(sectionName, ...)
  end

  return setmetatable(
    {
      name = sectionName,
      getRule = getRule,
      setRule = setRule,
      getRuleNames = getRuleNames
    },
    {
      __index = function(_, ruleName)
        return RulesSectionRuleProxy(getRule, setRule, ruleName, sectionName)
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

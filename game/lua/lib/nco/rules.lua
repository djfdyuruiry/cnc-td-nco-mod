local ruleSections =
{
  "Enhancements",
  "Game",
  "AirStrike",
  "IonCannon",
  "NuclearStrike"
}

local buildRuleFunctions = function()
  local ruleFunctions = {}

  for _, section in ipairs(ruleSections) do
    local getFunction = ("get%sRule"):format(section)
    local setFunction = ("set%sRule"):format(section)
    local upperSection = section:upper()

    ruleFunctions[getFunction] = function(...)
      if #table.pack(...) < 1 then
        error(("%s requires exactly one argument"):format(getFunction))
      end

      return getRuleValue(upperSection, ...)
    end

    ruleFunctions[setFunction] = function(...)
      if #table.pack(...) < 2 then
        error(("%s requires exactly two arguments"):format(setFunction))
      end

      return setRuleValue(upperSection, ...)
    end
  end

  return ruleFunctions
end

return (buildRuleFunctions())

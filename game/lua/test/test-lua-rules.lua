local areasProcessed = 0
local typesProcessed = 0
local sectionsProcessed = 0
local rulesRead = 0
local rulesWritten = 0
local validationErrors = 0

local validationFailed = false

local sectionsToTest =
{
  "Nco",
  "Enhancements",
  "Mods",
  "Game",
  "Easy",
  "Normal",
  "Hard",
  "AI",
  "IQ",
  "AirStrike",
  "IonCannon",
  "NuclearStrike"
}

local function testRule(rule)
  local ruleValue = rule()

  rulesRead = rulesRead + 1

  rule(ruleValue)

  rulesWritten = rulesWritten + 1

  local newRuleValue = rule()

  if newRuleValue == ruleValue then
    return
  end

  showError(
    "Validation of rule value failed: %s -> %s\nExpected: %s - Got: %s",
    rule.sectionName,
    rule.name,
    tostring(ruleValue),
    tostring(newRuleValue)
  )

  validationFailed = true
  ruleValidationErrors = ruleValidationErrors + 1
end

local function testSectionRules(section)
  Nco.Utils.log(">> Testing get/set '%s' rules", section.name)

  for _, ruleName in ipairs(section.getRuleNames()) do
    testRule(section[ruleName])
  end

  sectionsProcessed = sectionsProcessed + 1
end

local function testTypeRules()
  for _, area in ipairs(Nco.Info.getTypeNames()) do
    Nco.Utils.log(">> Testing rules for '%s' types", area)

    local typeArea = Nco[area]

    for _, areaType in ipairs(typeArea.getTypes()) do
      testSectionRules(typeArea[areaType])
      
      typesProcessed = typesProcessed + 1
    end

    areasProcessed = areasProcessed + 1
  end
end

local function testGenericRules()
  for _, sectionName in ipairs(sectionsToTest) do
    testSectionRules(Nco.Rules[sectionName])
  end
end

local function main()
  local oldLogLevel = Nco.Utils.getLogLevel()
  Nco.Utils.setLogLevel("info");

  Nco.Utils.log("> Testing Lua API get/set rules")

  local execStatus, err = pcall(function()
    testGenericRules()
    testTypeRules()
  end)

  local testStatus = execStatus and not validationFailed

  Nco.Utils.log(
    [[Test Lua API rules result: %s

  Sections Processed: %d
  Rules read: %d
  Rules written: %d

  Type API Areas processed: %d
  Types processed: %d

  Validation errors: %d
]],
    testStatus and "PASS" or "FAIL",
    sectionsProcessed,
    rulesRead,
    rulesWritten,
    areasProcessed,
    typesProcessed,
    validationErrors
  )

  Nco.Utils.setLogLevel(oldLogLevel);

  if not testStatus then
    error(err)
  end
end

main()

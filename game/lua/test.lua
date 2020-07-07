function printMinigunnerWeapon()
  local weapon = getInfantryRule("E1", "PrimaryWeapon")

  log(string.format("E1 primary weapon is: %s", weapon))
end

function testRules (isScenarioFour)
  printMinigunnerWeapon()

  local scenarioNumber = 1;
  local buildLevel = 1;

  setInfantryRule("E1", "CanCapture", true)

  setInfantryRule("E3", "ScenarioLevel", scenarioNumber)
  setInfantryRule("E3", "BuildLevel", buildLevel)
  
  setInfantryRule("E6", "ScenarioLevel", scenarioNumber)
  setInfantryRule("E6", "BuildLevel", buildLevel)
  
  setInfantryRule("RMBO", "Buildable", true)
  setInfantryRule("RMBO", "ScenarioLevel", scenarioNumber)
  setInfantryRule("RMBO", "BuildLevel", buildLevel)
  setInfantryRule("RMBO", "PrimaryWeapon", "obelisk_laser")
  setInfantryRule("RMBO", "Prerequisite", "none")
  
  setUnitRule("JEEP", "Speed", 100)
  setUnitRule("JEEP", "CanCrushInfantry", true)
  
  setUnitRule("FTNK", "ScenarioLevel", scenarioNumber)
  setUnitRule("FTNK", "BuildLevel", buildLevel)
  
  setUnitRule("MCV", "CanCloak", true)

  setUnitRule("BOAT", "CanCloak", true)
  
  setUnitRule("HTNK", "ScenarioLevel", scenarioNumber)
  setUnitRule("HTNK", "BuildLevel", buildLevel)
  setUnitRule("HTNK", "Cost", 10)

  setBuildingRule("BIO", "Buildable", true)
  setBuildingRule("BIO", "ScenarioLevel", scenarioNumber)
  setBuildingRule("BIO", "BuildLevel", buildLevel)
  setBuildingRule("BIO", "Prerequisite", "none")

  setBuildingRule("NUKE", "Bibbed", false)
  
  setBuildingRule("GUN", "Captureable", true) 

  setInfantryRule("E2", "PrimaryWeapon", "TOWTWO")

  printMinigunnerWeapon()
end

onScenarioStart(function(name)
  log(string.format("Scenario received: %s", name))

  testRules(name == "SCG04EA")
end)

onSaveLoad(function(house, scenario)
  log(string.format("Save game loading for %s scenario %d", house, scenario))

  testRules(scenario == 4 and house == "GOODGUY")
end)

function printMinigunnerWeapon()
  local weapon = getInfantryRule("E1", "PrimaryWeapon")

  log(string.format("E1 primary weapon is: %s", weapon))
end

onScenarioStart(function (house, scenarioNumber, buildLevel)
  log(string.format("Scenario received: %s %d", house, scenarioNumber))
  log(string.format("Scenario build level: %d", buildLevel))

  printMinigunnerWeapon()
 
  setInfantryRule("E1", "CanCapture", true)

  setInfantryRule("E3", "ScenarioLevel", scenarioNumber)
  setInfantryRule("E3", "BuildLevel", buildLevel)
  
  setInfantryRule("E6", "ScenarioLevel", scenarioNumber)
  setInfantryRule("E6", "BuildLevel", buildLevel)
  
  setUnitRule("JEEP", "Speed", 100)
  
  setUnitRule("FTNK", "ScenarioLevel", scenarioNumber)
  setUnitRule("FTNK", "BuildLevel", buildLevel)
  
  setUnitRule("MCV", "CanCloak", true)
 
  setUnitRule("BOAT", "CanCloak", true)
 
  setBuildingRule("BIO", "Buildable", true)
  setBuildingRule("BIO", "ScenarioLevel", scenarioNumber)
  setBuildingRule("BIO", "BuildLevel", buildLevel)
  setBuildingRule("BIO", "Prerequisite", "none")
  
  setBuildingRule("HAND", "FactoryType", "Unit")

  setBuildingRule("NUKE", "Bibbed", false)

  if scenarioNumber ~= 4 then
    return
  end

  setInfantryRule("E1", "FraidyCat", true)
  setInfantryRule("E1", "AvoidsTiberium", true)
  setInfantryRule("E1", "Crawling", false)

  setInfantryRule("E1", "PrimaryWeapon", "obelisk_laser")

  printMinigunnerWeapon()
end)

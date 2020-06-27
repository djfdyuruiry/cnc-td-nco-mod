function printMinigunnerWeapon()
  local weapon = getInfantryRule("E1", "PrimaryWeapon")

  log(string.format("E1 primary weapon is: %s", weapon))
end

onScenarioStart(function (scenarioNumber)
  log(string.format("Scenario number received: %d", scenarioNumber))

  printMinigunnerWeapon()
  
  setInfantryRule("E3", "ScenarioLevel", 1)
  setInfantryRule("E3", "BuildLevel", 1)
  
  setInfantryRule("E6", "ScenarioLevel", 1)
  setInfantryRule("E6", "BuildLevel", 1)

  if scenarioNumber ~= 4 then
    return
  end

  setInfantryRule("E1", "FraidyCat", true)
  setInfantryRule("E1", "AvoidsTiberium", true)
  setInfantryRule("E1", "Crawling", false)

  setInfantryRule("E1", "PrimaryWeapon", "obelisk_laser")

  printMinigunnerWeapon()
end)

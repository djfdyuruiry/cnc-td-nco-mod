function printMinigunnerWeapon()
  local weapon = Nco.Infantry.getRule("E1", "PrimaryWeapon")

  Nco.Utils.log("E1 primary weapon is: %s", weapon)
end

function testRules (isScenarioFour)
  printMinigunnerWeapon()

  local scenarioNumber = 1;
  local buildLevel = 1;

  Nco.Infantry.setRule("E1", "CanCapture", true)

  Nco.Infantry.setRule("E3", "ScenarioLevel", scenarioNumber)
  Nco.Infantry.setRule("E3", "BuildLevel", buildLevel)

  Nco.Infantry.setRule("E6", "ScenarioLevel", scenarioNumber)
  Nco.Infantry.setRule("E6", "BuildLevel", buildLevel)

  Nco.Infantry.setRule("RMBO", "Buildable", true)
  Nco.Infantry.setRule("RMBO", "ScenarioLevel", scenarioNumber)
  Nco.Infantry.setRule("RMBO", "BuildLevel", buildLevel)
  Nco.Infantry.setRule("RMBO", "PrimaryWeapon", "obelisk_laser")
  Nco.Infantry.setRule("RMBO", "Prerequisite", "none")

  Nco.Units.setRule("JEEP", "Speed", 100)
  Nco.Units.setRule("JEEP", "CanCrushInfantry", true)
  Nco.Units.setRule("JEEP", "CanTransportInfantry", true)
  Nco.Units.setRule("JEEP", "TransportCapacity", 10)

  Nco.Units.setRule("FTNK", "ScenarioLevel", scenarioNumber)
  Nco.Units.setRule("FTNK", "BuildLevel", buildLevel)

  Nco.Units.setRule("MCV", "CanCloak", true)

  Nco.Units.setRule("BOAT", "CanCloak", true)

  Nco.Units.setRule("HTNK", "ScenarioLevel", scenarioNumber)
  Nco.Units.setRule("HTNK", "BuildLevel", buildLevel)
  Nco.Units.setRule("HTNK", "Cost", 10)

  Nco.Buildings.setRule("BIO", "Buildable", true)
  Nco.Buildings.setRule("BIO", "ScenarioLevel", scenarioNumber)
  Nco.Buildings.setRule("BIO", "BuildLevel", buildLevel)
  Nco.Buildings.setRule("BIO", "Prerequisite", "none")

  Nco.Buildings.setRule("NUKE", "Bibbed", false)

  Nco.Buildings.setRule("GUN", "Captureable", true) 

  Nco.Infantry.setRule("E2", "PrimaryWeapon", "TOWTWO")

  printMinigunnerWeapon()
end

Nco.Events.onScenarioStart(function(name)
  Nco.Utils.log("Scenario received: %s", name)

  testRules()
end)

Nco.Events.onSaveLoad(function(house, scenario)
  Nco.Utils.log("Save game loading for %s scenario %d", house, scenario)

  testRules()
end)

local lastMessageTime = 0

Nco.Events.onGameTick(function()
  local now = Nco.Utils.getNowInEpochMillis()

  if now - lastMessageTime >= 30000 then -- every 30 seconds
    Nco.Game.showGameMessage(string.format("[%d] Peace through power!", now), 3)

    lastMessageTime = now
  end
end)

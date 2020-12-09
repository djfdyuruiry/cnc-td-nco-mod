function printMinigunnerWeapon()
  local weapon = Nco.Infantry.getRule("E1", "PrimaryWeapon")

  Nco.Utils.log("E1 primary weapon is: %s", weapon)
end

function testRules ()
  printMinigunnerWeapon()

  local scenarioNumber = 1;
  local buildLevel = 1;

  Nco.Infantry.E1.CanCapture(true)

  with(Nco.Infantry.E3, function()
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
  end)

  with(Nco.Infantry.E6, function()
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
  end)

  with(Nco.Infantry.RMBO, function()
    Buildable(true)
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
    PrimaryWeapon("obelisk_laser")
    Prerequisite("none")
  end)

  with(Nco.Units.JEEP, function()
    Speed(100)
    CanCrushInfantry(true)
    CanTransportInfantry(true)
    TransportCapacity(10)
  end)

  with(Nco.Units.FTNK, function()
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
  end)

  Nco.Units.MCV.CanCloak(true)

  Nco.Units.BOAT.CanCloak(true)

  with(Nco.Units.HTNK, function()
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
    Cost(10)
  end)

  with(Nco.Buildings.BIO, function()
    Buildable(true)
    ScenarioLevel(scenarioNumber)
    BuildLevel(buildLevel)
    Prerequisite("none")
  end)

  Nco.Buildings.NUKE.Bibbed(false)

  Nco.Buildings.GUN.Captureable(true) 

  Nco.Infantry.E2.PrimaryWeapon("TOWTWO")

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

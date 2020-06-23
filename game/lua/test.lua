function printMinigunnerWeapon()
    local weapon = getInfantryRule("E1", "PrimaryWeapon")

    log(string.format("E1 primary weapon is: %s", weapon))
end

onScenarioStart(function (scenarioNumber)
    log(string.format("Scenario number received: %d", scenarioNumber))

    printMinigunnerWeapon()

    if scenarioNumber ~= 4 then
      return
    end

    setInfantryRule("E1", "PrimaryWeapon", "obelisk_laser")

    printMinigunnerWeapon()
end)

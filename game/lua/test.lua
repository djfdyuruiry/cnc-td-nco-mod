onScenarioStart(function (scenarioNumber)
    log(string.format("Scenario number received: %d", scenarioNumber))

    local weapon = getInfantryRule("E1", "PrimaryWeapon")

    log(string.format("E1 primary weapon is currently: %s", weapon))

    setInfantryRule("E1", "PrimaryWeapon", "obelisk_laser")

    local newWeapon = getInfantryRule("E1", "PrimaryWeapon")

    log(string.format("E1 primary weapon is now: %s", newWeapon))
end)

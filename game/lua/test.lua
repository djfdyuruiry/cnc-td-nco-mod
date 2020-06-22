onScenarioStart(function (scenarioNumber)
    local file = io.open("debug.log", "a");

    log(string.format("Scenario number received: %d", scenarioNumber))
end)

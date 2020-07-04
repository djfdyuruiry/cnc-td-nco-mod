local ncoAndGameRules = [[### rules.ini for Tiberian Dawn
###
### full guide to editing this file: https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki/1.-RULES.INI-Full-Guide
###

[NCO]
LogLevel=error
EnableLuaScripts=true
LuaScripts=

## game rules
[Game]
EnableDebugLogging=false

; cheats
AllowBuildingAllForCurrentHouse=false
UnitsAreIndestructible=false
InfantryAutoScatters=false
GiveAttackersAnAdvantage=false
SpeedyBuilds=false

; misc
DefendAgainstFriendlyFire=false
TargetTrees=false
MvcRedeployable=false
SpawnVisceroids=false
VehiclesDoThreePointTurns=false
ShowBibsOnBuildings=false
ShowCivilianBuildingNames=false
TiberiumGrows=true
TiberiumSpreads=true
SlowTiberiumGrowthAndSpread=false
OnlyAllowNukeIfAllPartsHaveBeenCollected=true
OnlyAllowUsingOneNukePerScenario=true
HelipadsAndAircraftBoughtSeperately=false

; map
MaxBuildDistance=1
PreventBuildingInShroud=true
AllowBuildingBesideWalls=true

; house specific
HideTempleFromGdi=true
HideObeliskFromGdi=true
HideApcFromNod=true
HideRocketLauncherFromNod=true
HideHelipadFromNod=true
HideAdvancedCommCenterFromNod=true

; level specific
SetBuildLevelTo1InGdiScenario2=true
RenameTechCenterToPrisionInScenario3=true
HideBazookaFromGdiUntilSenario8=true
HideRocketLauncherFromGdiUntilScenario9=true
HideSandbagFromGdiUntilScenario9=true
AllowNodToBuildAdvancedPowerInScenario12=true

]]

function dumpRulesForTypeArea(rulesFile, typeAreaName, typeArea)
  rulesFile:write(string.format("## %s\n", typeAreaName))

  for _, areaType in ipairs(typeArea.getTypes()) do
    local friendlyName = typeArea.getRuleValue(areaType, "FriendlyName")

    rulesFile:write(string.format("; %s\n", friendlyName))
    rulesFile:write(string.format("[%s]\n", areaType))

    for _, ruleName in ipairs(typeArea.getRuleNames()) do
      if ruleName == "FriendlyName" then
          goto next
      end

      local ruleValue = typeArea.getRuleValue(areaType, ruleName)

      rulesFile:write(
          string.format(
          "%s=%s\n",
          ruleName,
          tostring(ruleValue):gsub(".0$", "")
          )
      )

      ::next::
    end

    rulesFile:write("\n")
  end
end

function main()
  local typeAreas =
  {
    infantry = {
      getTypes = getInfantryTypes,
      getRuleNames = getInfantryRuleNames,
      getRuleValue = getInfantryRule,
      rules = {}
    },
    unit = {
      getTypes = getUnitTypes,
      getRuleNames = getUnitRuleNames,
      getRuleValue = getUnitRule,
      rules = {}
    },
    aircraft = {
      getTypes = getAircraftTypes,
      getRuleNames = getAircraftRuleNames,
      getRuleValue = getAircraftRule,
      rules = {}
    },
    building = {
      getTypes = getBuildingTypes,
      getRuleNames = getBuildingRuleNames,
      getRuleValue = getBuildingRule,
      rules = {}
    }
  }

  local outputFile = "RULES-DUMP.INI"

  os.remove(outputFile)

  local rulesFile = io.open(outputFile, "w")

  log(string.format("Dumping rules ini to %s", outputFile))

  rulesFile:write(ncoAndGameRules)

  dumpRulesForTypeArea(rulesFile, "Infantry", typeAreas.infantry)
  dumpRulesForTypeArea(rulesFile, "Units", typeAreas.unit)
  dumpRulesForTypeArea(rulesFile, "Aircraft", typeAreas.aircraft)
  dumpRulesForTypeArea(rulesFile, "Buildings", typeAreas.building)

  rulesFile:close()

  log(string.format("Rules ini dump complete, see results @ %s", outputFile))
end

main()

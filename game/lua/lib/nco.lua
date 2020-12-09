local Utils = require("nco.Utils")
local Events = require("nco.EventBus")
local TypeApiProxy = require("nco.TypeApiProxy")

local typeAreas =
{
  "Warheads",
  "Bullets",
  "Weapons",
  "Buildings",
  "Infantry",
  "Units",
  "Aircraft"
}

local loadNativeTypeApiMixin = function()
  for _, typeArea in ipairs(typeAreas) do
    Nco[typeArea] = TypeApiProxy(Nco[typeArea])
  end
end

local loadNativeUtilApiMixin = function()
  -- overwrite print builtin to enable output
  _G.__print = _G.print
  _G.print = Utils.print

  -- inject native utils into existing C++ API
  for name, func in pairs(Utils) do
    if type(Nco.Utils[name]) ~= "function" then
      Nco.Utils[name] = func
    end
  end
end

local loadNativeApis = function()
  loadNativeUtilApiMixin()

  Nco.Events = Events()

  loadNativeTypeApiMixin()
end

loadNativeApis()

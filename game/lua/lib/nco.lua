local Utils = require("nco.Utils")
local Events = require("nco.EventBus")
local RulesApiProxy = require("nco.RulesApiProxy")

local function loadNativeApiMixins()
  Nco.Rules = RulesApiProxy(Nco.Rules)

  for _, type in ipairs(Nco.Info.getTypeNames()) do
    Nco[type] = RulesApiProxy(Nco[type])
  end
end

local function loadNativeUtilApiMixin()
  -- overwrite print builtin to enable output
  _G.__print = _G.print
  _G.print = Utils.print

  -- 'with' syntax sugar
  _G.with = Utils.with

  -- inject native utils into existing C++ API
  for name, func in pairs(Utils) do
    Nco.Utils[name] = Nco.Utils[name] and Nco.Utils[name] or func
  end
end

local function loadNativeApis()
  loadNativeUtilApiMixin()

  Nco.Events = Events()

  loadNativeApiMixins()
end

loadNativeApis()

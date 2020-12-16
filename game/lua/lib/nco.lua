local Events = require("nco.EventBus")
local LuaRepl = require("nco.LuaRepl")
local RulesApiProxy = require("nco.RulesApiProxy")
local Utils = require("nco.Utils")

local function loadNativeApiMixins()
  Nco.Rules = RulesApiProxy(Nco.Rules)

  for _, type in ipairs(Nco.Info.getTypeNames()) do
    Nco[type] = RulesApiProxy(Nco[type])
  end
end

local function loadNativeUtilApiMixin()
  -- overwrite print builtin to enable output
  _G.__print = _G.print
  _G.print = Utils.printString

  -- overwrite io read builtin to enable input
  io.__read = io.read
  io.read = Utils.readString

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
  Nco.LuaRepl = LuaRepl()

  loadNativeApiMixins()
end

loadNativeApis()

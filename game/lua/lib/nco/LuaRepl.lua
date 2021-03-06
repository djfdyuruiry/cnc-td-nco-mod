local function LuaRepl()
  local writeReplOutput
  local running = false
  local externalExitRequested = false

  local function writeTableEntry(key, value, depth)
    if type(key) == "string" then
      writeReplOutput(key, depth + 1, true)

      io.write(" = ")
    end

    local valueDepth = 0

    if type(value) == "table" then
      valueDepth = depth + 1

      io.write("\n")
    end

    writeReplOutput(value, valueDepth)
  end

  local function writeTableOutput(output, depth)
    writeReplOutput("{", depth, true)

    local firstVal = false

    for k, v in pairs(output) do
      if firstVal then
        io.write(",")
      end

      io.write("\n")

      writeTableEntry(k, v, depth)

      firstVal = true
    end

    io.write("\n")
    writeReplOutput("}", depth, true)
  end

  writeReplOutput = function(output, depth, outputPlainStrings)
    local outputType = type(output)

    if outputType == "table" then    
      writeTableOutput(output, depth)

      return
    end

    local indent = depth < 1 and "" or string.rep("  ", depth)

    io.write(indent)

    local outputString = tostring(output)

    if not outputPlainStrings then  
      if outputType == "string" then
        outputString = string.format([["%s"]], outputString)
      elseif outputType ~= "number" and outputType ~= "boolean" then
        outputString = string.format([[<%s>]], outputString)
      end
    end

    io.write(outputString)
  end

  local function evaluateReplInput(input)
    local evalFunc, evalError = load(input)

    if type(evalFunc) ~= "function" then
      writeReplOutput(string.format("Syntax Error: %s", evalError), 0)

      return
    end

    local execStatus, execErrOrReturnVal = pcall(evalFunc)
    local output = execErrOrReturnVal

    if not execStatus then
      output = string.format("Error: %s", tostring(execErrOrReturnVal))
    end

    writeReplOutput(output, 0)
  end

  local function repl()
    io.write("> ")

    local evalString = io.read("*l")

    if evalString == nil then
      return not externalExitRequested
    end

    local trimmedEvalString = evalString:gsub([[^%s*]], ""):gsub([[%s*$]], "")

    if trimmedEvalString == "" then
      return not externalExitRequested
    end

    if trimmedEvalString == "exit" then
      return false
    end

    if trimmedEvalString ~= "clear" then
      evaluateReplInput(evalString)
    else
      os.execute("cls")
    end

    io.write("\n")

    return not externalExitRequested
  end

  local function enter()
    if running then
      return
    end

    running = true

    Nco.Utils.toggleConsoleLog()

		print([[
=======================
  NCO Mod Lua Console  
=======================

Help: https://github.com/djfdyuruiry/cnc-td-nco-mod/wiki/09.-Lua-Scripting-API
]])

    while(repl()) do
    end

    Nco.Utils.toggleConsoleLog()

    running = false
    externalExitRequested = false
  end

  local function exit()
    if not running or externalExitRequested then
      return
    end

    externalExitRequested = true
  end

  return
  {
    enter = enter,
    exit = exit
  }
end

return LuaRepl

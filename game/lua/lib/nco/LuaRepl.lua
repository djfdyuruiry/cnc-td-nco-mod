local function LuaRepl()
  local writeReplOutput

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
    local output = execStatus and execErrOrReturnVal or string.format("Error: %s", tostring(execErrOrReturnVal))

    writeReplOutput(output, 0)
  end

  local function repl()
    io.write("# ")

    local evalString = tostring(io.read("*l"))
    local trimmedEvalString = evalString:gsub([[^\s*]], ""):gsub([[\s*$]], "")

    if trimmedEvalString == "exit" then
      return false
    end

    if trimmedEvalString ~= "clear" then
      evaluateReplInput(evalString)
    else
      os.execute("cls")
    end

    io.write("\n")

    return true
  end

  local function enter()
    print("------------")
    print("  Lua REPL  ")
    print("------------")

    while(repl()) do
    end
  end

  return
  {
    enter = enter
  }
end

return LuaRepl

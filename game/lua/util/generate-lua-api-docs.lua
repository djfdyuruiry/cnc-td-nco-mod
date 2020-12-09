local header = [[This page details all the functions that are available in Lua scripts.

The unit/infantry/building names and rule names/values used in Lua can all be found [here](01.-RULES.INI-Full-Guide).

**Note: Parameter values are not case sensitive**

]]

local apiHeader = [[----

## %s

%s

]]

local functionTemplate = [[----

### `%s`

%s

```lua
-- TODO: example field
```

]]

local hasKeys = function(tbl)
  for k, v in pairs(tbl) do
      return true
  end

  return false
end

local main = function()
  local output = ""
  local outputFile = "lua-api.md"
  local apis = Nco.Reflection.getApis()

  local appendToOutput = function(format, ...)
    output = output .. string.format(format, ...) .. "\n"
  end

  print("Generating API markdown docs")
  print("")

  appendToOutput(header)

  -- TODO: output events api function (this is maintained in Lua not C++)

  print("> Building index")

  for apiName, api in pairs(apis) do
    appendToOutput([[- [%s](#%s)]], apiName, apiName:gsub(" ", "-"))

    for funcName, func in pairs(api.functions) do
      appendToOutput([[  - [%s](#%s)]], funcName, funcName)
    end
  end

  appendToOutput("")

  print(">Building API sections")

  for apiName, api in pairs(apis) do
    print("  > Building section for API: %s...", apiName)

    appendToOutput(apiHeader, apiName, api.description)

    for funcName, func in pairs(api.functions) do
      appendToOutput([[- [%s](#%s)]], funcName, funcName)
    end

    appendToOutput("")

    for funcName, func in pairs(api.functions) do
      print("    > Building section for API function: %s...", funcName)

      appendToOutput(functionTemplate, funcName, func.description)

      if hasKeys(func.parameters) then
        appendToOutput("Parameters:")

        for paramName, param in pairs(func.parameters) do
          appendToOutput("  - `%s` [%s] - %s", paramName, param.type, param.description)
        end
      else
        appendToOutput("Parameters: none")
      end

      appendToOutput("")

      if hasKeys(func.returnValues) then
        appendToOutput("Return value(s):")

        for valueName, returnValue in pairs(func.returnValues) do
          appendToOutput("  - `%s` [%s] - %s", valueName, returnValue.type, returnValue.description)
        end
      else
        appendToOutput("Return value(s): none")
      end

      appendToOutput("")
    end
  end

  print("Saving generated markdown to file: %s...", outputFile)

  local f = io.open(outputFile, "w")

  f:write(output)
  f:close()

  print("API docs output to file: %s", outputFile)
end

main()

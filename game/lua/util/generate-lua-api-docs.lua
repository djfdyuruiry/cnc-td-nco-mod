local header = [[This page details all the functions that are available in Lua scripts.

All APIs are found inside the `Nco` global table, so to use the `Units` API for example you would use `Nco.Units.doSomething(...)`.

The unit/infantry/building names and rule names/values used in Lua can all be found [here](01.-RULES.INI-Full-Guide).

**Note: Parameter values are not case sensitive**

]]

local apiHeader = [[----

## `Nco.{{apiName}}`

{{apiDescription}}

]]

local functionTemplate = [[----

### `{{functionName}}`

{{functionDescription}}

```lua
Nco.{{apiName}}.{{functionName}}({{exampleParameters}})
```
]]

local function renderTemplate(template, context)
  if type(template) ~= "string" then
    error("renderTemplate parameter 'template' must be a string")
  end

  if type(context) ~= "table" then
    error("renderTemplate parameter 'context' must be a table")
  end

  local result = template

  for variable, rawValue in pairs(context) do
    local value = rawValue

    if type(value) == "nil" then
      value = ""
    elseif type(val) == "function" then
      value = rawValue(context)
    end

    result = result:gsub(
      string.format("{{%s}}", variable),
      tostring(value)
    )
  end

  return result
end

local function generateExampleValueForParameter(name, info)
  if info.type == "any" then
    return [["value"]]
  elseif info.type == "number" then
    return "1"
  elseif info.type == "string" then
    return string.format([["%s"]], name)
  elseif info.type == "bool" then
    return "true"
  elseif info.type == "table" then
    return [["{}"]]
  elseif info.type == "function" then
    return string.format("function(...)\n  -- %s code here\nend", name)
  end

  return "nil"
end

local function hasKeys(tbl)
  for k, v in pairs(tbl) do
      return true
  end

  return false
end

local function main()
  local output = ""
  local outputFile = "lua-api.md"
  local apis = Nco.Reflection.getApis()

  local function appendToOutput(format, ...)
    output = output .. string.format(format, ...) .. "\n"
  end

  print("Generating API markdown docs")
  print("")

  appendToOutput(header)

  print("> Building index")

  for apiName, api in pairs(apis) do
    appendToOutput([[- [%s](#%s)]], apiName, apiName:gsub(" ", "-"))

    for funcName, func in pairs(api.functions) do
      appendToOutput([[  - [%s](#%s)]], funcName, funcName)
    end
  end

  appendToOutput("")

  print("> Building API sections")

  for apiName, api in pairs(apis) do
    print("  > Building section for API: %s...", apiName)

    local context = 
    {
      apiName = apiName,
      apiDescription = api.description
    }

    appendToOutput(
      renderTemplate(apiHeader, context)
    )

    for funcName, func in pairs(api.functions) do
      appendToOutput([[- [%s](#%s)]], funcName, funcName)
    end

    appendToOutput("")

    for funcName, func in pairs(api.functions) do
      print("    > Building section for API function: %s...", funcName)

      context.functionName = funcName
      context.functionDescription = func.description

      local exampleParameters = ""

      if hasKeys(func.parameters) then
        local firstParam = true

        for _, param in ipairs(func.parameters) do
          local n = param.name

          if not firstParam then
            exampleParameters = exampleParameters .. ", "
          end

          exampleParameters = exampleParameters .. generateExampleValueForParameter(n, param)

          firstParam = false
        end
      end

      context.exampleParameters = exampleParameters

      appendToOutput(
        renderTemplate(functionTemplate, context)
      )

      if hasKeys(func.parameters) then
        appendToOutput("Parameters:")

        for _, param in ipairs(func.parameters) do
          local n = param.name

          appendToOutput("  - `%s` [%s] - %s", n, param.type, param.description)
        end
      else
        appendToOutput("Parameters: none")
      end

      appendToOutput("")

      if hasKeys(func.returnValues) then
        appendToOutput("Return value(s):")

        for _, returnValue in ipairs(func.returnValues) do
          local valueName = returnValue.name

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

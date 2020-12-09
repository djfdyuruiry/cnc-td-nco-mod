local EventProxy = function(eventBus, eventName)
  return setmetatable(
    {},
    {
      __call = function(_, ...)
        eventBus.registerHandler(eventName, ...)
      end
    }
  )
end

local EventBus = function()
  local names = Nco.Info.getEventNames()
  local handlers = {}
  
  local index = function(self, onEventName, ...)
    local eventNameUpperCamelCase = onEventName:gsub([[^on]], "")
    local eventName = string.format("%s%s", eventNameUpperCamelCase:sub(0, 1), eventNameUpperCamelCase:sub(2))

    return EventProxy(self, eventName)
  end

  local registerHandler = function (eventName, handler)
    if type(eventName) ~= "string" then
      error("eventName passed to registerHandler was not a string")
    elseif eventName == "" then
      error("eventName passed to registerHandler was empty")
    elseif type(handlers[eventName]) ~= "table" then
      error(string.format("eventName passed to registerHandler was not a recognised event: %s", eventName))
    elseif type(handler) ~= "function" then
      error("handler passed to registerHandler was not a function")
      return
    end

    table.insert(handlers[eventName], handler)
  end

  local fire = function(eventName, ...)
    if type(eventName) ~= "string" then
      error("eventName passed to fire was not a string")
    elseif eventName == "" then
      error("eventName passed to fire was empty")
    elseif type(handlers[eventName]) ~= "table" then
      error(string.format("eventName passed to fire was not a recognised event: %s", eventName))
    end

    for _, handler in ipairs(handlers[eventName]) do
      local ok, err = pcall(handler, ...)

      if not ok then
        Nco.Utils.showError(err)
      end
    end
  end

  for _, e in ipairs(names) do
    handlers[e] = {}
  end

  return setmetatable( 
    {
      names = names,
      registerHandler = registerHandler,
      fire = fire
    },
    {
      __index = index
    }
  )
end

return EventBus

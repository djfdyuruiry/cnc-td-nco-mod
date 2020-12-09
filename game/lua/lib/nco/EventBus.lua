local function EventProxy(eventBus, eventName)
  return setmetatable(
    {},
    {
      __call = function(_, ...)
        eventBus.registerHandler(eventName, ...)
      end
    }
  )
end

local function EventBus()
  local names = Nco.Info.getEventNames()
  local handlers = {}

  local function index(self, onEventName, ...)
    local eventNameUpperCamelCase = onEventName:gsub([[^on]], "")
    local eventName = string.format(
      "%s%s",
      eventNameUpperCamelCase:sub(0, 1),
      eventNameUpperCamelCase:sub(2)
    )

    return EventProxy(self, eventName)
  end

  local function validateEventName(caller, eventName)
    if type(eventName) ~= "string" then
      Nco.Utils.errorFormat("eventName passed to %s was not a string", caller)
    elseif eventName == "" then
      Nco.Utils.errorFormat("eventName passed to %s was empty", caller)
    elseif type(handlers[eventName]) ~= "table" then
      Nco.Utils.errorFormat("eventName passed to %s was not a recognised event: %s", caller, eventName)
    end
  end

  local function registerHandler(eventName, handler)
    validateEventName("registerHandler", eventName)

    if type(handler) ~= "function" then
      error("handler passed to registerHandler was not a function")
    end

    table.insert(handlers[eventName], handler)
  end

  local function fire(eventName, ...)
    validateEventName("fire", eventName)

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

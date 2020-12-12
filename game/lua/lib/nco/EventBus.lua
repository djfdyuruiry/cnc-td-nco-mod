local eventNames = Nco.Info.getEventNames()

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
  local handlers = {}

  local function index(self, onEventName, ...)
    local eventName = onEventName:gsub([[^on]], "")

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

  local function fire(eventName, ...)
    validateEventName("fire", eventName)

    for _, handler in ipairs(handlers[eventName]) do
      local ok, err = pcall(handler, ...)

      if not ok then
        Nco.Utils.showError(err)
      end
    end
  end

  local function registerHandler(eventName, handler)
    validateEventName("registerHandler", eventName)

    if type(handler) ~= "function" then
      error("handler passed to registerHandler was not a function")
    end

    table.insert(handlers[eventName], handler)
  end

  local function getEventNames()
    return eventNames
  end

  for _, e in ipairs(eventNames) do
    handlers[e] = {}
  end

  return setmetatable( 
    {
      getEventNames = getEventNames,
      registerHandler = registerHandler,
      fire = fire
    },
    {
      __index = index
    }
  )
end

local function buildEventApiInfo()
  local eventApi =
  {
    name = "Events",
    description = "Register event handlers and get info on game events",
    functions =
    {
      getEventNames =
      {
        description = "Get a list of game events",
        returnValues =
        {
          eventNames = 
          {
            description = "Table containing names of game events as strings",
            type = "table"
          }
        }
      }
    }
  }

  for _, e in ipairs(eventNames) do
    local eventFunction = string.format("on%s", e)

    eventApi.functions[eventFunction] =
    {
      description = string.format("Register an event handler that will be called on %s", e),
      parameters =
      {
        eventHandler =
        {
          description = "Function which is called with event parameters when event is fired",
          type = "function"
        }
      }
    }
  end

  return eventApi
end

Nco.Reflection.registerApi(buildEventApiInfo())

return EventBus

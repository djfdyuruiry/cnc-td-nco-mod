local Events =
{
  handlers = {}
}

local registerEventHandler = function (eventName, handler)
  if type(eventName) ~= "string" then
    error("eventName passed to registerEventHandler was not a string")
  elseif eventName == "" then
    error("eventName passed to registerEventHandler was empty")
  elseif type(Events.handlers[eventName]) ~= "table" then
    error(string.format("eventName passed to registerEventHandler was not a recognised event: %s", eventName))
  elseif type(handler) ~= "function" then
    error("handler passed to registerEventHandler was not a function")
    return
  end

  table.insert(Events.handlers[eventName], handler)
end

Events.initHandlers = function()
  Events.names = getEventNames()

  for _, eventName in ipairs(Events.names) do
    Events.handlers[eventName] = {}

    local eventFunctionName = string.format("on%s", eventName)

    _G[eventFunctionName] = function(h)
      registerEventHandler(eventName, h)
    end
  end
end

Events.fire = function(eventName, ...)
  if type(eventName) ~= "string" then
    error("eventName passed to fire was not a string")
  elseif eventName == "" then
    error("eventName passed to fire was empty")
  elseif type(Events.handlers[eventName]) ~= "table" then
    error(string.format("eventName passed to fire was not a recognised event: %s", eventName))
  end

  for _, handler in ipairs(Events.handlers[eventName]) do
    local ok, err = pcall(handler, ...)

    if not ok then
      showError(err)
    end
  end
end

return Events

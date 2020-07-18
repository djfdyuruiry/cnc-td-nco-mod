local Events =
{
  handlers = {}
}

local registerEventHandler = function (eventName, handler)
  if eventName == nil or handler == nil then
    -- TODO: throw error on incorrect types
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
  if eventName == nil then
    -- TODO: throw error on incorrect type
    return
  end

  for _, handler in ipairs(Events.handlers[eventName]) do
    local ok, err = pcall(handler, ...)

    if not ok then
      showError(err)
    end
  end
end

return Events

local function setfenv(fn, env)
  local idx = 1

  local function envProvider()
    return env
  end

  while true do
    local name = debug.getupvalue(fn, idx)
  
    if not name then
      break
    end
  
    if name == "_ENV" then
      debug.upvaluejoin(fn, idx, envProvider, 1)
      break
    end

    idx = idx + 1
  end

  return fn
end

return setfenv

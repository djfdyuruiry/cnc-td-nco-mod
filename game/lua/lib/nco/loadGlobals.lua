return function(globalTables)
  for _, globalTable in ipairs(globalTables) do
    for var, val in pairs(globalTable) do
      _G[var] = val
    end
  end
end

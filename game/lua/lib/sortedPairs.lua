local function sortedPairs(tbl, order)
  if type(tbl) ~= "table" then
    return pairs(tbl)
  end

  local keys = {}

  for key in pairs(tbl) do
    local idx = #keys + 1

    keys[idx] = key
  end

  if type(order) == "function" then
    table.sort(
      keys,
      function(a,b)
        return order(tbl, a, b)
      end
    )
  else
    table.sort(keys)
  end

  local idx = 0

  return function()
    idx = idx + 1

    local key = keys[idx]

    if key then
      return key, tbl[key]
    end
  end
end

return sortedPairs

local ObjectProxy = function(objectPointer, method)
  return setmetatable(
    {
      pointer = objectPointer
    },
    {
      __call = function(this, ...)
        return method(this.pointer)
      end
    }
  )
end

local registerObjectMethod = function(name, objectPointer, method)
  _G[name] = ObjectProxy(objectPointer, method)
end

return
{
  ObjectProxy = ObjectProxy,
  registerObjectMethod = registerObjectMethod
}

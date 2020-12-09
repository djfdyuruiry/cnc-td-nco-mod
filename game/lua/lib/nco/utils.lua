local setfenv = require("setfenv")

return 
{
  log = function(message, ...)
    Nco.Utils.logString(string.format(message, ...))
  end,

  print = function(message, ...)
    Nco.Utils.printString(string.format(message, ...))
  end,

  showError = function(message, ...)
    Nco.Utils.showErrorString(string.format(message, ...))
  end,

  errorFormat = function(errorMessageFormat, ...)
    error(string.format(errorMessageFormat, ...))
  end,

  with = function(context, handler)
    setfenv(handler, context)

    return handler()
  end
}

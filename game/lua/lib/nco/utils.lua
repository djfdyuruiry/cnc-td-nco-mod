local setfenv = require("setfenv")

Nco.Reflection.registerApiFunctions(
  "Utils",
  {
    log =
    {
      description = "Write a line to the logs using string.format style formatting",
      parameters =
      {
        message = 
        {
          description = "Message format to log, pass one or more parameters after this to provide format values",
          type = "string"
        }
      }
    },
    printString =
    {
      description = "Write a line to standard out using string.format style formatting",
      parameters =
      {
        message = 
        {
          description = "Message format to output, pass one or more parameters after this to provide format values",
          type = "string"
        }
      }
    },
    readString =
    {
      description = "Read a line from standard in using io.read format",
      parameters =
      {
        format = 
        {
          description = "Format to read, defaults to '*l'",
          type = "string"
        }
      }
    },
    showError =
    {
      description = "Show a windows error popup to the user, using string.format style formatting for the message",
      parameters =
      {
        message = 
        {
          description = "Message format to show in the popup, pass one or more parameters after this to provide format values",
          type = "string"
        }
      }
    },
    errorFormat =
    {
      description = "Raise a lua error, using string.format style formatting for the message",
      parameters =
      {
        message = 
        {
          description = "Message format to use when raising the error, pass one or more parameters after this to provide format values",
          type = "string"
        }
      }
    },
    with =
    {
      description = "Syntax sugar to use fields and methods of a table inside a handler, useful for making multiple calls without repeating the table variable name",
      parameters =
      {
        context = 
        {
          description = "The keys in this table will be variables withing the scope of the handler",
          type = "table"
        },
        handler =
        {
          description = "The handler to execute in the provided context",
          type = "function"
        }
      },
      returnValues =
      {
        handlerReturnValue =
        {
          description = "If the handler parameter returns a value when executed, this will be returned"
        }
      }
    },
  }
)

return 
{
  log = function(message, ...)
    Nco.Utils.logString(string.format(message, ...))
  end,

  printString = function(message, ...)
    local output = message
    
    if type(message) == "string" then
      output = string.format(message, ...)
    end

    if output == message then
      _G.__print(output, ...)

      return
    end

    _G.__print(output)
  end,

  readString = function(format)
    if format == nil or format == "*l" or format == "*L" then
      return Nco.Utils.readString()
    end

    error(string.format("Format not supported by readString: %s", format))
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

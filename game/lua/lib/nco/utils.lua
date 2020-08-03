return
{
  log = function(message, ...)
    logString(string.format(message, ...))
  end,

  print = function(message, ...)
    printString(string.format(message, ...))
  end,

  showError = function(message, ...)
    showErrorString(string.format(message, ...))
  end
}

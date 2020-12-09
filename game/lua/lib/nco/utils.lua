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
  end
}

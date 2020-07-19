_G.log = function(message, ...)
  logString(string.format(message, ...))
end

_G.print = function(message, ...)
  printString(string.format(message, ...))
end

_G.showError = function(message, ...)
  showErrorString(string.format(message, ...))
end

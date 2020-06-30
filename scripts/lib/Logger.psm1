function Write-Log {
  param(
    $Message,
    $Level = "INFO",
    $Colour = $null
  )

  $formattedMessage = `
    ">> $(Get-Date -Format "dd/MM/yyyy H:MM:ss.ff") - ${Level} - ${Message}" 

  if ($Colour) {
    Write-Host -Object $formattedMessage -ForegroundColor $Colour
  } else {
    Write-Host -Object $formattedMessage
  }
}

function Write-Log-Success {
  param($Message)

  Write-Log -Message $Message -Colour "Green"
}

function Write-Log-Debug {
  param($Message)

  Write-Log -Message $Message -Level "DEBUG" -Colour "Purple"
}

Set-Alias -Name Write-Log-Info -Value Write-Log

function Write-Log-Warn {
  param($Message)

  Write-Log -Message $Message -Level "WARN" -Colour "Yellow"
}

function Write-Log-Error {
  param($Message)

  Write-Log -Message $Message -Level "ERROR" -Colour "Red"
}

Export-ModuleMember -Function * -Alias *

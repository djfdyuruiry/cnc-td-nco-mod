using namespace System.IO

$ErrorActionPreference = "Stop"

Import-Module -Name "${PSScriptRoot}\lib\Logger" -Force 3> $null

$repoRootPath = [Path]::GetFullPath("${PSScriptRoot}\..")

$gameFilesPath = "${repoRootPath}\game"

$destFile = $args[0]

function Main {
  if ([string]::IsNullorEmpty($destFile)) {
    Write-Log-Error "Destination file was blank"

    exit 1
  }

  [xml] $modCommands = Get-Content "${gameFilesPath}\XML\CNCModGameCommands.xml"
  [xml] $testModCommands = Get-Content "${gameFilesPath}\XML\TestModCommands.xml"

  $testModCommands.SelectNodes("//Command") | Select-Object {
    $importedCommand = $modCommands.ImportNode($_, $true);

    $modCommands.ModGameCommands.`
      CNCModGameCommandManager.`
      CNCModGameCommands.AppendChild($importedCommand)
  }

  if (Test-Path $destFile -ErrorAction SilentlyContinue) {
    Remove-Item -Force $destFile
  }

  $modCommands.Save($destFile)
}

Main

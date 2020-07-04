using namespace System.IO

$ErrorActionPreference = "Stop"

Import-Module -Name "${PSScriptRoot}\lib\Logger" -Force 3> $null

$repoRootPath = [Path]::GetFullPath("${PSScriptRoot}\..")

$gameFilesPath = "${repoRootPath}\game"
$workshopInPath = "${gameFilesPath}\steam-workshop"

$buildOutPath =  "${repoRootPath}\bin\Win32"

$workshopOutPath = "${repoRootPath}\.steam-workshop"
$workshopOutDataPath = "${workshopOutPath}\NCO\Data"

function Zip-Mod-Package {
  Write-Log-Info "Zipping mod package"

  $currentGitCommit=(git --git-dir "${repoRootPath}\.git" rev-parse --short HEAD)
 
  Compress-Archive -Path "${workshopOutPath}\*" `
    -DestinationPath "${workshopOutPath}\cnc-td-nco-mod-steam-${currentGitCommit}.zip"
}

function Generate-Rules-File {
  & "${repoRootPath}\scripts\build-and-dump-rules.bat"

  Copy-Item -Path "${buildOutPath}\RULES-DUMP.INI" -Destination "${workshopOutDataPath}\RULES.INI"
}

function Build-Mod-Spec {
  Write-Log-Info "Buidling mod spec file"

  $modSpec = Get-Content -Raw -Path "${workshopInPath}\NCO.workshop.json" | ConvertFrom-Json
  $description = Get-Content -Raw -Path "${workshopInPath}\description.txt" | Out-String

  $modSpec.description = $description

  $modSpec | ConvertTo-Json -Depth 5 | Out-File "$workshopOutPath\NCO.workshop.json"
}

function Copy-Game-Files {
  Write-Log-Info "Copying game files"

  Copy-Item -Path "${gameFilesPath}\ccmod.json" -Destination "${workshopOutPath}\NCO"
  Copy-Item -Path "${workshopInPath}\NCOWorkshopPreview.png" -Destination $workshopOutPath
  Copy-Item -Path "${buildOutPath}\TiberianDawn.dll" -Destination $workshopOutDataPath
}

function Main {
  Write-Log-Info "Packaging NCO mod for Steam Workshop"

  if (Test-Path $workshopOutPath) {
    Remove-Item -Recurse -Force $workshopOutPath
  }

  New-Item -Path $workshopOutDataPath -ItemType directory | Out-Null

  Copy-Game-Files
  Build-Mod-Spec
  Generate-Rules-File

  Zip-Mod-Package
}

Main

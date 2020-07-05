using namespace System.IO

$ErrorActionPreference = "Stop"

Import-Module -Name "${PSScriptRoot}\lib\Logger" -Force 3> $null

$repoRootPath = [Path]::GetFullPath("${PSScriptRoot}\..")

$gameFilesPath = "${repoRootPath}\game"

function Main {
    Write-Log-Info "Building debug rules configuration"

    $generatedRules = (Get-Content "${gameFilesPath}\RULES-DUMP.INI" | Select-Object -Skip 10) | Out-String
    $debugRules = Get-Content -Raw -Path "${gameFilesPath}\RULES.INI"

    $debugRules | Set-Content "${gameFilesPath}\RULES-DEBUG.INI"
    $generatedRules | Add-Content "${gameFilesPath}\RULES-DEBUG.INI"
}

Main

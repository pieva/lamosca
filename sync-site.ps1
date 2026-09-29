# Script per sincronizzare i file della documentazione web su valocchi.it
$ErrorActionPreference = "Stop"

$sourceDir = Join-Path $PSScriptRoot "web"
$targetDir = "C:\Users\pietr\Il mio Drive (pier.piero.pietro@gmail.com)\Web\valocchi.it\lamosca"

Write-Host "===================================================" -ForegroundColor Cyan
Write-Host " Sincronizzazione Sito Web LaMoSca -> valocchi.it" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "Sorgente:    $sourceDir" -ForegroundColor Gray
Write-Host "Destinazione: $targetDir" -ForegroundColor Gray

if (-not (Test-Path $sourceDir)) {
    Write-Error "Cartella sorgente '$sourceDir' non trovata."
    exit 1
}

if (-not (Test-Path $targetDir)) {
    Write-Error "Cartella di destinazione su Google Drive '$targetDir' non trovata."
    exit 1
}

Write-Host "`nCopia file e immagini in corso..." -ForegroundColor Yellow

# Copia ricorsiva dei contenuti di web/ in lamosca/
Copy-Item -Path "$sourceDir\*" -Destination $targetDir -Recurse -Force

Write-Host "`n===================================================" -ForegroundColor Green
Write-Host " SINCRONIZZAZIONE COMPLETATA CON SUCCESSO!" -ForegroundColor Green
Write-Host " Google Drive aggiornera' i file online su valocchi.it" -ForegroundColor Green
Write-Host "===================================================" -ForegroundColor Green

$ErrorActionPreference = "Stop"

Write-Host "===================================================" -ForegroundColor Cyan
Write-Host " Compilazione LaMoSca (64-bit MSVC)" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    Write-Error "vswhere.exe non trovato in $vswhere"
    exit 1
}

$vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsPath) {
    Write-Error "Installazione Visual Studio C++ non trovata."
    exit 1
}

Write-Host "Visual Studio rilevato in: $vsPath" -ForegroundColor Gray

$vcvars = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
if (-not (Test-Path $vcvars)) {
    Write-Error "vcvars64.bat non trovato in $vcvars"
    exit 1
}

if (-not (Test-Path "build")) { New-Item -ItemType Directory -Path "build" | Out-Null }
if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" | Out-Null }

Write-Host "Compilazione sorgenti C in corso..." -ForegroundColor Yellow

$cmd = "call `"$vcvars`" >nul && cl /nologo /W3 /O2 /D_CRT_SECURE_NO_WARNINGS /Fo:build\ /Fe:bin\LaMoSca.exe src\LaMoSca.c src\Genera.c src\Cerca.c src\Valuta.c"
cmd.exe /c $cmd

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n===================================================" -ForegroundColor Green
    Write-Host " COMPILAZIONE RIUSCITA!" -ForegroundColor Green
    Write-Host " Eseguibile generato: bin\LaMoSca.exe (64-bit nativo)" -ForegroundColor Green
    Write-Host "===================================================" -ForegroundColor Green
} else {
    Write-Host "`n[ERRORE] Compilazione fallita con codice $LASTEXITCODE." -ForegroundColor Red
    exit $LASTEXITCODE
}

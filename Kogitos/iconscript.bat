@echo off
chcp 65001 > nul

set "EXE_NAME=Kogitos.exe"
set "ICO_NAME=Kogitos icon.ico"
set "SHORTCUT_NAME=Kogitos.lnk"

set "TARGET_DIR=%~dp0"
set "EXE_PATH=%TARGET_DIR%%EXE_NAME%"
set "ICO_PATH=%TARGET_DIR%%ICO_NAME%"
set "DESKTOP_PATH=%USERPROFILE%\Desktop\%SHORTCUT_NAME%"

if not exist "%EXE_PATH%" (
    echo [ERREUR] Impossible de trouver le fichier %EXE_NAME% !
    pause
    exit /b
)

set "PS_SCRIPT=%TEMP%\create_shortcut.ps1"

echo $WshShell = New-Object -ComObject WScript.Shell > "%PS_SCRIPT%"
echo $Shortcut = $WshShell.CreateShortcut('%DESKTOP_PATH%') >> "%PS_SCRIPT%"
echo $Shortcut.TargetPath = '%EXE_PATH%' >> "%PS_SCRIPT%"
echo $Shortcut.WorkingDirectory = '%TARGET_DIR%' >> "%PS_SCRIPT%"
if exist "%ICO_PATH%" (
    echo $Shortcut.IconLocation = '%ICO_PATH%' >> "%PS_SCRIPT%"
)
echo $Shortcut.Save() >> "%PS_SCRIPT%"

powershell -ExecutionPolicy Bypass -NoProfile -File "%PS_SCRIPT%"
del "%PS_SCRIPT%"

echo.
echo [SUCCÈS] Le raccourci "Kogitos" avec son icône a été créé sur le Bureau !
pause
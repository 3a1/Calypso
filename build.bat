@echo off
TITLE Calypso Builder

echo   ___      _
echo  / __^|__ _^| ^|_  _ _ __ ___ ___
echo ^| (__/ _` ^| ^| ^|^| ^| '_ (_-^</ _ \
echo  \___\__,_^|_^|\_, ^| .__/__/\___/
echo              ^|__/^|_^|
echo.

echo [+] Press Any Key to start compilation

pause

"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "Calypso/CalypsoEFI/CalypsoEFI.sln" /t:Clean,Build /property:Configuration=Release /property:Platform=x64
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "Calypso/CalypsoUM/CalypsoUM.sln" /t:Clean,Build /property:Configuration=Release /property:Platform=x64

cls

echo.
copy "Calypso\CalypsoUM\x64\Release\CalypsoUM.exe" .
copy "Calypso\CalypsoEFI\x64\Release\CalypsoEFI.efi" "USB\"
echo.

echo [+] Compilation done

pause
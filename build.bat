"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" "CalypsoUM.sln" /t:Clean,Build /property:Configuration=Release /property:Platform=x64
copy "x64\Release\CalypsoUM.exe" .

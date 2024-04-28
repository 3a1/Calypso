echo -off

cls

if exist fs0:\CalypsoEFI.efi then
fs0:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs1:\CalypsoEFI.efi then
fs1:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs2:\CalypsoEFI.efi then
fs2:
load CalypsoEFI.efi
goto EXIT
endif

:EXIT
stall 5000000
exit
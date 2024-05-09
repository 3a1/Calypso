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

if exist fs3:\CalypsoEFI.efi then
fs3:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs4:\CalypsoEFI.efi then
fs4:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs5:\CalypsoEFI.efi then
fs5:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs6:\CalypsoEFI.efi then
fs6:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs7:\CalypsoEFI.efi then
fs7:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs8:\CalypsoEFI.efi then
fs8:
load CalypsoEFI.efi
goto EXIT
endif

if exist fs9:\CalypsoEFI.efi then
fs9:
load CalypsoEFI.efi
goto EXIT
endif

load CalypsoEFI.efi
goto EXIT

:EXIT
stall 5000000
exit

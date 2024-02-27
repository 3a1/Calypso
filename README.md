# **CS2-UEFI-Arduino**

UEFI Driver + Aduino Leonardo CS2 Cheat

In development phase.

After you open client side for first time application will create config for you with my own settings that i use for full legit playstyle.
You can always open config.cfg and set settings to your own preferations, i tries make it as easy as it can be.

Usage:
+ Format your usb drive
+ Download shell.efi from [this](https://github.com/tianocore/edk2-archive/blob/master/ShellBinPkg/UefiShell/X64/Shell.efi) repo and rename it to bootx64.efi
+ Compile EFIDriver from my repo (in linux type just "make" in console and done)
+ Simply drop EFIDriver(memory.efi) to pendrive then create folders in usb drive and paste bootx64.efi(D:\EFI\Boot\bootx64.efi; D:\memory.efi)
+ Boot from usb drive and try to find your usb drive by typing "FS(disk number):" then "ls". For me it always on FS2:
+ Load driver by "load memory.efi", then just type exit and load back to your system
+ Start EFIClient application and enjoy


TODO:

+ Smart legit aimbot.
+ Make choose between arduino and non-adruino version
+ Bhop
+ Clean code and fix bugs
+ Auto-update offsets

Constribution:
+ [EFI_Driver_Access](https://github.com/TheCruZ/EFI_Driver_Access)

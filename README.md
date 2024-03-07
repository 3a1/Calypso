# CS2-UEFI-Arduino

UEFI Driver + Arduino Support CS2 Cheat

You can always open config.cfg and set settings to your own preferations, i tries make it as easy as it can be.

## Setup
- Usb Drive
- [Optional]Arduino

(Any arduino that supports Mouse.h library)

## Features
- Aimbot
- Triggerbot
- Config system
- Arduino && Non-Arduino Support

## Default Key Binds:

- **Aimbot**: LMouse
- **Triggerbot**: Alt

## Usage:
+ Format your usb drive
+ Download shell.efi from [this](https://github.com/tianocore/edk2-archive/blob/master/ShellBinPkg/UefiShell/X64/Shell.efi) repo and rename it to bootx64.efi
+ Compile EFIDriver from my repo (in linux type just "make" in console and done)
+ Simply drop EFIDriver(memory.efi) to pendrive then create folders in usb drive and paste bootx64.efi(D:\EFI\Boot\bootx64.efi; D:\memory.efi)
+ Boot from usb drive and try to find your usb drive by typing "FS(disk number):" then "ls". For me it always on FS2:
+ Load driver by "load memory.efi", then just type exit and load back to your system
+ Start CS2 and EFIClient application, enjoy


## TODO:
+ Add Raspberry Pi Pico W Support(?)
+ Clean code and fix bugs
+ Auto-update offsets

## Credits:
+ [EFI Driver](https://github.com/TheCruZ/EFI_Driver_Access)
+ [Arduino Code](https://github.com/backpack-0x1337/CyberAim-Valorant/)

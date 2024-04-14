# Calypso

<p align="center">
<img src="https://i.imgur.com/0VWHi0u.png" alt="Banshee" width="400" />
</p>

UEFI Driver + Arduino Support CS2 Cheat

Regular updates are temporarily frozen due to a working for huge update. (3.0)

[Link[2.5 updated]](https://mega.nz/file/DlAV0QaQ#XDajJ3kTm4gqqzBYPOdKnABkS7poS7nIDM7CR6YJDDI) to the compiled EFIDriver (will be here until I change the driver so it can be compiled through Visual Studio)

## Setup
- Usb Drive
- [Optional]Arduino

(Any arduino that supports Mouse.h library)

## Genaral
- Aimbot (Customizable field of view, smoothing, speed and key).
- Triggerbot (You can customize triggerbot delay and key)
- Config system
- Can be used with or without Arduino

## Default Key Binds:

- **Aimbot**: Left Click
- **Triggerbot**: Alt

## Usage:
1. Format your usb drive
2. Download shell.efi from [this](https://github.com/tianocore/edk2-archive/blob/master/ShellBinPkg/UefiShell/X64/Shell.efi) repo and rename it to bootx64.efi
3. Compile EFIDriver from my repo (in linux type just "make" in console and done)
4. Simply drop EFIDriver(memory.efi) to pendrive then create folders in usb drive and paste bootx64.efi:
```
USB:.
 │   memory.efi
 │
 └───EFI
      └───Boot
              bootx64.efi
```
5. Boot from usb drive and try to find your usb drive by typing "FS(disk number):" then "ls". For me it always on FS2:
6. Load driver by "load memory.efi", then just type exit and load back to your system
7. Start CS2 and open EFIClient, enjoy 

## Usage & Showcase:
Click on image below to watch it on YouTube

[![Click](http://i3.ytimg.com/vi/J-yl2FgI6eo/hqdefault.jpg)](https://youtu.be/FY2V1YdpacM)

## TODO:
+ Add Raspberry Pi Pico W Support(?)
+ Add USB Host Shield support
+ Get module base address via EFI not usermode(?)
+ Add fullscreen esp(?)
+ Code own EFI Driver(?)
+ Clean code and fix bugs
+ Auto-update offsets

## Credits:
+ [EFI Driver](https://github.com/TheCruZ/EFI_Driver_Access)
+ [Arduino Code](https://github.com/backpack-0x1337/CyberAim-Valorant/)

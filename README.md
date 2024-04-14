# Calypso

<p align="center">
<img src="https://i.imgur.com/0VWHi0u.png" alt="Banshee" width="400" />
</p>

UEFI Driver + Arduino Support CS2 Cheat

Regular updates are temporarily frozen due to a working for huge update. (3.0)

[2.6 updated][Link](https://mega.nz/file/6xxAgb4T#gLYt3pbMdsPQdnb36Iq3zfLja-xg4gjK9zdbZhuTtjU) to the compiled EFIDriver, so you can skip 1-3 steps in usage

## Setup
- Usb Drive
- [Optional]Arduino

(Any arduino that supports Mouse.h library)

## Features
- Aimbot (customizable field of view, smoothing, speed and key).
- Triggerbot (customizable triggerbot, delay and key)
- Config system
- Can be used with or without Arduino

## Default Key Binds

- **Aimbot**: Left Click
- **Triggerbot**: Alt

## Usage
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
7. Install visual studio 2022, and open build.bat, after a while you will see CalypsoUM.exe file in the folder (optional: you can open CalypsoUM.sln and build project by yourself)
8. Start cs2 and open CalypsoUM.exe

<p align="center">
<img src="https://i.imgur.com/9fW1NGv.png" alt="Calypso CLI" width="600"/>
</p>

## Usage & Showcase
Click on image below to watch it on YouTube

[![Click](http://i3.ytimg.com/vi/J-yl2FgI6eo/hqdefault.jpg)](https://youtu.be/FY2V1YdpacM)

## TODO
+ Add Raspberry Pi Pico W Support(?)
+ Add USB Host Shield support
+ Get module base address via EFI not usermode(?)
+ Add fullscreen esp(?)
+ Clean code and fix bugs
+ Auto-update offsets

## Credits
+ [EFI Driver](https://github.com/TheCruZ/EFI_Driver_Access)
+ [Arduino Code](https://github.com/backpack-0x1337/CyberAim-Valorant/)

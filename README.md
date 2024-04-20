# Calypso

<p align="center">
<img src="https://i.imgur.com/0VWHi0u.png" alt="Calypso" width="400" />
</p>

<p align="center">
Calypso is CS2 EFI cheat with Arduino Support.

04/20: updates for Calypto currently on freeze due to busy in irl

</p>

## Setup
- Usb Drive
- [Optional]Arduino

(Any arduino that supports Mouse.h library)

## Features
#### Aimbot
- Customizable `fov`, `smooth`, `speed` and `key`.
#### Triggerbot
- Customizable `delay` and `key`.
#### Config System
- Cheat uses self-sufficient config system. Dont have config file? Cheat will create it itself.
#### Arduino Support
- Cheat be used with or without Arduino.

## Default Key Binds

 **Aimbot**: `Left Click`
 
 **Triggerbot**: `Alt`

## Usage
[2.6 updated][Link](https://mega.nz/file/6xxAgb4T#gLYt3pbMdsPQdnb36Iq3zfLja-xg4gjK9zdbZhuTtjU) to the compiled CalypsoEFI, so you can skip 1-3 steps in usage
1. Format your usb drive.
2. Download `shell.efi` from [this](https://github.com/tianocore/edk2-archive/blob/master/ShellBinPkg/UefiShell/X64/Shell.efi) repo and rename it to `bootx64.efi`.
3. Compile `CalypsoEFI` from my repo (in linux type just `make` in console and done).
4. Simply drop `memory.efi` to pendrive then create folders in usb drive and paste `bootx64.efi` like this:
```
USB:.
 │   memory.efi
 │
 └───EFI
      └───Boot
              bootx64.efi
```
5. Boot from usb drive and try to find your usb drive by typing "FS(disk number):" then "ls". For me it always on FS2:
6. Load driver by `load memory.efi`, then just type `exit` and load back to your system.
7. Install [VS 2022](https://visualstudio.microsoft.com/downloads/), and open `build.bat`, after a while you will see `CalypsoUM.exe` file in the folder (optional: you can open `CalypsoUM.sln` and build project by yourself).
8. Start CS2 and open `CalypsoUM.exe`.

<p align="center">
<img src="https://i.imgur.com/0uCHTB9.png" alt="Calypso CLI" width="600"/>
</p>

## Showcase & Usage
Click on image below to watch it on YouTube

[![Click](https://i3.ytimg.com/vi/_rVH9mMZ--A/hqdefault.jpg)](https://www.youtube.com/watch?v=_rVH9mMZ--A)

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

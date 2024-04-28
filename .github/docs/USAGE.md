# Usage
> [!IMPORTANT]
> 💻 Calypso might doesnt work on legacy boot systems 

## Table of Contents
- [Visual Studio Installation](#visual-studio-installation)
- [Calypso Installation](#calypso-installation)

## Visual Studio Installation

- Open [Visual Studio official page](https://visualstudio.microsoft.com/ru/downloads/)

- Scroll down and click on ``community free download``. 
<img src="https://i.imgur.com/Tqvqy5P.png" height="250" />

- After download open Visual Studio Installation and follow the process until you reach download tab

- On download tab select ``Desktop development with C++``
<img src="https://i.imgur.com/eWnqAD0.png" height="250" />

- After that click on install and Visual Studio will be installed

## Calypso Installation
> [!WARNING]
> 💻 Arduino usage might doesn't work for Windows 11.

### 1. Download

- ``Download`` and ``Extract`` last Calypso Release from [releases page](https://github.com/3a1/CS2-Calypso/releases/) or directly from [github page](https://github.com/3a1/CS2-Calypso)
  
<img src="https://i.imgur.com/NjpLK7J.png" height="250" /><img src="https://i.imgur.com/vR5KNOT.png" height="250" />

### 2. Compile
- Open Calypso folder and start ``build.bat`` it will automatically compile binaries and copy them to the folder

### 3. USB Setup
- Insert your usb drive to the system and make ``format`` to the ``NTFS`` or ``FAT32``

<img src="https://i.imgur.com/XGf3iWj.png" height="250" /><img src="https://i.imgur.com/vPRhMwB.png" height="250" />

- ``Download`` efi shell from [this link](https://github.com/tianocore/edk2-archive/raw/master/ShellBinPkg/UefiShell/X64/Shell.efi)

- ``Rename`` efi sheel from ``shell.efi`` to ``bootx64.efi``

- Copy ``CalypsoEFI.efi``, ``Startup.nsh`` from Calypso folder and renamed before ``bootx64.efi`` to the usb drive like this:

```
USB:.
 │   CalypsoEFI.efi
 │   Startup.nsh
 │
 └───EFI
      └───Boot
              bootx64.efi
```
(EFI and Boot are folders that you need to create)

- Calypso EFI Setup Done!

### 4. Booting from USB Drive
> [!!WARNING]
> 💻 On all systems bios is different so this process can looks different for you

If you have PC search in google "{your motherboard model} bios key" or if you have laptop search instead "{your laptop model} bios key"
(generally in 99% it will be one of these keybinds:  F1, F2, F10, F12, Del, or Esc.) 

After that there is two methods how you can boot from usb drive:
#### Change bios boot order
> [!IMPORTANT]
> 💻 Calypso might doesnt work on legacy boot systems 

 - Boot manually from usb drive and back to the windows

 with first way after you change the boot order, your system will be automatically booting from usb drive every system startup until you will change boot order back

 with second way you just booting usb drive one time and you need to do it every system restart when you want to use the cheat

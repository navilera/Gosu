# Gosu

Gosu is the mechanical keyboard firmware based on the Navilos(https://github.com/navilera/navilos) RTOS. Currently, Gosu supports ARM Cortex-M3 controller which is STM32F103. Gosu has flexible design so it can be extended to many other contollers.

## Features

*HID USB Keyboard*
It is the core firmware feature. This main firmware should be loaded by MSC loader.

*MSC (Mass Storage Class) loader*
Gosu supports firmware/keymap updating feature via mass storage device. It doesn't need any burner equipment like serial-to-USB or ICE. It needs one of these equipment only one time to download MSC loader at the first time. After download MSC loader, It upgrade a firmware or a keymap by itself. Just drag a binary file and drop it into a mass storage drive.

*Keymap editor*
This Keymap editor is based on TMK keymap editor(http://www.tmk-kbd.com/tmk_keyboard/editor/). It is very easy to use and it has an intuitive user interface. The keymap editor generates a keymap binary file. This keymap binary file should be loaded by MSC loader.

## Build and download firmware

Gosu currently supports a bluepill and blackpill board which are very cheap STM32F103 based boards. Gosu has working (and sample) code and build environment(Makefile) for bluepill board. For make MSC loader binary file, please do following command.

> ./bluepill_loader.sh

That is all. It generates /out/loader.bin file. Then, do following command to download MSC loader into a bluepill or blackpill board after jumper setting.

> ./bluepill_loader.sh burn

For make main firmware binary file, please do following command.

> ./bluepill.sh

It generates /out/gosu.bin file. Please disconnect USB cable from bluepill or blackpill board and then press ESC key, keep pressing ESC key and re-connect USB case to bluepill board. Once it runs a MSC loader, the OS (Linux or Windows or OSX etc.) shows a mounted mass storage drive in file manager program. It is exactly same as using USB thumb drive. Finally, Just drag and drop the /out/gosu.bin file.

## Custom Keymap

User can make a their own custom keymap using the keymap editor. The keymap editor implemented by HTML so, User can use the keymap editor by run web browser.


- [Getting Started](doc/GettingStarted.md)
- [How to contribute](doc/Contribute.md)

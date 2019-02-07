# Getting Started

This guide helps you to get started with building Gosu firmware for Goso Peak
keyboard and following series. After following the steps in this guide, you are
all set to build the firmware and download it to the keyboards.

## Conventions in this guide

## System requirement

-   Mac OS or Linux system having more than 4GB memory and enough diskspace to
    compile ARM GCC. For Linux system, Debian-based (Ubuntu is recommended)
    system is assumed.
-   ARM GCC, STM32 Flash tool

## Install ARM GCC

Goso Peak and following series of keyboards use ARM controller as its main MCU.
So, to compile the firmware, it requires ARM cross-compiler. You can get ARM GCC
from [ARM developer website][]. You can download platform-specific pre-compiled
binary GCC or compile from the source code. It is assumed to use binary at this
time. 

[ARM developer website]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

Download the binary file and extract it. You just need to copy all files to any
location (recommend `~/.local/`) and put the path `bin/` into `$PATH`
environment variable.

## Compiling the Firmware

```console
$ ./bluepill.sh
```

After compiling is done, new firmware is created under `out/gosu.bin`. 

## Download Firmware via USB-UART board

At this moment, gosu firmware can be downloaded to STM32 dev board for testing
purpose. Downloading firmware to STM32 uses USB-to-UART board as of now. By
default, STM32 controller only supports UART firmware download. It is planned to
support USB firmware download mode after intial firmware is written to internal
eFlash memory.

```console
$ ./bluepill.sh burn
```

You need `stm32flash` tool, which can be installed with `sudo apt install
stm32flash` in linux and `brew install stm32flash` in macOS.

## Download Firmware via USB

### Entering to DFU mode

### Download

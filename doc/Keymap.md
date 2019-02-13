# Keymap

Gosu firmware supports configurable key mapping without changing Firmware
binary. It communicates to host using Virtual COM Port (VCP) defined in USB
specification.

When firmware enters keymap download mode (TBD later), it enables VCP port so
host can initiate keymap transfer.

## Keymap structure

Keymap is binary stream, not using JSON format to help Firmware parse the
content easily. First 4 bytes are CRC(or checksum) of following data stream and
next two bytes indicate the number of rows and colums of the actual keymaps.
Number of layers is pre-defined so that host should send all layer information
even only one layer is used (as of now, Gosu for Coso peak supports two layers).

    {
      crc: 32bit
      row: 8bit
      col: 8bit
      keys_t keycodes[NUM_LAYERS][ROW * COL] : 8bit X ROW X COL X NUM_LAYERS
    }

8bit of actual keycodes are defined in `keymap.h` which adding a few reserved
key code for extended functionality on top of HID scancode.

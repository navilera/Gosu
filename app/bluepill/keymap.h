/**
 * file: keymap.h
 * auther: @eunchan
 *
 * description: Keymap Structure used in HID, KeymapDl
 *
 * &copy; COPYRIGHT(c) 2019 Polypeak LLC,
 */

#ifndef APP_CORE_KEYMAP_H_
#define APP_CORE_KEYMAP_H_

#include "stdint.h"
#include "stdbool.h"

/* Modifier: First byte in HID report */
typedef enum HidMod {
  kModLctrl  = 0x01,
  kModLshift = 0x02,
  kModLalt   = 0x04,
  kModLmeta  = 0x08,
  kModRctrl  = 0x10,
  kModRshift = 0x20,
  kModRalt   = 0x40,
  kModRmeta  = 0x80
} HidMod_t;

typedef enum Scancode {
  kNone = 0x00,
  kErrOvf = 0x01, /* Keyboard Error Roll Over */
  kA = 0x04,
  kB = 0x05,
  kC = 0x06,
  kD = 0x07,
  kE = 0x08,
  kF = 0x09,
  kG = 0x0a,
  kH = 0x0b,
  kI = 0x0c,
  kJ = 0x0d,
  kK = 0x0e,
  kL = 0x0f,
  kM = 0x10,
  kN = 0x11,
  kO = 0x12,
  kP = 0x13,
  kQ = 0x14,
  kR = 0x15,
  kS = 0x16,
  kT = 0x17,
  kU = 0x18,
  kV = 0x19,
  kW = 0x1a,
  kX = 0x1b,
  kY = 0x1c,
  kZ = 0x1d,

  k1 = 0x1e,
  k2 = 0x1f,
  k3 = 0x20,
  k4 = 0x21,
  k5 = 0x22,
  k6 = 0x23,
  k7 = 0x24,
  k8 = 0x25,
  k9 = 0x26,
  k0 = 0x27,

  kEnter = 0x28,
  kEsc = 0x29,
  kBackspace = 0x2a,
  kTab = 0x2b,
  kSpace = 0x2c,
  kMinus = 0x2d,
  kEqual = 0x2e,
  kLeftbrace = 0x2f,
  kRightbrace = 0x30,
  kBackslash = 0x31, /* \ and | */
  kHashtilde = 0x32, /* Non-US # and ~ */
  kSemicolon = 0x33,
  kApostrophe = 0x34,
  kGrave = 0x35, /* ` and ~ */
  kComma = 0x36, /* , and < */
  kDot = 0x37,   /* . and > */
  kSlash = 0x38, /* / and ? */
  kCapslock = 0x39,

  kF1 = 0x3a,
  kF2 = 0x3b,
  kF3 = 0x3c,
  kF4 = 0x3d,
  kF5 = 0x3e,
  kF6 = 0x3f,
  kF7 = 0x40,
  kF8 = 0x41,
  kF9 = 0x42,
  kF10 = 0x43,
  kF11 = 0x44,
  kF12 = 0x45,

  kSysreq = 0x46,     /* PrintScrn */
  kScrolllock = 0x47, /* Scroll Lock */
  kPause = 0x48,
  kInsert = 0x49,
  kHome = 0x4a,
  kPageup = 0x4b,
  kDelete = 0x4c,
  kEnd = 0x4d,
  kPagedown = 0x4e,
  kRight = 0x4f,
  kLeft = 0x50,
  kDown = 0x51,
  kUp = 0x52,

  kNumlock = 0x53,
  kKpslash = 0x54,    // Keypad /
  kKpasterisk = 0x55, // Keypad *
  kKpminus = 0x56,    // Keypad -
  kKpplus = 0x57,     // Keypad +
  kKpenter = 0x58,    // Keypad ENTER
  kKp1 = 0x59,        // Keypad 1 and End
  kKp2 = 0x5a,        // Keypad 2 and Down Arrow
  kKp3 = 0x5b,        // Keypad 3 and PageDn
  kKp4 = 0x5c,        // Keypad 4 and Left Arrow
  kKp5 = 0x5d,        // Keypad 5
  kKp6 = 0x5e,        // Keypad 6 and Right Arrow
  kKp7 = 0x5f,        // Keypad 7 and Home
  kKp8 = 0x60,        // Keypad 8 and Up Arrow
  kKp9 = 0x61,        // Keypad 9 and Page Up
  kKp0 = 0x62,        // Keypad 0 and Insert
  kKpdot = 0x63,      // Keypad . and Delete

  k102nd = 0x64,   // Keyboard Non-US \ and |
  kCompose = 0x65, // Keyboard Application
  kPower = 0x66,   // Keyboard Power
  kKpequal = 0x67, // Keypad =

  kF13 = 0x68, // Keyboard F13
  kF14 = 0x69, // Keyboard F14
  kF15 = 0x6a, // Keyboard F15
  kF16 = 0x6b, // Keyboard F16
  kF17 = 0x6c, // Keyboard F17
  kF18 = 0x6d, // Keyboard F18
  kF19 = 0x6e, // Keyboard F19
  kF20 = 0x6f, // Keyboard F20
  kF21 = 0x70, // Keyboard F21
  kF22 = 0x71, // Keyboard F22
  kF23 = 0x72, // Keyboard F23
  kF24 = 0x73, // Keyboard F24

  kOpen = 0x74,  // Keyboard Execute
  kHelp = 0x75,  // Keyboard Help
  kProps = 0x76, // Keyboard Menu
  kFront = 0x77, // Keyboard Select
  kStop = 0x78,  // Keyboard Stop
  kAgain = 0x79, // Keyboard Again
  kUndo = 0x7a,  // Keyboard Undo
  kCut = 0x7b,   // Keyboard Cut
  kCopy = 0x7c,  // Keyboard Copy
  kPaste = 0x7d, // Keyboard Paste
  kFind = 0x7e,  // Keyboard Find
  kMute = 0x7f,  // Keyboard Mute
  kVup = 0x80,   // Keyboard Volume Up
  kVdown = 0x81, // Keyboard Volume Down
  // 0x82  Keyboard Locking Caps Lock
  // 0x83  Keyboard Locking Num Lock
  // 0x84  Keyboard Locking Scroll Lock
  kKpcomma = 0x85, // Keypad Comma
  // 0x86  Keypad Equal Sign
  Kro = 0x87,               // Keyboard International1
  kKatakanahiragana = 0x88, // Keyboard International2
  kYen = 0x89,              // Keyboard International3
  kHenkan = 0x8a,           // Keyboard International4
  kMuhenkan = 0x8b,         // Keyboard International5
  kKpjpcomma = 0x8c,        // Keyboard International6
  // 0x8d  Keyboard International7
  // 0x8e  Keyboard International8
  // 0x8f  Keyboard International9
  kHangeul = 0x90,        // Keyboard LANG1
  Khanja = 0x91,          // Keyboard LANG2
  kKatakana = 0x92,       // Keyboard LANG3
  kHiragana = 0x93,       // Keyboard LANG4
  kZenkakuhankaku = 0x94, // Keyboard LANG5
  // 0x95  Keyboard LANG6
  // 0x96  Keyboard LANG7
  // 0x97  Keyboard LANG8
  // 0x98  Keyboard LANG9
  // 0x99  Keyboard Alternate Erase
  // 0x9a  Keyboard SysReq/Attention
  // 0x9b  Keyboard Cancel
  // 0x9c  Keyboard Clear
  // 0x9d  Keyboard Prior
  // 0x9e  Keyboard Return
  // 0x9f  Keyboard Separator
  // 0xa0  Keyboard Out
  // 0xa1  Keyboard Oper
  // 0xa2  Keyboard Clear/Again
  // 0xa3  Keyboard CrSel/Props
  // 0xa4  Keyboard ExSel

  // 0xb0  Keypad 00
  // 0xb1  Keypad 000
  // 0xb2  Thousands Separator
  // 0xb3  Decimal Separator
  // 0xb4  Currency Unit
  // 0xb5  Currency Sub-unit
  kKpleftparen = 0xb6,  // Keypad (
  kKprightparen = 0xb7, // Keypad )
  // 0xb8  Keypad {
  // 0xb9  Keypad }
  // 0xba  Keypad Tab
  // 0xbb  Keypad Backspace
  // 0xbc  Keypad A
  // 0xbd  Keypad B
  // 0xbe  Keypad C
  // 0xbf  Keypad D
  // 0xc0  Keypad E
  // 0xc1  Keypad F
  // 0xc2  Keypad XOR
  // 0xc3  Keypad ^
  // 0xc4  Keypad %
  // 0xc5  Keypad <
  // 0xc6  Keypad >
  // 0xc7  Keypad &
  // 0xc8  Keypad &&
  // 0xc9  Keypad |
  // 0xca  Keypad ||
  // 0xcb  Keypad :
  // 0xcc  Keypad #
  // 0xcd  Keypad Space
  // 0xce  Keypad @
  // 0xcf  Keypad !
  // 0xd0  Keypad Memory Store
  // 0xd1  Keypad Memory Recall
  // 0xd2  Keypad Memory Clear
  // 0xd3  Keypad Memory Add
  // 0xd4  Keypad Memory Subtract
  // 0xd5  Keypad Memory Multiply
  // 0xd6  Keypad Memory Divide
  // 0xd7  Keypad +/-
  // 0xd8  Keypad Clear
  // 0xd9  Keypad Clear Entry
  // 0xda  Keypad Binary
  // 0xdb  Keypad Octal
  // 0xdc  Keypad Decimal
  // 0xdd  Keypad Hexadecimal

  kLeftctrl = 0xe0,   // Keyboard Left Control
  kLeftshift = 0xe1,  // Keyboard Left Shift
  kLeftalt = 0xe2,    // Keyboard Left Alt
  kLeftmeta = 0xe3,   // Keyboard Left GUI
  kRightctrl = 0xe4,  // Keyboard Right Control
  kRightshift = 0xe5, // Keyboard Right Shift
  kRightalt = 0xe6,   // Keyboard Right Alt
  kRightmeta = 0xe7,  // Keyboard Right GUI

  kMplaypause = 0xe8,
  kMstopcd = 0xe9,
  kMprevioussong = 0xea,
  kMnextsong = 0xeb,
  kMejectcd = 0xec,
  kMvolumeup = 0xed,
  kMvolumedown = 0xee,
  kMmute = 0xef,
  kMwww = 0xf0,
  kMback = 0xf1,
  kMforward = 0xf2,
  kMstop = 0xf3,
  kMfind = 0xf4,
  kMscrollup = 0xf5,
  kMscrolldown = 0xf6,
  kMedit = 0xf7,
  kMsleep = 0xf8,
  kMcoffee = 0xf9,
  kMrefresh = 0xfa,
  kMcalc = 0xfb,
  kFunction = 0xff			/* Gosu defined scan code for Fn key */
} Scancode_t;

#define NUM_LAYERS 2  /* Total two layers */

#define KEYMAP_COL_NUM	14
#define KEYMAP_ROW_NUM	6

#define TOTAL_KEY_NUM	82

typedef union Keyaddr
{
	uint8_t val;
	struct {
		uint8_t col:4;
		uint8_t row:3;
		uint8_t fn:1;
	} bit;
} Keyaddr_t;

typedef struct KeymapFile
{
	uint32_t	checksum;
        uint8_t     num_row;
        uint8_t     num_col;
	uint8_t		keymap[NUM_LAYERS][TOTAL_KEY_NUM];
} KeymapFile_t;

/*
 * public functions
 */
void LoadKeymap(void);
bool WriteKeyMapToFlash(KeymapFile_t* keyfile, uint32_t size);

#endif /*APP_CORE_KEYMAP_H_*/

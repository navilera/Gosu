#ifndef APP_GOSUPEEK_LAYOUT
#define APP_GOSUPEEK_LAYOUT

#include "Keycode.h"

#define NUM_LAYERS 2  

#define KEYMAP_COL_NUM	        14
#define KEYMAP_ROW_NUM	        6
#define TOTAL_KEY_MAP_SIZE	(KEYMAP_COL_NUM * KEYMAP_ROW_NUM)

extern Keypin_t gRowPin[KEYMAP_ROW_NUM];
extern Keypin_t gColPin[KEYMAP_COL_NUM];
extern uint8_t gKeymap_buffer_layer0[KEYMAP_ROW_NUM][KEYMAP_COL_NUM];
extern uint8_t gKeymap_buffer_layer1[KEYMAP_ROW_NUM][KEYMAP_COL_NUM];

#endif


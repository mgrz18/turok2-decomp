/* Local types that vm1/MenuItem and its vm2 copy both define (one layout): one definition
 * for both copies (tools/hoist_blocks.py). */
#ifndef STRUCTS_TWIN_MENUITEM_H
#define STRUCTS_TWIN_MENUITEM_H

#include "common.h"

/* The colour records a window starts with (Menu colors): 8 records of
 * 0x18 bytes, read as halfwords at 0, 4, 8 and bytes at 0xD, 0x11, 0x15 (the
 * low parts of six words: the symbol sits 2 bytes into the table). */
typedef struct { u16 a; u8 pad2[2]; u16 b; u8 pad6[2]; u16 c; u8 padA[3]; u8 d; u8 pad0E[3]; u8 e; u8 pad12[3]; u8 f; u8 pad16[2]; } MenuColorInit;

#endif

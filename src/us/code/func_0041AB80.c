#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *key;
} KeyEvent;


/* Inserts the event's character at the cursor (max 22 chars). */
s32 EnterCheatMenu_InsertLetter(KeyEvent *ev) {
    s32 i;

    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 22) {
        return 0;
    }
    for (i = 22; gCheatEntry.cursor < i; i--) {
        gCheatEntryText[i] = gCheatEntryText[i - 1];
    }
    gCheatEntryText[gCheatEntry.cursor] = *ev->key;
    gCheatEntry.cursor++;
    gCheatEntry.length++;
    gCheatEntryText[gCheatEntry.length] = 0;
    return 0;
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80408AC4_7.CPP, func_8043CC10_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *chr;
} ObjRecAt14;

extern s32 sEnterCheatResult;

/* Inserts the selected character at the cursor of the name entry. */
s32 EnterCheatMenu_InsertLetter_2(ObjRecAt14 *arg0) {
    s32 index;
    s32 count;

    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 0x16) {
        return 0;
    }
    index = 0x16;
    if (gCheatEntry.cursor < 0x16) {
        do {
            gCheatEntry.text[index] = gCheatEntry.text[index - 1];
            index--;
        } while (gCheatEntry.cursor < index);
    }
    gCheatEntry.text[gCheatEntry.cursor] = *arg0->chr;
    count = gCheatEntry.length + 1;
    gCheatEntry.cursor++;
    gCheatEntry.length = count;
    gCheatEntry.text[count] = 0;
    return 0;
}

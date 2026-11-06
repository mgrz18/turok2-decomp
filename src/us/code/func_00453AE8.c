/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80408AC4_7.CPP, func_8043CC10_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *chr;
} ObjRecAt14;

extern s32 D_80130920;

/* Inserts the selected character at the cursor of the name entry. */
s32 EnterCheatMenu_InsertLetter_2(ObjRecAt14 *arg0) {
    s32 index;
    s32 count;

    D_80130920 = -1;
    D_800C1F80.timer = 0;
    if (D_800C1F80.length == 0x16) {
        return 0;
    }
    index = 0x16;
    if (D_800C1F80.cursor < 0x16) {
        do {
            D_800C1F80.text[index] = D_800C1F80.text[index - 1];
            index--;
        } while (D_800C1F80.cursor < index);
    }
    D_800C1F80.text[D_800C1F80.cursor] = *arg0->chr;
    count = D_800C1F80.length + 1;
    D_800C1F80.cursor++;
    D_800C1F80.length = count;
    D_800C1F80.text[count] = 0;
    return 0;
}

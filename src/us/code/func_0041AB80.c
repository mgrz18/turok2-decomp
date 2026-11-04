#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *key;
} KeyEvent;

extern u8 D_800C1F8C[];
extern s32 D_80130920;

/* Inserts the event's character at the cursor (max 22 chars). */
s32 func_0041AB80(KeyEvent *ev) {
    s32 i;

    D_80130920 = -1;
    D_800C1F80.timer = 0;
    if (D_800C1F80.length == 22) {
        return 0;
    }
    for (i = 22; D_800C1F80.cursor < i; i--) {
        D_800C1F8C[i] = D_800C1F8C[i - 1];
    }
    D_800C1F8C[D_800C1F80.cursor] = *ev->key;
    D_800C1F80.cursor++;
    D_800C1F80.length++;
    D_800C1F8C[D_800C1F80.length] = 0;
    return 0;
}

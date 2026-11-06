/* WRONG CUT: the real function starts at 0x00456948 (size 0x234 = 564): splat
 * gave its first two words (lui/lw gPakMenuAnyPak) to the function before. */
#include "common.h"
#include "structs/menu.h"

/* Level select: grey out the entries (16) that cannot be chosen in the current
 * game mode. */

#define ENTRY_DISABLED 0x01000000

extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;
extern s32 gPakMenuMode;
extern char gPakGameCode[];
extern char gPakCompanyCode[];
extern char gPakCharacterNoteName[];
extern char gPakGameNoteName[];

s32 Pak_FindNote_2(s32 mode, s32 index, s32 *valid, char *name, void *arg4, s32 *arg5, char *arg6, char *arg7);
s32 Str_Length_2(char *s);
s32 Str_Compare_2(char *a, char *b);

static __inline__ s32 isSpecial(char *a, char *b) {
    if (Str_Length_2(b) == 4 && Str_Length_2(a) == 2 && Str_Compare_2(b, gPakGameCode) == 0 && Str_Compare_2(a, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}

static __inline__ void enableEntry(MenuItem *entry) {
    entry->flags &= ~ENTRY_DISABLED;
}

static __inline__ void disableEntry(MenuItem *entry) {
    entry->flags |= ENTRY_DISABLED;
}

void PakMenu_UpdateNoteRows_2(Menu *event) {
    s32 mode;
    s32 i;
    s32 enabled;
    s32 failed;
    s32 special;
    u8 sp20[8];
    char a[8];
    char b[8];
    char name[16];
    s32 valid;
    s32 sp4C;

    if (gPakMenuAnyPak != 0) {
        mode = gPakMenuPort;
    } else {
        mode = event->controller->port;
    }
    for (i = 0; i < 16; i++) {
        failed = Pak_FindNote_2(mode, i, &valid, name, sp20, &sp4C, a, b);
        enabled = 1;
        if (failed == 0 && valid != 0) {
            switch (gPakMenuMode) {
            case 1:
                special = isSpecial(a, b);
                if (Str_Compare_2(name, gPakCharacterNoteName) == 0 && special) {
                    enabled = 0;
                }
                break;
            case 2:
                special = isSpecial(a, b);
                if (Str_Compare_2(name, gPakGameNoteName) == 0 && special) {
                    enabled = 0;
                }
                break;
            case 0:
            default:
                enabled = 0;
                break;
            }
        }
        if (enabled) {
            enableEntry(&event->items[i + 3]);
        } else {
            disableEntry(&event->items[i + 3]);
        }
    }
}

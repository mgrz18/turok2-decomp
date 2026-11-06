#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Greys out the controller pak notes that cannot be picked: a used note is
 * selectable unless it is this game's save (gPakCharacterNoteName when loading, mode 1,
 * gPakGameNoteName in mode 2) with Turok 2's game and company codes. */

extern s32 Pak_FindNote(s32 slot, s32 note, s32 *used, char *name, char *ext, s32 *size, char *company, char *game);
extern s32 Str_Length(char *s);
extern s32 Str_Compare(char *a, char *b);

/* Game code / company code check on the controller-pak note (as in PakMenu_SelectNote). */
static __inline__ s32 k7_chk(char *gc, char *cc) {
    if (Str_Length(gc) == 4 && Str_Length(cc) == 2 &&
        Str_Compare(gc, gPakGameCode) == 0 && Str_Compare(cc, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}

static __inline__ void rowEnable(MenuItem *row) {
    row->flags &= ~0x01000000;
}

static __inline__ void rowDisable(MenuItem *row) {
    row->flags |= 0x01000000;
}

void PakMenu_UpdateNoteRows(Menu *menu) {
    char ext[8];
    char company[8];
    char game[8];
    char name[16];
    s32 used;
    s32 size;
    s32 slot;
    s32 i;
    s32 ok;
    s32 ours;
    s32 err;

    if (gPakMenuAnyPak) {
        slot = gPakMenuPort;
    } else {
        slot = menu->controller->port;
    }
    for (i = 0; i < 16; i++) {
        err = Pak_FindNote(slot, i, &used, name, ext, &size, company, game);
        ok = 1;
        if (err == 0 && used != 0) {
            switch (gPakMenuMode) {
            case 1:
                ours = k7_chk(game, company);
                if (Str_Compare(name, gPakCharacterNoteName) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 2:
                ours = k7_chk(game, company);
                if (Str_Compare(name, gPakGameNoteName) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 0:
            default:
                ok = 0;
                break;
            }
        }
        if (ok) {
            rowEnable(&menu->items[i + 3]);
        } else {
            rowDisable(&menu->items[i + 3]);
        }
    }
}

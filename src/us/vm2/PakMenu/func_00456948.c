#include "common.h"
#include "protos/pak_2.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/pak.h"

/* Greys out the controller pak notes that cannot be picked: a used note is
 * selectable unless it is this game's save (gPakCharacterNoteName when loading, mode 1,
 * gPakGameNoteName in mode 2) with Turok 2's game and company codes. */


/* Game code / company code check on the controller-pak note (as in PakMenu_SelectNote_2). */
static __inline__ s32 k7_chk(char *gc, char *cc) {
    if (Str_Length_2(gc) == 4 && Str_Length_2(cc) == 2 &&
        Str_Compare_2(gc, gPakGameCode) == 0 && Str_Compare_2(cc, gPakCompanyCode) == 0) {
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

void PakMenu_UpdateNoteRows_2(Menu *menu) {
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
        err = Pak_FindNote_2(slot, i, &used, name, ext, &size, company, game);
        ok = 1;
        if (err == 0 && used != 0) {
            switch (gPakMenuMode) {
            case 1:
                ours = k7_chk(game, company);
                if (Str_Compare_2(name, gPakCharacterNoteName) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 2:
                ours = k7_chk(game, company);
                if (Str_Compare_2(name, gPakGameNoteName) == 0 && ours) {
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

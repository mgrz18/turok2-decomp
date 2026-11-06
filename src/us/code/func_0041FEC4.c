#include "common.h"
#include "structs/menu.h"

/* Save/load menu "select pak" handler: checks the pak in the chosen slot;
 * when saving over an existing game file asks for confirmation, when there
 * is no usable pak cycles to the next slot with one. */

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_801309B4;
extern s32 D_801309BC;
extern s32 D_801309C4;
extern s32 D_800C2028;
extern s32 D_8011F148;
extern char D_800B45CC[];
extern u8 D_80119E2C[];
extern u8 D_0043B714[];
extern u8 D_0043C20C[];

extern s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet);
extern void PakMenu_UpdateNoteRows(Menu *c);
extern void PakMenu_WriteProfile(void *menu, Menu *c, s32 arg2);
extern void PakMenu_WriteSaveGame(void *menu, Menu *c, s32 arg2);
extern void Pak_ProbeSlot(s32 slot);
extern s32 Pak_GetStatus(s32 slot);
extern void Pak_GetNoteGameName(s32 slot, s32 index, char **out);
extern void Pak_DecodeText(char *src, char *dst, s32 n);
extern s32 Str_Compare(char *a, char *b);
extern void MenuPool_OpenMenu(void *menus, void *tmpl, s32 a, ControllerProfile *b, void *parent);

/* PakMenu_FindNoteByName, inlined (the found path sets the result before storing the index) */
static __inline__ s32 findFile(void *unused, char *file, s32 slot, s32 *index) {
    char name[16];
    char *entry;
    s32 found;
    s32 i;

    for (i = 0; i < 16; i++) {
        Pak_GetNoteGameName(slot, i, &entry);
        Pak_DecodeText(entry, name, 16);
        if (Str_Compare(name, file) == 0) {
            found = 1;
            if (index != NULL) {
                *index = i;
            }
            goto end;
        }
    }
    found = 0;
end:
    return found;
}

static __inline__ void menuSlot(Menu *c, s32 *slot) {
    if (D_80130990) {
        *slot = D_800C2028;
    } else {
        *slot = c->controller->port;
    }
}

s32 PakMenu_SelectPak(void *menu, Menu *c, s32 arg2) {
    s32 slot;
    s32 pak;
    s32 n;
    s32 next;
    s32 cur;

    slot = 0;
    if (D_80130984) {
        menuSlot(c, &slot);
    }
    if (D_80130990) {
        pak = D_800C2028;
    } else {
        pak = slot;
    }
    if (PakMenu_CheckPak(c, pak, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B4) {
        if (D_80130984) {
            PakMenu_WriteProfile(menu, c, arg2);
            return 1;
        }
    } else if (D_80130984) {
        D_801309C4 = -1;
        if (findFile(menu, D_800B45CC, pak, &D_801309C4) && D_801309C4 >= 0 && D_801309C4 < 16) {
            MenuPool_OpenMenu(D_80119E2C, D_0043C20C, c->player, c->controller, D_0043B714);
            return 1;
        }
        PakMenu_WriteSaveGame(menu, c, arg2);
        return 1;
    }
    if (D_80130990) {
        if (D_800C2028 == -1) {
            next = 0;
        } else {
            next = D_800C2028 + 1;
            if (next >= 4) {
                next = 0;
            }
        }
        for (n = 0; n < 4; n++) {
            Pak_ProbeSlot(next);
            if (Pak_GetStatus(next) != -2) {
                break;
            }
            next++;
            if (next >= 4) {
                next = 0;
            }
        }
        if (n == 4) {
            next = -1;
        }
        D_800C2028 = next;
        if (next == -1) {
            D_8011F148 = D_801309BC = 1;
            return 1;
        }
        if (D_80130990) {
            cur = D_800C2028;
        } else {
            cur = c->controller->port;
        }
        slot = cur;
        if (PakMenu_CheckPak(c, cur, 0) != 0) {
            D_801309BC = 1;
            return 1;
        }
        PakMenu_UpdateNoteRows(c);
    }
    return 0;
}

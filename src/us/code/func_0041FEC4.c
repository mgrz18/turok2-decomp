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

extern s32 func_0041DD90(Menu *c, s32 slot, s32 quiet);
extern void func_0041DA28(Menu *c);
extern void func_0041EDE0(void *menu, Menu *c, s32 arg2);
extern void func_0041F0C8(void *menu, Menu *c, s32 arg2);
extern void func_00426480(s32 slot);
extern s32 func_0042655C(s32 slot);
extern void func_00426978(s32 slot, s32 index, char **out);
extern void func_00426C88(char *src, char *dst, s32 n);
extern s32 func_00426D54(char *a, char *b);
extern void func_00416644(void *menus, void *tmpl, s32 a, MenuController *b, void *parent);

/* func_00421A54, inlined (the found path sets the result before storing the index) */
static __inline__ s32 findFile(void *unused, char *file, s32 slot, s32 *index) {
    char name[16];
    char *entry;
    s32 found;
    s32 i;

    for (i = 0; i < 16; i++) {
        func_00426978(slot, i, &entry);
        func_00426C88(entry, name, 16);
        if (func_00426D54(name, file) == 0) {
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

s32 func_0041FEC4(void *menu, Menu *c, s32 arg2) {
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
    if (func_0041DD90(c, pak, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B4) {
        if (D_80130984) {
            func_0041EDE0(menu, c, arg2);
            return 1;
        }
    } else if (D_80130984) {
        D_801309C4 = -1;
        if (findFile(menu, D_800B45CC, pak, &D_801309C4) && D_801309C4 >= 0 && D_801309C4 < 16) {
            func_00416644(D_80119E2C, D_0043C20C, c->player, c->controller, D_0043B714);
            return 1;
        }
        func_0041F0C8(menu, c, arg2);
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
            func_00426480(next);
            if (func_0042655C(next) != -2) {
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
        if (func_0041DD90(c, cur, 0) != 0) {
            D_801309BC = 1;
            return 1;
        }
        func_0041DA28(c);
    }
    return 0;
}

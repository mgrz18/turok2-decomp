/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004082FC.c, func_004082FC, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"

/* Controller pak menu, confirm: checks the pak, then saves (replacing the game's note when one exists, with a
 * message when it is in use) or, without a pak, looks for the next pad that has one. */

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_801309B4;
extern s32 D_801309BC;
extern s32 D_801309C4; /* note index */
extern s32 D_800C2028; /* pad */
extern char D_800B45CC[];
extern s32 D_8011F148;
extern u8 D_80119E2C[];
extern u8 D_0043C12C[];
extern u8 D_0043B634[];
s32 func_00426C74(u8 *a, char *b);
void func_004263A0(s32 a);
s32 func_0042647C(s32 a);
void func_00426898(s32 a, s32 b, s32 *c);
void func_00426BA8(s32 a, u8 *b, s32 c);
void func_0041D948(Menu *o);
s32 func_0041DCB0(Menu *o, s32 a, s32 b);
void func_0041ED00(s32 a, Menu *o, s32 c);
void func_0041EFE8(s32 a, Menu *o, s32 c);
void func_00416644(void *a, void *b, s32 c, MenuController *d, void *e);

s32 func_00458DE4(s32 a0, Menu *o, s32 a2) {
    u8 buf[16];
    struct {
        s32 pad;
        s32 v;
    } L;
    s32 s0, s1, found;

    L.pad = 0;
    if (D_80130984 != 0) {
        if (D_80130990 != 0) {
            L.pad = D_800C2028;
        } else {
            L.pad = o->controller->port;
        }
    }
    if (D_80130990 != 0) {
        s1 = D_800C2028;
    } else {
        s1 = L.pad;
    }
    if (func_0041DCB0(o, s1, 0) != 0) {
        goto done;
    }
    if (D_801309B4 != 0) {
        if (D_80130984 != 0) {
            func_0041ED00(a0, o, a2);
            return 1;
        }
    } else if (D_80130984 != 0) {
        s32 *index;

        D_801309C4 = -1;
        /* Probably the optional out-pointer of an inlined search helper: tested through a local, stored by symbol. */
        index = &D_801309C4;
        s0 = 0;
        do {
            func_00426898(s1, s0, &L.v);
            func_00426BA8(L.v, buf, 0x10);
            if (func_00426C74(buf, D_800B45CC) == 0) {
                found = 1;
                if (index != NULL) {
                    D_801309C4 = s0;
                }
                goto after;
            }
            s0++;
        } while (s0 < 0x10);
        found = 0;
    after:
        if (found != 0 && D_801309C4 >= 0 && D_801309C4 < 0x10) {
            func_00416644(D_80119E2C, D_0043C12C, o->player, o->controller, D_0043B634);
            return 1;
        }
        func_0041EFE8(a0, o, a2);
        return 1;
    }
    if (D_80130990 != 0) {
        s32 r = D_800C2028;

        s0 = 0;
        if (r != -1) {
            s0 = r + 1;
            if (s0 >= 4) {
                s0 = 0;
            }
        }
        s1 = 0;
        do {
            func_004263A0(s0);
            if (func_0042647C(s0) != -2) {
                break;
            }
            s0++;
            if (s0 >= 4) {
                s0 = 0;
            }
            s1++;
        } while (s1 < 4);
        if (s1 == 4) {
            s0 = -1;
        }
        D_800C2028 = s0;
        if (s0 == -1) {
            D_8011F148 = D_801309BC = 1;
            return 1;
        }
        {
            s32 a1 = s0;

            if (D_80130990 == 0) {
                a1 = o->controller->port;
            }
            L.pad = a1;
            if (func_0041DCB0(o, a1, 0) != 0) {
            done:
                D_801309BC = 1;
                return 1;
            }
            func_0041D948(o);
        }
    }
    return 0;
}

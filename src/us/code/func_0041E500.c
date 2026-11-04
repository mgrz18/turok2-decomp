/* REAL func_0041E500 size 0x584 (0x0041E500-0x0041EA84); splat cut it into func_0041E500 + func_0041E7D0 + func_0041E824 + func_0041E97C. Matches whole (checked with the size overridden). Twin of src/us/code/func_00457420.c. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004069F4.c, func_004069F4, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"

/* Controller pak menu, load: finds a pad with a controller pak (trying each pad in turn), checks it for the
 * game's notes and free space and opens the load menu, or shows why the pak cannot be used. */

extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_8011F144;
extern s32 D_800C202C;
extern s32 D_8011F13C;
extern s32 D_800C2028; /* pad */
extern s32 D_8011B108;
extern s32 D_8011B114;
extern char D_800B45C0[];
extern char D_800B45CC[];
extern s32 D_800F70A8;
extern u8 D_80119E2C[];
extern u8 D_800F6CB0[];
extern u8 D_0043BCFC[];
extern u8 D_0043BCB4[];
extern u8 D_0043C05C[];
extern u8 D_0043B738[];
extern u8 D_0043C1E8[];
extern u8 D_0043C1C4[];
s32 func_00285998(void);
void func_002886AC(u8 *a);
s32 func_00426D54(u8 *a, char *b);
void func_00426480(s32 a);
s32 func_0042655C(s32 a);
s32 func_0042670C(s32 a, s32 *b);
s32 func_004267A0(s32 a, s32 *b);
void func_00426978(s32 a, s32 b, s32 *c);
s32 func_00426BD8(s32 a);
void func_00426C88(s32 a, u8 *b, s32 c);
s32 func_00426E6C(s32 a);
void func_0041DA28(Menu *o);
s32 func_0041E374(Menu *o, s32 a, s32 *b, s32 *c);
void func_00416644(u8 *a, u8 *b, s32 c, MenuController *d, void *e);

#define PAK_ERROR()                                                        \
    D_801309BC = 1;                                                        \
    D_8011F144 = 0;                                                        \
    func_00416644(D_80119E2C, D_0043BCB4, arg0->player, arg0->controller, NULL); \
    D_8011F13C = 0;                                                        \
    D_800C202C = 1;

void func_0041E500(Menu *arg0) {
    u8 buf[16];
    s32 sp28, sp2C, sp30, sp34, sp38, sp3C;
    s32 s0, s1, s2, s3, f1, v0, m1, r2;

    D_80130984 = 0;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 1;
    if (func_00285998() == 0) {
        D_801309BC = 1;
        D_8011F144 = 0;
        func_00416644(D_80119E2C, D_0043BCFC, arg0->player, arg0->controller, NULL);
        D_800C202C = 1;
        D_8011F13C = 0;
        return;
    }
    s0 = 0;
    if (D_800C2028 == -1) {
        s1 = 0;
        do {
            func_00426480(s0);
            if (func_0042655C(s0) != -2) {
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
            PAK_ERROR()
            return;
        }
    }
    if (func_00426BD8(D_800C2028) != 0) {
        goto blk59;
    }
    func_002886AC(D_800F6CB0);
    if ((arg0->controller->buttons & 0x1000) && D_800C202C == 0) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    D_800C202C = 1;
    if (D_8011B114 != 0 || D_8011B108 != 0) {
        func_00426480(D_800C2028);
        if (func_0042655C(D_800C2028) != -2) {
            goto fin;
        }
        PAK_ERROR()
        return;
    }
    s3 = 0;
    s2 = s3;
    sp2C = -1;
    sp28 = D_800C2028;
    do {
        if (D_800C2028 != -1) {
            s3 = func_0041E374(arg0, D_800C2028, &sp28, &sp2C);
            if (s3 != 0) {
                goto found;
            }
        }
        s0 = 0;
        if (D_800C2028 != -1) {
            s0 = D_800C2028 + 1;
            if (s0 >= 4) {
                s0 = 0;
            }
        }
        s1 = 0;
        do {
            func_00426480(s0);
            if (func_0042655C(s0) != -2) {
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
        s2++;
    } while (s2 < 4);
    if (s3 == 0) {
        D_800C2028 = sp28;
        func_00426480(sp28);
        if (func_0042655C(D_800C2028) == -2) {
            PAK_ERROR()
            return;
        }
    }
found:
    s1 = D_800C2028;
    s0 = 0;
    do {
        func_00426978(s1, s0, &sp30);
        func_00426C88(sp30, buf, 0x10);
        if (func_00426D54(buf, D_800B45C0) == 0) {
            f1 = 1;
            goto L2;
        }
        s0++;
    } while (s0 < 0x10);
    f1 = 0;
L2:
    s1 = D_800C2028;
    s0 = 0;
    do {
        func_00426978(s1, s0, &sp34);
        func_00426C88(sp34, buf, 0x10);
        if (func_00426D54(buf, D_800B45CC) == 0) {
            v0 = 1;
            goto L3;
        }
        s0++;
    } while (s0 < 0x10);
    v0 = 0;
L3:
    if (f1 != 0) {
        goto fin;
    }
    if (v0 != 0) {
        goto fin;
    }
    if (func_004267A0(D_800C2028, &sp38) != 0) {
        goto blk59;
    }
    s0 = func_0042670C(D_800C2028, &sp3C);
    if (s0 != 0) {
        goto blk59;
    }
    m1 = func_00426E6C(D_800F70A8 + 0x688);
    r2 = func_00426E6C(0x18);
    if (sp3C == 0 || sp38 < r2) {
        goto L56;
    }
    if (sp38 < m1) {
        D_801309BC = 1;
        D_8011B108 = 1;
        func_00416644(D_80119E2C, D_0043C1E8, arg0->player, arg0->controller, D_0043B738);
        goto after;
    }
    if (sp38 < r2) {
        goto L56;
    }
    goto after;
L56:
    D_801309BC = 1;
    D_8011B108 = 1;
    func_00416644(D_80119E2C, D_0043C1C4, arg0->player, arg0->controller, D_0043B738);
after:
    if (s0 == 0) {
        goto fin;
    }
blk59:
    D_801309BC = 1;
    D_8011B108 = 1;
    func_00416644(D_80119E2C, D_0043C05C, arg0->player, arg0->controller, D_0043B738);
fin:
    if (D_801309BC == 0) {
        func_0041DA28(arg0);
    }
    D_8011F13C = 0;
    D_800C202C = 1;
}

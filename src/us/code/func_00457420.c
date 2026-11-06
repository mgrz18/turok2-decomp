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
extern u8 gEngineApp[];
extern u8 D_0043BC1C[];
extern u8 D_0043BBD4[];
extern u8 D_0043BF7C[];
extern u8 D_0043B658[];
extern u8 D_0043C108[];
extern u8 D_0043C0E4[];
s32 Controller_AnyConnected(void);
void CEngineApp__UpdateControllers(u8 *a);
s32 func_00426C74(u8 *a, char *b);
void func_004263A0(s32 a);
s32 func_0042647C(s32 a);
s32 func_0042662C(s32 a, s32 *b);
s32 func_004266C0(s32 a, s32 *b);
void func_00426898(s32 a, s32 b, s32 *c);
s32 func_00426AF8(s32 a);
void func_00426BA8(s32 a, u8 *b, s32 c);
s32 Pak_BytesToPages_2(s32 a);
void func_0041D948(Menu *o);
s32 func_0041E294(Menu *o, s32 a, s32 *b, s32 *c);
void MenuPool_OpenMenu(u8 *a, u8 *b, s32 c, ControllerProfile *d, void *e);

#define PAK_ERROR()                                                        \
    D_801309BC = 1;                                                        \
    D_8011F144 = 0;                                                        \
    MenuPool_OpenMenu(D_80119E2C, D_0043BBD4, arg0->player, arg0->controller, NULL); \
    D_8011F13C = 0;                                                        \
    D_800C202C = 1;

void PakMenu_InitSaveGameAnyPak_2(Menu *arg0) {
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
    if (Controller_AnyConnected() == 0) {
        D_801309BC = 1;
        D_8011F144 = 0;
        MenuPool_OpenMenu(D_80119E2C, D_0043BC1C, arg0->player, arg0->controller, NULL);
        D_800C202C = 1;
        D_8011F13C = 0;
        return;
    }
    s0 = 0;
    if (D_800C2028 == -1) {
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
            PAK_ERROR()
            return;
        }
    }
    if (func_00426AF8(D_800C2028) != 0) {
        goto blk59;
    }
    CEngineApp__UpdateControllers(gEngineApp);
    if ((arg0->controller->buttons & 0x1000) && D_800C202C == 0) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    D_800C202C = 1;
    if (D_8011B114 != 0 || D_8011B108 != 0) {
        func_004263A0(D_800C2028);
        if (func_0042647C(D_800C2028) != -2) {
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
            s3 = func_0041E294(arg0, D_800C2028, &sp28, &sp2C);
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
        s2++;
    } while (s2 < 4);
    if (s3 == 0) {
        D_800C2028 = sp28;
        func_004263A0(sp28);
        if (func_0042647C(D_800C2028) == -2) {
            PAK_ERROR()
            return;
        }
    }
found:
    s1 = D_800C2028;
    s0 = 0;
    do {
        func_00426898(s1, s0, &sp30);
        func_00426BA8(sp30, buf, 0x10);
        if (func_00426C74(buf, D_800B45C0) == 0) {
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
        func_00426898(s1, s0, &sp34);
        func_00426BA8(sp34, buf, 0x10);
        if (func_00426C74(buf, D_800B45CC) == 0) {
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
    if (func_004266C0(D_800C2028, &sp38) != 0) {
        goto blk59;
    }
    s0 = func_0042662C(D_800C2028, &sp3C);
    if (s0 != 0) {
        goto blk59;
    }
    m1 = Pak_BytesToPages_2(D_800F70A8 + 0x688);
    r2 = Pak_BytesToPages_2(0x18);
    if (sp3C == 0 || sp38 < r2) {
        goto L56;
    }
    if (sp38 < m1) {
        D_801309BC = 1;
        D_8011B108 = 1;
        MenuPool_OpenMenu(D_80119E2C, D_0043C108, arg0->player, arg0->controller, D_0043B658);
        goto after;
    }
    if (sp38 < r2) {
        goto L56;
    }
    goto after;
L56:
    D_801309BC = 1;
    D_8011B108 = 1;
    MenuPool_OpenMenu(D_80119E2C, D_0043C0E4, arg0->player, arg0->controller, D_0043B658);
after:
    if (s0 == 0) {
        goto fin;
    }
blk59:
    D_801309BC = 1;
    D_8011B108 = 1;
    MenuPool_OpenMenu(D_80119E2C, D_0043BF7C, arg0->player, arg0->controller, D_0043B658);
fin:
    if (D_801309BC == 0) {
        func_0041D948(arg0);
    }
    D_8011F13C = 0;
    D_800C202C = 1;
}

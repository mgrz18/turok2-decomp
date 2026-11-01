/* NON-MATCHING: 5 of 112 words differ (W batch). */
/* NON-MATCHING: 5 of 112 words differ: the D_800C2020 branch passes plr from $v0 instead of $s0, and the checksum store loads the buffer and size globals into swapped registers. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00406F7C.c, func_00406F7C, with Turok 2's layouts. */
#include "common.h"

/* Controller pak menu, "save" entry: when the pak is ready, builds the save note (with its checksum) and
 * opens the save menu; otherwise shows why the pak cannot be used. */

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *hud;
    /* 0x520 */ u8 pad520[0xA50 - 0x520];
    /* 0xA50 */ s32 controller;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 pad;
} MenuPad;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
    /* 0x20 */ MenuPad *pad;
} MenuOwner;

typedef struct {
    s32 v;
    s32 w;
} PadSel;

extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern s32 D_80130940;
extern u8 *D_800C2018; /* save buffer */
extern s32 D_800F70A8; /* save size */
extern u8 D_80119E2C[];
extern u8 D_0043BE14[];
extern u8 D_0043BB68[];
extern u8 D_0043BF7C[];
extern u8 D_0043B634[];
extern u8 D_0043BB20[];
void func_004263A0(s32 ch);
s32 func_0042647C(s32 ch);
s32 func_004264B4(s32 ch);
s32 func_00426AF8(s32 ch);
void func_0041D7B0(Player *p);
void func_0041D948(MenuOwner *o);
u32 func_00426D98(u8 *data, s32 len, u32 crc);
void func_00416644(u8 *a, void *b, void *c, s32 d, void *e);

void func_004579A4(MenuOwner *own) {
    Player *plr;
    PadSel sel;

    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_800C2024 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    sel.v = own->pad->pad;
    {
        register Player *pp asm("$2") = own->player;

        plr = pp;
    }
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        func_00416644(plr->hud + 0x53C, D_0043BE14, plr, plr->controller, NULL);
        D_80130940 = 0;
    } else {
        func_004263A0(own->pad->pad);
        if (func_0042647C(sel.v) == 0) {
            func_0041D7B0(plr);
            *(u32 *)(D_800C2018 + D_800F70A8 + 0x684) = func_00426D98(D_800C2018, D_800F70A8 + 0x684, 0xB);
            func_0041D948(own);
        } else {
            register s32 one asm("$17") = 1;

            D_801309BC = one;
            if (func_004264B4(sel.v) != 0) {
                func_00416644(plr->hud + 0x53C, D_0043BB68, plr, plr->controller, (void *)one);
            } else if (func_00426AF8(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043BF7C, plr, plr->controller, D_0043B634);
            } else {
                func_00416644(plr->hud + 0x53C, D_0043BB20, plr, plr->controller, D_0043BE14);
            }
        }
    }
}

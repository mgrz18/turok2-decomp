/* FAKEMATCH: two register asm pins and a second player pointer stand for what natural C does not give: the ROM
 * loads the player into $v0 and copies it to $s0 (pp, then p as the copy the branches use), and the constant 1 of
 * the error branch gets its own register (natural C shares one $s register for every 1: cse2 follows the jumps
 * into that branch). Twin of func_004579A4 (same C). */
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
extern u8 D_0043BEF4[];
extern u8 D_0043BC48[];
extern u8 D_0043C05C[];
extern u8 D_0043B714[];
extern u8 D_0043BC00[];
void func_00426480(s32 ch);
s32 func_0042655C(s32 ch);
s32 func_00426594(s32 ch);
s32 func_00426BD8(s32 ch);
void func_0041D890(Player *p);
void func_0041DA28(MenuOwner *o);
u32 func_00426E78(u8 *data, s32 len, u32 crc);
void func_00416644(u8 *a, void *b, void *c, s32 d, void *e);

void func_0041EA84(MenuOwner *own) {
    Player *plr;
    Player *p;
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
        p = plr;
        D_801309BC = 1;
        func_00416644(p->hud + 0x53C, D_0043BEF4, p, p->controller, NULL);
        D_80130940 = 0;
    } else {
        func_00426480(own->pad->pad);
        p = plr;
        if (func_0042655C(sel.v) == 0) {
            func_0041D890(plr);
            {
                u32 crc = func_00426E78(D_800C2018, D_800F70A8 + 0x684, 0xB);
                s32 buf = (s32)D_800C2018;
                s32 size = D_800F70A8;

                *(u32 *)(size + buf + 0x684) = crc;
            }
            func_0041DA28(own);
        } else {
            register s32 one asm("$17") = 1;

            D_801309BC = one;
            if (func_00426594(sel.v) != 0) {
                func_00416644(p->hud + 0x53C, D_0043BC48, p, p->controller, (void *)one);
            } else if (func_00426BD8(sel.v) != 0) {
                func_00416644(D_80119E2C, D_0043C05C, p, p->controller, D_0043B714);
            } else {
                func_00416644(p->hud + 0x53C, D_0043BC00, p, p->controller, D_0043BEF4);
            }
        }
    }
}

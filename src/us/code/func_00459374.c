/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408870.c, func_00408870, with Turok 2's layouts. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"

/* Controller pak menu, load: after the pak check, copies the loaded save into the player and the game state
 * and shows the "loaded" message; otherwise reports whether the note could be read. */

typedef struct {
    s32 w[0x4E8 / 4];
} SaveBlock18;

typedef struct {
    s32 w[0x184 / 4];
} SaveBlock500;

typedef struct {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ f32 unk8;
    /* 0x00C */ f32 unkC;
    /* 0x010 */ f32 unk10;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ SaveBlock18 blk18;
    /* 0x500 */ SaveBlock500 blk500;
    /* 0x684 */ u8 unk684[1];
} SaveData;

typedef struct {
    /* 0x000 */ u8 pad0[0x520];
    /* 0x520 */ SaveBlock18 unk520;
    /* 0xA08 */ u8 padA08[0xA50 - 0xA08];
    /* 0xA50 */ s32 controller;
} Player;


extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_801309BC;
extern s32 D_801309B8;
extern s32 D_801309C4;
extern CWorld D_800F7078[];
extern SaveData *D_800C201C;
extern u8 D_8011AC58;
extern s32 D_8011AADC;
extern f32 D_8011ACB4[];
extern f32 D_8011ACB8[];
extern f32 D_8011ACBC[];
extern s32 D_80110060;
extern u8 D_800EBAC0[];
extern u8 D_80119E2C[];
extern u8 D_0043BC40[];
extern u8 D_0043BCD0[];
extern u8 D_0043BD3C[];
s32 func_0041DCB0(Menu *o, s32 pad, s32 z);
Player *func_0025342C(CPlayerList *list, ControllerProfile *s);
s32 func_00425F84(s32 pad, s32 idx);
void func_00416644(u8 *a, void *b, void *c, s32 d, s32 e);

s32 func_00459374(s32 unused, Menu *own) {
    f32 *new_var;
    s32 pad;
    Player *plr;
    CWorld *g;
    u8 save;
    s32 i;
    SaveData *c;
    u8 *src;
    u8 *dp;

    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->controller->port;
    }
    if (func_0041DCB0(own, pad, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B8 != 0) {
        plr = func_0025342C(&D_80119870.m_PlayerList, own->controller);
        g = D_800F7078;
        save = D_8011AC58;
        plr->unk520 = D_800C201C->blk18;
        {
            *(SaveBlock500 *)&D_8011AAD8 = D_800C201C->blk500;
            c = D_800C201C;
        }
        new_var = &c->unkC;
        D_8011ACB0.frames = c->unk8;
        D_8011ACB4[0] = *new_var;
        D_8011ACB8[0] = c->unk10;
        D_80110060 = D_8011AADC;
        D_8011AC58 = save;
        D_8011ACBC[0] = c->unk14;
        src = c->unk684;
        dp = D_800EBAC0;
        for (i = 0; i < g->unk_30; i++) {
            *dp++ = *src++;
        }
        D_80119870.m_PlayerList.m_bCinema = 0;
        D_80119870.m_PlayerList.m_bCinemaStarted = 0;
        func_00416644(D_80119870.m_MenuPool, D_0043BC40, plr, plr->controller, 0);
        return 1;
    }
    if (func_00425F84(pad, D_801309C4) == 0) {
        func_00416644(D_80119E2C, D_0043BCD0, own->player, own->controller, own->context);
    } else {
        func_00416644(D_80119E2C, D_0043BD3C, own->player, own->controller, own->context);
    }
    return 1;
}

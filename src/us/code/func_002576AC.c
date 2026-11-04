#include "common.h"

extern f32 D_800A723C;
extern f32 D_800A7240;

/* The player (CPlayer, structs/player.h) seen by raw offset: LibTEngine's
 * layout is 4 bytes off around 0xA00-0xC10, so these stay local. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x18];
    /* 0x0018 */ s32 velX;   /* m_CharacterActor.m_vVelocity, cleared as words */
    /* 0x001C */ s32 velY;
    /* 0x0020 */ s32 velZ;
    /* 0x0024 */ u8 pad24[0xD4 - 0x24];
    /* 0x00D4 */ s32 actorFlags;   /* m_CharacterActor.m_dwFlags */
    /* 0x00D8 */ u8 padD8[0xE2 - 0xD8];
    /* 0x00E2 */ s8 unkE2;
    /* 0x00E3 */ u8 padE3[0xA64 - 0xE3];
    /* 0x0A64 */ s32 buttons;
    /* 0x0A68 */ s32 buttonsSingle;
    /* 0x0A6C */ u8 padA6C[0xA78 - 0xA6C];
    /* 0x0A78 */ f32 unkA78;
    /* 0x0A7C */ f32 unkA7C;
    /* 0x0A80 */ u8 padA80[0xBFC - 0xA80];
    /* 0x0BFC */ s32 unkBFC;
    /* 0x0C00 */ u8 padC00[0xC04 - 0xC00];
    /* 0x0C04 */ s32 charAnim;
    /* 0x0C08 */ u8 padC08[0x1298 - 0xC08];
    /* 0x1298 */ s32 unk1298;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 useLowerAmmo;
} PlayerRaw;

void func_002576AC(PlayerRaw *arg0) {
    f32 temp_f1;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->charAnim == 0x1144) {
        var_a1 = arg0->unkE2 == 0;
    }
    if (var_a1 == 0) {
        temp_f1 = arg0->unkA78;
        if (D_800A723C <= temp_f1) {
            arg0->charAnim = 0x8A2;
            return;
        }
        if (temp_f1 <= D_800A7240) {
            arg0->charAnim = 0x8A7;
            return;
        }
        arg0->charAnim = 0x14;
    }
}

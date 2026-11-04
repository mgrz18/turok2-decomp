#include "common.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 damping;
} Unk800A71A8;
extern s32 D_800A71A8;   /* an Unk800A71A8 */

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

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
} Unk00255B20;

/* Byte-offset field access keeps the original load order. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void func_00255B20(PlayerRaw *arg0, Unk00255B20 *arg1) {
    FIELD_AT(arg0, f32, PlayerRaw, unkA78) = (FIELD_AT(arg0, f32, PlayerRaw, unkA78) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
    FIELD_AT(arg0, f32, PlayerRaw, unkA7C) = (FIELD_AT(arg0, f32, PlayerRaw, unkA7C) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
    FIELD_AT(arg1, f32, Unk00255B20, unk1C) = (FIELD_AT(arg1, f32, Unk00255B20, unk1C) * FIELD_AT(&D_800A71A8, f32, Unk800A71A8, damping));
}

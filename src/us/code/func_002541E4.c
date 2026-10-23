#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 field3E;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 index;
} Intelligence;

typedef struct {
    /* 0x000 */ u8 pad0[0x19];
    /* 0x019 */ u8 field19;
    /* 0x01A */ u8 pad1A[0x20C - 0x1A];
    /* 0x20C */ s32 field20C;
} State8011AAD8;

extern State8011AAD8 D_8011AAD8;
extern s32 D_800ADB5C;
extern s32 D_800ADB30[];
extern u8 D_800F7078[];
extern void func_002241D8(void *, CPlayer *, s32, s32);

/* 0xC04 is an s32 id, not LibTEngine's f32 m_WaitDeathmatchTimer. */
#define PLAYER_C04(p) (*(s32 *)&(p)->m_WaitDeathmatchTimer)

void func_002541E4(CPlayer *player) {
    State8011AAD8 *state = &D_8011AAD8;
    s32 id;

    if (state->field19 == 0) {
        id = 0x66;
    } else if (state->field20C != 0
               && ((PlayerOptions *)player->m_pOptions)->field3E != 0) {
        id = D_800ADB5C;
    } else {
        id = D_800ADB30[((Intelligence *)player->m_CharacterActor.header.m_pIntelligence)->index];
    }
    func_002241D8(D_800F7078, player, id, PLAYER_C04(player));
}

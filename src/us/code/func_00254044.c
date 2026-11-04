#include "common.h"
#include "structs/player.h"

/* A spawn point record (from func_00225EBC). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 pos[3];
} Spawn;

Spawn *func_00225EBC(s32, s32, s32);
extern s32 D_800F7078;

/* Picks a spawn point for the player and copies its position. */
void func_00254044(CPlayer *arg0) {
    s32 *var_s1;
    s32 var_a2;
    Spawn *var_v0;
    CPlayer *var_s0;

    var_s0 = arg0;
    var_a2 = var_s0->m_Data.m_nDeathmatchCharacter;
    if (var_s0->m_pOptions->m_TagMode == 1) {
        var_a2 = 0xB;
    }
    var_s1 = &D_800F7078;
    var_s1 = &D_800F7078;
    var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, var_a2);
    if (var_v0 == 0) {
        var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, -1);
        if (var_v0 == 0) {
            var_v0 = func_00225EBC((s32) &D_800F7078, -1, -1);
        }
    }
    var_s0->m_CharacterActor.header.m_pIntelligence = var_v0;
    var_s0->m_CharacterActor.m_vScale.x = (f32) var_v0->pos[0];
    var_s0->m_CharacterActor.m_vScale.y = (f32) var_v0->pos[1];
    var_s0->m_CharacterActor.m_vScale.z = (f32) var_v0->pos[2];
}

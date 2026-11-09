#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/player.h"
#include "protos/scene.h"
#include "protos/player.h"

/* A 0x64-byte record of the world model sections (CScene__GetRegionInfo finds it from an id). */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0x64 - 0x48];
} ModelRecord; /* size 0x64 */

/* What CScene__FindIntelligence finds (id 0xC45): three floats at 0x38. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} Found0xC45;


s32 Cinema_SetNextCallbacks(s32, s32, s32 *);     /* extern */
extern s32 PlayerDeath_CinemaDone;

void CPlayer__StartDeathCinema(CPlayer *arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    sptr temp_a1;
    ModelRecord *temp_v0;
    Found0xC45 *temp_v0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_v1;

    var_s0 = 1;
    temp_a1 = arg0->m_CharacterActor.header.m_pCurrentRegion;
    var_s1 = 0;
    if (temp_a1 != 0) {
        temp_v0 = CScene__GetRegionInfo(&gScene, temp_a1);
        if (temp_v0 != 0) {
            var_s1 = temp_v0->flags;
        }
    }
    if (gScene.m_bStreamAll != 0) {
        var_s0 = 0;
    }
    temp_a0 = arg0->m_ModeFlags & 0x8000;
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (gScene.m_bStreamAll != 0) {
        var_s0 = 0;
    }
    if (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) {
        var_s0 = 0;
    }
    if (arg2 == 0xC) {
        var_s0 = 0;
    }
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (var_s1 & 0x01000000) {
        var_s0 = 0;
    }
    if ((var_s1 & 0x80000) && (arg2 != 0x14) && (arg2 != 0x28) && (arg2 != 0x1E)) {
        var_s0 = 0;
    }
    arg0->m_DoDeathCin = var_s0;
    if (var_s0 != 0) {
        if (arg2 == 0x32) {
            gDeathTint = 1;
            var_v1 = 0;
            do {
                gDeathTintColor[var_v1] = arg0->m_pCamera->m_FogColor[var_v1];
                var_v1 += 1;
            } while (var_v1 < 4);
        }
        gCinemaHidesActors = 0;
        sDeathCinemaPlayer = arg0;
        Cinema_SetNextCallbacks(arg2, 0, &PlayerDeath_CinemaDone);
    }
    temp_v0_2 = CScene__FindIntelligence(&gScene, -1, 0xC45);
    if (temp_v0_2 != 0) {
        arg0->m_CharacterActor.m_vScale.x = temp_v0_2->unk38;
        arg0->m_CharacterActor.m_vScale.y = temp_v0_2->unk3C;
        arg0->m_CharacterActor.m_vScale.z = temp_v0_2->unk40;
    }
}

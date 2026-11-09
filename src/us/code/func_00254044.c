#include "common.h"
#include "structs/player.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* A spawn point record (from CScene__FindIntelligence). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 pos[3];
} Spawn;


/* Picks a spawn point for the player and copies its position. */
void CPlayer__PickSpawnPoint(CPlayer *arg0) {
    s32 *var_s1;
    s32 var_a2;
    Spawn *var_v0;
    CPlayer *var_s0;

    var_s0 = arg0;
    var_a2 = var_s0->m_Data.m_nDeathmatchCharacter;
    if (var_s0->m_pOptions->m_TagMode == 1) {
        var_a2 = 0xB;
    }
    var_s1 = &gScene;
    var_s1 = &gScene;
    var_v0 = CScene__FindIntelligence(&gScene, 0xB, var_a2);
    if (var_v0 == 0) {
        var_v0 = CScene__FindIntelligence(&gScene, 0xB, -1);
        if (var_v0 == 0) {
            var_v0 = CScene__FindIntelligence(&gScene, -1, -1);
        }
    }
    var_s0->m_CharacterActor.header.m_pIntelligence = var_v0;
    var_s0->m_CharacterActor.m_vScale.x = (f32) var_v0->pos[0];
    var_s0->m_CharacterActor.m_vScale.y = (f32) var_v0->pos[1];
    var_s0->m_CharacterActor.m_vScale.z = (f32) var_v0->pos[2];
}

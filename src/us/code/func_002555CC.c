#ifdef TARGET_PORT
/* The port: typed on CGameObjectInstance (the m2c fields were N64 offsets: the velocity at 0x1C,
 * the definition at 0x14); the platform pointer-wide; the jump sound takes the position as the
 * CVector3 Sound__Play is defined with (three s32 words went in integer registers). */
#include "common.h"
#include "structs/game_object.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "structs/intelligence.h"
#include <string.h>

sptr GameObject_GetPlatformBelow(CGameObjectInstance *);
void GameObject_GetFrameDelta(sptr, CVector3 *);
s32 Sound__Play(s16, CVector3, sptr, sptr);
void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.pad08) + 4 == 0x20, "CIntelligence 0x20");

void PlayerJump_Enter(void *arg0, CGameObjectInstance *arg1) {
    CVector3 delta;
    f32 speed;
    sptr platform;

    speed = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        speed = 481.28f;
    }
    {
        /* The jump scale, a float at 0x20 of the player's intelligence: inside CPlayerIntelligence's
         * pad08, which the archive's swap plan leaves big-endian (1.0f read 4.6e-41 little-endian,
         * so every jump left with no upward speed: demo 3 fell at gf 71 where the N64 jumps).
         * Read as the N64 does, as CPlayer__UpdateSpeedScale reads 0x1C (round 11 z5). */
        const u8 *b = (const u8 *) arg1->header.m_pIntelligence + 0x20;
        u32 w = ((u32) b[0] << 24) | ((u32) b[1] << 16) | ((u32) b[2] << 8) | b[3];
        f32 scale;
        memcpy(&scale, &w, sizeof(scale));
        arg1->m_vVelocity.y = speed * scale;
    }
    platform = GameObject_GetPlatformBelow(arg1);
    if (platform != 0) {
        GameObject_GetFrameDelta(platform, &delta);
        CVector3__Add(&arg1->m_vVelocity, &arg1->m_vVelocity, &delta);
    }
    if (gEngineApp_m_Settings.multiplayer == 0) {
        Sound__Play(0x3AC1, arg1->header.m_vPos, 0, -1);
    }
}
#else
#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "globals/scene.h"

sptr GameObject_GetPlatformBelow(s32);
s32 GameObject_GetFrameDelta(sptr, sptr);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play Sound__Play_i3
#endif
s32 Sound__Play(s32, s32, sptr, sptr, s32, s32);

M2C_UNK CVector3__Add(void *, void *, M2C_UNK *);   /* extern */

void PlayerJump_Enter(sptr arg0, void *arg1) {
    u8 sp18[0x10];
    f32 var_f1;
    sptr temp_v0;
    void *temp_a0;

    var_f1 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f1 = 481.28f;
    }
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) (var_f1 * M2C_FIELD(M2C_FIELD(arg1, void **, 0x14), f32 *, 0x20));
    temp_v0 = GameObject_GetPlatformBelow((sptr) arg1);
    if (temp_v0 != 0) {
        GameObject_GetFrameDelta(temp_v0, (sptr) sp18);
        temp_a0 = arg1 + 0x18;
        CVector3__Add(temp_a0, temp_a0, sp18);
    }
    if (gEngineApp_m_Settings.multiplayer == 0) {
        Sound__Play(0x3AC1, M2C_FIELD(arg1, s32 *, 4), M2C_FIELD(arg1, s32 *, 8), M2C_FIELD(arg1, s32 *, 0xC), 0, -1);
    }
}
#endif

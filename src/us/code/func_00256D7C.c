#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
} Region;

extern Region *func_00268A2C(CGameObjectInstance *actor);
extern f32 func_00298470(f32 x); /* square root */
extern f32 func_0026872C(CGameObjectInstance *actor, s32 arg1);

/* Keeps an actor inside a sphere of radius 20000 in regions with flag 0x4000. */
void func_00256D7C(CPlayer *player, CGameObjectInstance *actor) {
    Region *region = func_00268A2C(actor);
    f32 xx;
    f32 zz;
    f32 y;

    if (region != NULL && (region->flags & 0x4000)) {
        xx = actor->header.m_vPos.x * actor->header.m_vPos.x;
        zz = actor->header.m_vPos.z * actor->header.m_vPos.z;
        if (xx + actor->header.m_vPos.y * actor->header.m_vPos.y + zz > 400000000.0f) {
            y = 400000000.0f - xx - zz;
            if (y < 0.0f) {
                y = 0.0f;
            }
            y = func_00298470(y);
            if (actor->header.m_vPos.y < 0.0f) {
                y = -y;
            }
            actor->header.m_vPos.y = y;
            if (region->flags & 0x4000) {
                actor->header.m_vPos.y = func_0026872C(actor, 0) - 1.0f;
            }
        }
    }
}

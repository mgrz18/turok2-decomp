#include "common.h"
#include "structs/game_object.h"

/* Spawns effect 3 from obj at other's position, with params (0xA00, value << 8). */

typedef struct {
    /* 0x00 */ s16 upper;
    /* 0x02 */ s16 lower;
    /* 0x04 */ u8 extra[4];
} __attribute__((aligned(4))) ActionParams;

extern void ImpactEvent_Send(); /* (src, dst, kind, id, CVector3 pos, 8-byte params): MotherBoss_TeleportTarget's file names the params Part */
typedef void (*ImpactSendFn)(void *source, void *p, s32 effect, s32 arg3, CVector3 pos, ActionParams params);

void MotherBoss_SendImpactAt(CGameObjectInstance *obj, CGameObjectInstance *other, s32 value) {
    ActionParams params;

    params.upper = 0xA00;
    params.lower = value << 8;
    params.extra[0] = params.extra[1] = params.extra[2] = params.extra[3] = 0;
    ((ImpactSendFn) ImpactEvent_Send)(obj, obj, 3, -1, other->header.m_vPos, params);
}

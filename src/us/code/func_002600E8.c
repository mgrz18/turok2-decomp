#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 C26): the members at their host offsets (the N64 pads put the flags on
 * m_rpGeometryHeaderSize, as in Destructible_GoEnter, func_00265B58.c); the flag bits by mask
 * below (the bit-fields are laid out from the other end on a little-endian host) */
typedef struct {
    union {
        struct { u8 _hpPos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; CVector3 pos; };
        struct { u8 _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; CIntelligence *def; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; union { u32 word; } flags; };
    };
} Actor;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ union {
        u32 word;
        struct {
            u32 hi : 12;
            u32 b80000 : 1;
            u32 b : 5;
            u32 b2000 : 1;
            u32 c : 4;
            u32 b100 : 1;
            u32 lo : 8;
        } bits;
    } flags;
} Actor;
#endif


void Actor_SpawnParticle(Actor *self, s32 type, CVector3 pos, s32 arg4);
void TotemDefense_AddKill(void);
void AI_StopSound(Actor *self, void *ai);
void CListedInstance__Free(Actor *self);

void EnemyAI_NukedFrozenRemove(Actor *self, void *ai) {
    CIntelligence *def;

    Actor_SpawnParticle(self, 0x17, self->pos, 0);
    TotemDefense_AddKill();
    def = self->def;
    if (!(self->flags.word & 0x80000)) {
        if (!(def->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit(&gScene, self, 1);
        }
        GameObject_FireTriggers(self, 1, self);
    }
#ifdef TARGET_PORT
    self->flags.word &= ~0x100;
    self->flags.word &= ~0x2000;
#else
    self->flags.bits.b100 = 0;
    self->flags.bits.b2000 = 0;
#endif
    AI_StopSound(self, ai);
    if (self->flags.word & 0x80000) {
        CListedInstance__Free(self);
    }
}

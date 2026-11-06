#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"

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

extern s32 D_800F7078[];

void Actor_SpawnParticle(Actor *self, s32 type, CVector3 pos, s32 arg4);
void TotemDefense_AddKill(void);
void CScene__SetInstanceSavedBit(void *mgr, Actor *actor, s32 flag);
void GameObject_FireTriggers(Actor *actor, s32 flags, Actor *actor2);
void AI_StopSound(Actor *self, void *ai);
void CListedInstance__Free(Actor *self);

void EnemyAI_NukedFrozenRemove(Actor *self, void *ai) {
    CIntelligence *def;

    Actor_SpawnParticle(self, 0x17, self->pos, 0);
    TotemDefense_AddKill();
    def = self->def;
    if (!(self->flags.word & 0x80000)) {
        if (!(def->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit(D_800F7078, self, 1);
        }
        GameObject_FireTriggers(self, 1, self);
    }
    self->flags.bits.b100 = 0;
    self->flags.bits.b2000 = 0;
    AI_StopSound(self, ai);
    if (self->flags.word & 0x80000) {
        CListedInstance__Free(self);
    }
}

#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
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

void AI_StopSound(Actor *self, void *ai);
void CListedInstance__Free(Actor *self);

void EnemyAI_StopSoundAndFree(Actor *self, void *ai) {
    self->flags.bits.b100 = 0;
    self->flags.bits.b2000 = 0;
    AI_StopSound(self, ai);
    if (self->flags.word & 0x80000) {
        CListedInstance__Free(self);
    }
}

#include "common.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 C26: the N64 pads put the flags (0xD4) on host m_rpGeometryHeader/m_rpObjectHeader); the bits by mask below (the bit-fields are laid out from the other end on a
 * little-endian host) */
typedef struct {
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_dwFlags)];
    union { u32 word; } flags;
} Actor;
#else

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
#endif

void AI_StopSound(Actor *self, void *ai);
void CListedInstance__Free(Actor *self);

void EnemyAI_StopSoundAndFree(Actor *self, void *ai) {
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

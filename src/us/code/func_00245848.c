/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80213ED4_8.CPP, func_802164A8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"

typedef struct {
    u8 pad0[0xC];
    s32 unkC;                   /* 0xC: -1 = finished */
} SoundSource;

typedef struct {
    u8 pad0[4];
    CVector3 position;              /* 0x04 */
    u8 pad10[4];
    s32 *kind;                  /* 0x14: the definition; its first word is the kind */
    u8 pad18[0x50 - 0x18];
    f32 height;                 /* 0x50 */
    u8 pad54[0xD4 - 0x54];
    u32 flags;                  /* 0xD4 */
    u8 padD8[0x16C - 0xD8];
    s32 active;                 /* 0x16C */
} Source;

typedef struct Dest Dest;
struct Dest {
    u8 pad0[0x40];
    f32 value;                  /* 0x40 */
    u8 pad44[4];
    CVector3 old_position;          /* 0x48 */
    CVector3 position;              /* 0x54 */
    f32 height;                 /* 0x60 */
    u8 pad64[0xC9 - 0x64];
    u8 timer;                   /* 0xC9 */
    u8 padCA[0xEC - 0xCA];
    SoundSource *sound;         /* 0xEC */
    void (*callback)(Source *, Dest *); /* 0xF0 */
};

extern f32 D_800B6D28;
extern void AI_StopSound(Source *, Dest *);
extern void AI_UpdateRegionFlags(Source *, Dest *);
extern void AI_RideMovingPlatform(Source *, Dest *);
extern void CAI__UpdateModeAnim(Source *, Dest *);

void CAI__Update(Source *src, Dest *dst) {
    if (dst->sound != NULL && dst->sound->unkC == -1) {
        AI_StopSound(src, dst);
    }
    if (dst->timer != 0) {
        dst->timer--;
        D_800B6D28 = 0.0f;
        return;
    }
    if (src->active != 0) {
        if (*src->kind == 2) {
            dst->old_position = src->position;
            dst->height = src->height;
        }
        dst->position = src->position;
        dst->value += D_800B6D28;
        AI_UpdateRegionFlags(src, dst);
        if (src->flags & 0x10000) {
            AI_RideMovingPlatform(src, dst);
        }
        if (dst->callback != NULL) {
            dst->callback(src, dst);
        }
        CAI__UpdateModeAnim(src, dst);
        if (*src->kind != 2) {
            dst->old_position = src->position;
            dst->height = src->height;
        }
    }
}

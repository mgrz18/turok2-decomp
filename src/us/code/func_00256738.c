/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_5.CPP, func_8022E0B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "structs/player.h"



typedef struct SoundRange {
    char pad0[0x18];
    s8 min;                    /* 0x18 */
    s8 max;                    /* 0x19 */
    s8 divisor;                /* 0x1A */
} SoundRange;

typedef struct SoundEvent {
    CGameObjectInstance *object;        /* 0x00 (Rage Wars 0x08) */
    char pad4[0x14 - 0x4];
    SoundRange *range;         /* 0x14 (Rage Wars 0x1C) */
    char pad18[0x90 - 0x18];
    s32 value;                 /* 0x90 (Rage Wars 0x04) */
} SoundEvent;

extern void CMtxF__RotateX(void *arg0, f32 arg1);
extern float D_800A71FC;

void Actor_PlayRangedSound(void *arg0, void *arg1) {
    SoundRange *range;
    CPlayer *actor;
    s32 value;
    f32 amount;
    SoundEvent *event = arg1;
    CGameObjectInstance *object;
    range = event->range;
    value = event->value;
    if (value < range->min) {
        return;
    }
    if (range->max < value) {
        return;
    }
    object = event->object;
    actor = ((CPlayer *)object->m_AI.m_pParent);
    amount = -actor->m_vHeadRotOffset.x;
    if (actor->m_Mode == 0xE) {
        amount += D_800A71FC;
    }
    CMtxF__RotateX(arg0, amount / (f32)range->divisor);
}

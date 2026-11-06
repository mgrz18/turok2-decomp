#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_StopSound(CGameObjectInstance *obj, CAI *ai);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */
extern s32 D_8012F988[];
extern s32 D_8012F9A8[];

void Primagen_UpdateMode2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    s32 limit;
    u32 bit;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    limit = 0x4B00;
    if (D_8012F608.stage == 0) {
        limit = 0x2580;
    }
    if (D_8012F608.damage >= limit) {
        AI_StopSound(obj, ai);
        ai->m_VisBits &= ~(1 << D_8012F608.part);
        D_8012F608.partActive = 0;
        CAI__SetMode(obj, ai, 7);
    } else if (D_8012F608.partTimer <= 0.0f) {
        AI_StopSound(obj, ai);
        D_8012F988[D_8012F608.part] = D_8012F9A8[D_8012F608.part];
        bit = 1 << D_8012F608.part;
        D_8012F608.partActive = 0;
        D_8012F608.partsDone |= bit;
        ai->m_VisBits |= bit;
        CAI__SetMode(obj, ai, 7);
    } else {
        D_8012F608.partTimer -= D_800B6D28;
    }
}

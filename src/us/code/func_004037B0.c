#include "common.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 mask;
} Source004037B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
    /* 0x08 */ u8 pad1[0xC - 0x8];
    /* 0x0C */ s32 flags;
    /* 0x10 */ Source004037B0 *source;
} Hit004037B0;

extern u8 D_00431A7C[];
extern u8 D_00431B0C[];
void ScriptAnimator_SetScript(void *light, void *arg);
void Primagen_SetSpawnScript(s32 a0);
f32 Random_Blend(f32 lo, f32 hi);
void CAI__SetMode(void *a0, void *a1, s32 event);

void Primagen_Hurt(void *a0, void *a1, Hit004037B0 *hit) {
    s32 destroyed = 0;
    s32 bonus;
    s32 bit;
    s32 i;

    if (hit->source == NULL) {
        return;
    }
    if (D_8012F608.armed == 0) {
        return;
    }
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (!(hit->source->mask & bit)) {
            continue;
        }
        if (!(D_8012F608.partsDone & bit) && !D_8012F608.partActive) {
            continue;
        }
        if (!D_8012F608.partActive) {
            D_8012F608.values[0][i] -= hit->damage;
        }
        if (D_8012F608.partActive) {
            if (i != 1) {
                continue;
            }
            D_8012F608.damage += hit->damage;
        }
        if (D_8012F608.values[0][i] <= 0 && D_8012F608.stage != 3 && !D_8012F608.partActive) {
            ScriptAnimator_SetScript(&D_8012F608.anims[i], D_00431B0C);
            D_8012F608.part = i;
            destroyed = 1;
            break;
        }
        ScriptAnimator_SetScript(&D_8012F608.anims[i], D_00431A7C);
    }
    bonus = 0;
    if ((hit->flags & 0x400000) && D_8012F608.stage == 1 && (hit->source->mask & 2)) {
        bonus = hit->damage / 2;
    }
    if (bonus != 0 && D_8012F608.stage == 1 && !D_8012F608.partActive) {
        for (i = 3; i < 7; i++) {
            if (D_8012F608.values[0][i] > 0) {
                D_8012F608.values[0][i] -= bonus;
                if (D_8012F608.values[0][i] <= 0) {
                    ScriptAnimator_SetScript(&D_8012F608.anims[i], D_00431B0C);
                    D_8012F608.part = i;
                    destroyed = 1;
                    break;
                }
                ScriptAnimator_SetScript(&D_8012F608.anims[i], D_00431A7C);
            }
        }
    }
    if (!destroyed) {
        return;
    }
    if (D_8012F608.partActive) {
        return;
    }
    if (D_8012F608.stage == 2) {
        Primagen_SetSpawnScript(4);
    }
    if (!D_8012F608.unk3CA) {
        D_8012F608.partActive = 1;
        D_8012F608.partTimer = 90.0f;
        D_8012F608.swayPhase = Random_Blend(0.0f, 2.0f);
        D_8012F608.swayAmount = Random_Blend(2.5f, 4.0f);
        D_8012F608.bossMode = 9;
        CAI__SetMode(a0, a1, 0x12);
    } else {
        D_8012F608.partActive = 1;
        CAI__SetMode(a0, a1, 0x18);
    }
}

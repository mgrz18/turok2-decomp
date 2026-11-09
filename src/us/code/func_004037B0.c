#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

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

void ScriptAnimator_SetScript(void *light, void *arg);
void Primagen_SetSpawnScript(s32 a0);
f32 Random_Blend(f32 lo, f32 hi);

void Primagen_Hurt(void *a0, void *a1, Hit004037B0 *hit) {
    s32 destroyed = 0;
    s32 bonus;
    s32 bit;
    s32 i;

    if (hit->source == NULL) {
        return;
    }
    if (gLevelScript.armed == 0) {
        return;
    }
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (!(hit->source->mask & bit)) {
            continue;
        }
        if (!(gLevelScript.partsDone & bit) && !gLevelScript.partActive) {
            continue;
        }
        if (!gLevelScript.partActive) {
            gLevelScript.values[0][i] -= hit->damage;
        }
        if (gLevelScript.partActive) {
            if (i != 1) {
                continue;
            }
            gLevelScript.damage += hit->damage;
        }
        if (gLevelScript.values[0][i] <= 0 && gLevelScript.stage != 3 && !gLevelScript.partActive) {
            ScriptAnimator_SetScript(&gLevelScript.anims[i], sPrimagenPartDestroyScript);
            gLevelScript.part = i;
            destroyed = 1;
            break;
        }
        ScriptAnimator_SetScript(&gLevelScript.anims[i], sPrimagenPartHitScript);
    }
    bonus = 0;
    if ((hit->flags & 0x400000) && gLevelScript.stage == 1 && (hit->source->mask & 2)) {
        bonus = hit->damage / 2;
    }
    if (bonus != 0 && gLevelScript.stage == 1 && !gLevelScript.partActive) {
        for (i = 3; i < 7; i++) {
            if (gLevelScript.values[0][i] > 0) {
                gLevelScript.values[0][i] -= bonus;
                if (gLevelScript.values[0][i] <= 0) {
                    ScriptAnimator_SetScript(&gLevelScript.anims[i], sPrimagenPartDestroyScript);
                    gLevelScript.part = i;
                    destroyed = 1;
                    break;
                }
                ScriptAnimator_SetScript(&gLevelScript.anims[i], sPrimagenPartHitScript);
            }
        }
    }
    if (!destroyed) {
        return;
    }
    if (gLevelScript.partActive) {
        return;
    }
    if (gLevelScript.stage == 2) {
        Primagen_SetSpawnScript(4);
    }
    if (!gLevelScript.unk3CA) {
        gLevelScript.partActive = 1;
        gLevelScript.partTimer = 90.0f;
        gLevelScript.swayPhase = Random_Blend(0.0f, 2.0f);
        gLevelScript.swayAmount = Random_Blend(2.5f, 4.0f);
        gLevelScript.bossMode = 9;
        CAI__SetMode(a0, a1, 0x12);
    } else {
        gLevelScript.partActive = 1;
        CAI__SetMode(a0, a1, 0x18);
    }
}

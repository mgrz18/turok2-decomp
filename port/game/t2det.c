/* t2det.c: DEBUG-ONLY determinism trace for `turok2 --game` (round 9 x4, docs/port/MACOS.md section 11).
 * Called from game.cpp between retraces, while every game thread is blocked. */
#include "common.h"
#include "structs/world.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/intelligence.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* T2_DET_LOG=N (round 9 x4, DEBUG-ONLY): a determinism trace to diff between hosts. Every N frames
 * (from T2_DET_FROM, default 0) one line with the exact bits of the RNG seeds, the first player
 * (position, facing, velocity, health, AI mode) and an FNV-1a hash over every level actor and
 * pooled instance (position, facing, health, AI mode, action, animation index and frame); with
 * T2_DET_ACTORS=1 also one line per live actor, so the first actor whose bits differ shows up.
 * Floats are printed as their bit patterns: %.1f hides the drift that leads to a divergence. */
#include "globals/math.h"

static u32 T2Det_Bits(f32 f) {
    u32 u;
    memcpy(&u, &f, 4);
    return u;
}

static u64 T2Det_Mix(u64 h, u32 v) {
    int i;
    for (i = 0; i < 4; i++) {
        h ^= (v >> (i * 8)) & 0xFF;
        h *= 0x100000001B3ull;
    }
    return h;
}

static u64 T2Det_Actor(u64 h, CGameObjectInstance *o, const char *kind, int i, int verbose) {
    u32 v[10];
    int k;
    v[0] = T2Det_Bits(o->header.m_vPos.x);
    v[1] = T2Det_Bits(o->header.m_vPos.y);
    v[2] = T2Det_Bits(o->header.m_vPos.z);
    v[3] = T2Det_Bits(o->m_RotY);
    v[4] = (u32) o->m_AI.m_Health;
    v[5] = (u32) o->m_AI.m_Mode;
    v[6] = (u32) o->m_AI.m_Action;
    v[7] = (u32) o->m_CurrentAnimation.m_nAnimIndex;
    v[8] = T2Det_Bits(o->m_CurrentAnimation.m_AnimationFrame);
    v[9] = (u32) o->m_dwFlags;
    for (k = 0; k < 10; k++) {
        h = T2Det_Mix(h, v[k]);
    }
    if (verbose) {
        printf("det:  %s %d pos %08x %08x %08x (%.3f %.3f %.3f) rot %08x hp %d mode %d act %d anim %d %08x fl %x\n", kind, i,
               v[0], v[1], v[2], o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, v[3], (int) v[4],
               (int) v[5], (int) v[6], (int) v[7], v[8], v[9]);
    }
    return h;
}

void T2Port_LogDet(int frame) {
    static int sFrom = -1, sVerbose;
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    u64 h = 0xCBF29CE484222325ull;
    int i, n = 0;
    if (sFrom < 0) {
        sFrom = getenv("T2_DET_FROM") != NULL ? atoi(getenv("T2_DET_FROM")) : 0;
        sVerbose = getenv("T2_DET_ACTORS") != NULL;
    }
    if (frame < sFrom) {
        return;
    }
    printf("det: frame %d mode %d level %d rng %08x %08x", frame, (int) gEngineApp.m_Settings.mode,
           (int) gScene.m_nLevel, (unsigned) gRandomValue, (unsigned) gRandomValueB);
    if (p != NULL) {
        CGameObjectInstance *a = &p->m_CharacterActor;
        printf(" player %08x %08x %08x rot %08x vel %08x %08x %08x hp %d mode %d btn %x stick %08x %08x",
               T2Det_Bits(a->header.m_vPos.x), T2Det_Bits(a->header.m_vPos.y), T2Det_Bits(a->header.m_vPos.z),
               T2Det_Bits(a->m_RotY), T2Det_Bits(a->m_vVelocity.x), T2Det_Bits(a->m_vVelocity.y),
               T2Det_Bits(a->m_vVelocity.z), (int) p->m_Data.m_Health, (int) a->m_AI.m_Mode,
               (unsigned) p->m_ContState.m_dwButtons, T2Det_Bits(p->m_ContState.m_MoveX),
               T2Det_Bits(p->m_ContState.m_LookX));
    }
    printf("\n");
    for (i = gScene.m_nFirstActor; gScene.m_pActors != NULL && i < gScene.m_nActors; i++, n++) {
        h = T2Det_Actor(h, &gScene.m_pActors[i], "lvl", i, sVerbose && gScene.m_pActors[i].header.m_pIntelligence != NULL);
    }
    {
        CListedInstance *li = (CListedInstance *) gScene.m_InstancePool.m_ActiveList.m_pHead;
        int k;
        for (k = 0; li != NULL && k < 256; k++, li = li->m_pNext, n++) {
            h = T2Det_Actor(h, &li->m_Instance, "dyn", k, sVerbose);
        }
    }
    printf("det: frame %d actors %d hash %016llx\n", frame, n, (unsigned long long) h);
    fflush(stdout);
}

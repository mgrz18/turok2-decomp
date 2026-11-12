/* t2det.c: DEBUG-ONLY determinism trace for `turok2 --game` (round 9 x4, docs/port/MACOS.md section 11).
 * Called from game.cpp between retraces, while every game thread is blocked. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* dladdr (T2_RNG_TRACE) on glibc */
#endif
#include "common.h"
#include "structs/world.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/intelligence.h"
#include "structs/particle.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/* T2_DET_LOG=N (round 9 x4, DEBUG-ONLY): a determinism trace to diff between hosts. Every N frames
 * (from T2_DET_FROM, default 0) one line with the exact bits of the RNG seeds, the first player
 * (position, facing, velocity, health, AI mode) and an FNV-1a hash over every level actor and
 * pooled instance (position, facing, health, AI mode, action, animation index and frame); with
 * T2_DET_ACTORS=1 also one line per live actor, so the first actor whose bits differ shows up.
 * Floats are printed as their bit patterns: %.1f hides the drift that leads to a divergence. */
#include "globals/math.h"
#include "globals/frame.h"

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
    static int sFrom = -1, sVerbose, sParticles, sGameFrames;
    static u32 sLastGf = 0xFFFFFFFF;
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    u64 h = 0xCBF29CE484222325ull;
    int i, n = 0;
    if (sFrom < 0) {
        sFrom = getenv("T2_DET_FROM") != NULL ? atoi(getenv("T2_DET_FROM")) : 0;
        sVerbose = getenv("T2_DET_ACTORS") != NULL;
        sParticles = getenv("T2_DET_PARTICLES") != NULL; /* one line per live particle */
        sGameFrames = getenv("T2_DET_GF") != NULL;
    }
    if (frame < sFrom) {
        return;
    }
    /* T2_DET_GF=1 (round 10 y3): one line set per game frame (a new gFrameCountGameplay), as
     * tools/port/n64trace.py writes them from the N64's RDRAM; tools/port/detdiff.py aligns both. */
    if (sGameFrames) {
        if (gFrameCountGameplay == sLastGf) {
            return;
        }
        sLastGf = gFrameCountGameplay;
    }
    printf("det: frame %d gf %u ticks %d mode %d level %d rng %08x %08x", frame, (unsigned) gFrameCountGameplay,
           (int) gNextTicks, (int) gEngineApp.m_Settings.mode,
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
    if (p != NULL && sVerbose) {
        T2Det_Actor(0, &p->m_WeaponActor, "wpn", 0, 1); /* the weapon actor: its bones place the shots */
        if (p->m_pCamera != NULL) { /* round 11 z5: the camera (view inputs, tremor, shake, view translation) */
            CCamera *c = p->m_pCamera;
            CCameraViewParams *vp = &c->m_ViewParams;
            printf("det:  cam vp %08x %08x %08x roty %08x off %08x %08x %08x yoff %08x trem %08x %08x "
                   "shk %08x %08x %08x amp %08x %08x %08x pos %08x %08x %08x\n",
                   T2Det_Bits(vp->m_vPos.x), T2Det_Bits(vp->m_vPos.y), T2Det_Bits(vp->m_vPos.z),
                   T2Det_Bits(vp->m_RotY), T2Det_Bits(vp->m_vRotOffset.x), T2Det_Bits(vp->m_vRotOffset.y),
                   T2Det_Bits(vp->m_vRotOffset.z), T2Det_Bits(vp->m_YPosOffset), T2Det_Bits(c->m_TremorAngle),
                   T2Det_Bits(c->m_TremorHeight), T2Det_Bits(c->m_Shake.m_XAxis.m_Shake),
                   T2Det_Bits(c->m_Shake.m_YAxis.m_Shake), T2Det_Bits(c->m_Shake.m_ZAxis.m_Shake),
                   T2Det_Bits(c->m_Shake.m_XAxis.m_Amp), T2Det_Bits(c->m_Shake.m_YAxis.m_Amp),
                   T2Det_Bits(c->m_Shake.m_ZAxis.m_Amp), T2Det_Bits(c->m_vPos.x), T2Det_Bits(c->m_vPos.y),
                   T2Det_Bits(c->m_vPos.z));
        }
    }
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
    if (sVerbose) {
        /* the live particles (round 10 y3): the shots and effects, which hit the actors */
        int j, k;
        for (j = 0; j < 3; j++) {
            CParticle *q = (CParticle *) gScene.m_ParticleSystem.m_ActiveList[j].m_pHead;
            for (k = 0; q != NULL && k < 128; k++, q = q->m_pNext) {
                printf("det:  prt %d %d type %d pos %08x %08x %08x vel %08x %08x %08x age %08x\n", j, k,
                       (int) q->header.m_nObjType, T2Det_Bits(q->header.m_vPos.x), T2Det_Bits(q->header.m_vPos.y),
                       T2Det_Bits(q->header.m_vPos.z), T2Det_Bits(q->m_vVelocity.x), T2Det_Bits(q->m_vVelocity.y),
                       T2Det_Bits(q->m_vVelocity.z), T2Det_Bits(q->m_cFrame));
            }
        }
    }
    {
        /* round 10 y4: the particle pool (position, velocity, flags of all 128 slots; free slots
         * keep their last values, the same on every host), so a shot that flies differently
         * shows up before it hits something */
        u64 hp = 0xCBF29CE484222325ull;
        int k;
        for (k = 0; k < 128; k++) {
            CParticle *pt = &gScene.m_ParticleSystem.m_ParticleVars[k];
            hp = T2Det_Mix(hp, T2Det_Bits(pt->header.m_vPos.x));
            hp = T2Det_Mix(hp, T2Det_Bits(pt->header.m_vPos.y));
            hp = T2Det_Mix(hp, T2Det_Bits(pt->header.m_vPos.z));
            hp = T2Det_Mix(hp, T2Det_Bits(pt->m_vVelocity.x));
            hp = T2Det_Mix(hp, T2Det_Bits(pt->m_vVelocity.y));
            hp = T2Det_Mix(hp, T2Det_Bits(pt->m_vVelocity.z));
            hp = T2Det_Mix(hp, pt->m_dwFlags);
            if (sParticles && pt->m_pEffect != NULL) {
                printf("det:  ptcl %d pos %08x %08x %08x vel %08x %08x %08x fl %x\n", k,
                       T2Det_Bits(pt->header.m_vPos.x), T2Det_Bits(pt->header.m_vPos.y),
                       T2Det_Bits(pt->header.m_vPos.z), T2Det_Bits(pt->m_vVelocity.x),
                       T2Det_Bits(pt->m_vVelocity.y), T2Det_Bits(pt->m_vVelocity.z), (unsigned) pt->m_dwFlags);
            }
        }
        printf("det: frame %d actors %d hash %016llx ptcl %016llx\n", frame, n, (unsigned long long) h,
               (unsigned long long) hp);
    }
    fflush(stdout);
}

/* T2_RNG_TRACE=F1..F2 (round 10 y4, DEBUG-ONLY): one line per Random_Next call in host frames
 * F1..F2: the frame, the seed before the call, and the caller's return address (absolute, and
 * relative to its image: `atos -o turok2 -l BASE ADDR` on macOS, `addr2line -fe turok2 ADDR` on
 * Linux). Diffing two hosts' traces names the caller that draws a different number of times. */
extern int gT2HostFrame;

void T2Port_RngTrace(void *ra) {
    static int sLo = -2, sHi;
    Dl_info info;
    if (sLo == -2) {
        const char *s = getenv("T2_RNG_TRACE");
        sLo = -1;
        if (s != NULL) {
            sLo = atoi(s);
            sHi = strstr(s, "..") != NULL ? atoi(strstr(s, "..") + 2) : sLo;
        }
    }
    if (sLo < 0 || gT2HostFrame < sLo || gT2HostFrame > sHi) {
        return;
    }
    if (dladdr(ra, &info) != 0 && info.dli_fbase != NULL) {
        printf("rng: frame %d seed %08x ra %p off %lx %s\n", gT2HostFrame, (unsigned) gRandomValue, ra,
               (unsigned long) ((char *) ra - (char *) info.dli_fbase), info.dli_sname ? info.dli_sname : "?");
    } else {
        printf("rng: frame %d seed %08x ra %p\n", gT2HostFrame, (unsigned) gRandomValue, ra);
    }
}

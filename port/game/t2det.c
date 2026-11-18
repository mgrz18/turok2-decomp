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
#include <stdint.h>

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
    if (verbose && getenv("T2_DET_MTX") != NULL) {
        /* round 18 g2: the draw matrices (m_pmtDrawMtxs, else the last frame's) as block indices in
         * gScene.m_Heap and an FNV hash of the first 8 blocks */
        MtxPool *mp = &gScene.m_Heap[0];
        u8 *d = (u8 *) o->m_pmtDrawMtxs, *l = (u8 *) o->m_pmtLastFrameDrawMtxs, *m = d != NULL ? d : l;
        u32 bh[8];
        int b, k2;
        for (k2 = 0; k2 < 8; k2++) {
            u32 x = 0x811C9DC5u;
            for (b = 0; m != NULL && b < 0x40; b++) {
                x = (x ^ m[k2 * 0x40 + b]) * 0x01000193u;
            }
            bh[k2] = x;
        }
        printf("det:  mtx %s %d D %ld L %ld h %08x %08x %08x %08x %08x %08x %08x %08x\n", kind, i,
               d != NULL ? (long) ((d - (u8 *) mp->base) / 0x40) : -1L,
               l != NULL ? (long) ((l - (u8 *) mp->base) / 0x40) : -1L, bh[0], bh[1], bh[2], bh[3], bh[4], bh[5],
               bh[6], bh[7]);
    }
    if (verbose) {
        printf("det:  %s %d pos %08x %08x %08x (%.3f %.3f %.3f) rot %08x hp %d mode %d act %d anim %d %08x fl %x\n", kind, i,
               v[0], v[1], v[2], o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, v[3], (int) v[4],
               (int) v[5], (int) v[6], (int) v[7], v[8], v[9]);
    }
    return h;
}

/* round 18 g2: T2_DET_WATCH=FRAME:ACTOR calls these two (never inlined) so that gdb can put a
 * hardware watchpoint on a level actor's position from FRAME's det line to the next one:
 * `break T2Det_WatchBegin` -> `watch -l *(float *) $rdi`, `break T2Det_WatchEnd` -> stop. */
__attribute__((noinline)) void T2Det_WatchBegin(void *addr) {
    __asm__ volatile("" : : "r"(addr) : "memory");
}
__attribute__((noinline)) void T2Det_WatchEnd(void *addr) {
    __asm__ volatile("" : : "r"(addr) : "memory");
}

void T2Port_LogDet(int frame) {
    static int sFrom = -1, sVerbose, sParticles, sGameFrames, sWatchFrame = -1, sWatchActor, sWatchLen = 1;
    if (sWatchFrame == -1) {
        const char *w = getenv("T2_DET_WATCH");
        sWatchFrame = -2;
        if (w != NULL && sscanf(w, "%d:%d:%d", &sWatchFrame, &sWatchActor, &sWatchLen) < 2) {
            sWatchFrame = -2;
        }
    }
    if (sWatchFrame >= 0 && (frame == sWatchFrame || frame == sWatchFrame + sWatchLen) && gScene.m_pActors != NULL) {
        void *a = &gScene.m_pActors[sWatchActor].header.m_vPos.x;
        printf("det: watch frame %d actor %d at %p\n", frame, sWatchActor, a);
        fflush(stdout);
        if (frame == sWatchFrame) {
            T2Det_WatchBegin(a);
        } else {
            T2Det_WatchEnd(a);
        }
    }
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
        /* round 11 z4: the pool's lists (free, draw, the 3 active lists) as slot sequences: a slot
         * freed or allocated in another order keeps its values, so the slot hash above misses it */
        {
            CList *ls[5];
            int j;
            ls[0] = &gScene.m_ParticleSystem.m_FreeList;
            ls[1] = &gScene.m_ParticleSystem.m_DrawList;
            for (j = 0; j < 3; j++) {
                ls[2 + j] = &gScene.m_ParticleSystem.m_ActiveList[j];
            }
            for (j = 0; j < 5; j++) {
                u8 *e = (u8 *) ls[j]->m_pHead;
                hp = T2Det_Mix(hp, (u32) ls[j]->m_nCount);
                for (k = 0; e != NULL && k < 129; k++) {
                    hp = T2Det_Mix(hp, (u32) ((e - (u8 *) gScene.m_ParticleSystem.m_ParticleVars) / (int) sizeof(CParticle)));
                    e = *(u8 **) (e + ls[j]->m_NextOffset);
                }
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

/* T2_RAW_DUMP=FILE:LEVEL:GF[-GF2][,GF...] (DEBUG-ONLY, round 17 f3): the raw bytes of the game's
 * state structs at the given game frames (gFrameCountGameplay) of LEVEL, one record per object:
 * gRandomValue, gScene, the first player (CPlayer: its character and weapon actors), its camera, every level actor
 * (gScene.m_pActors[i]) and every pooled instance (the active list, in order). tools/port/n64cmp.py
 * reads them with the host layout of include/ (libclang) and compares them member by member with
 * the N64's (n64trace.py --raw, the same records at the N64 layout), so a field that a host function
 * wrote at its N64 offset, or a conversion that went wrong, shows up by its member name. The file
 * starts with "T2RW"; a record is tag[4] idx s32, gf u32, level s32, addr u64, size u32, bytes. */
static void T2Raw_Put(FILE *f, const char *tag, int idx, const void *p, unsigned size) {
    u32 gf = gFrameCountGameplay;
    s32 lv = gScene.m_nLevel, i = idx;
    u64 a = (u64) (uintptr_t) p;
    fwrite(tag, 1, 4, f);
    fwrite(&i, 4, 1, f);
    fwrite(&gf, 4, 1, f);
    fwrite(&lv, 4, 1, f);
    fwrite(&a, 8, 1, f);
    fwrite(&size, 4, 1, f);
    fwrite(p, 1, size, f);
}

static int T2Raw_Want(const char *s, int gf) {
    while (*s) {
        char *e;
        long a = strtol(s, &e, 10), b = a;
        if (e == s) {
            return 0;
        }
        if (*e == '-') {
            b = strtol(e + 1, &e, 10);
        }
        if (gf >= a && gf <= b) {
            return 1;
        }
        if (*e != ',') {
            break;
        }
        s = e + 1;
    }
    return 0;
}

void T2Port_RawDump(const char *spec) {
    static FILE *sOut;
    static int sLevel = -99999, sBad;
    static u32 sLastGf = 0xFFFFFFFF;
    static const char *sList;
    CPlayer *p;
    int i;
    if (sBad) {
        return;
    }
    if (sOut == NULL) {
        char path[512];
        const char *c1 = strchr(spec, ':'), *c2 = c1 ? strchr(c1 + 1, ':') : NULL;
        if (c2 == NULL || (size_t) (c1 - spec) >= sizeof(path)) {
            sBad = 1;
            fprintf(stderr, "T2_RAW_DUMP: bad spec '%s' (FILE:LEVEL:GF[-GF2][,...])\n", spec);
            return;
        }
        memcpy(path, spec, c1 - spec);
        path[c1 - spec] = 0;
        sOut = fopen(path, "wb");
        if (sOut == NULL) {
            sBad = 1;
            perror("T2_RAW_DUMP");
            return;
        }
        fwrite("T2RW", 1, 4, sOut);
        sLevel = atoi(c1 + 1);
        sList = c2 + 1;
    }
    if (gEngineApp.m_Settings.mode != 13 || gScene.m_nLevel != sLevel || gFrameCountGameplay == sLastGf) {
        return;
    }
    sLastGf = gFrameCountGameplay;
    if (!T2Raw_Want(sList, (int) gFrameCountGameplay)) {
        return;
    }
    T2Raw_Put(sOut, "RNG ", 0, &gRandomValue, 4);
    T2Raw_Put(sOut, "SCN ", 0, &gScene, sizeof(gScene));
    p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (p != NULL) {
        T2Raw_Put(sOut, "PLR ", 0, p, sizeof(*p));
        if (p->m_pCamera != NULL) {
            T2Raw_Put(sOut, "CAM ", 0, p->m_pCamera, sizeof(*p->m_pCamera));
        }
    }
    for (i = 0; gScene.m_pActors != NULL && i < gScene.m_nActors; i++) {
        T2Raw_Put(sOut, "LVL ", i, &gScene.m_pActors[i], sizeof(CGameObjectInstance));
    }
    {
        CListedInstance *li = (CListedInstance *) gScene.m_InstancePool.m_ActiveList.m_pHead;
        for (i = 0; li != NULL && i < 256; i++, li = li->m_pNext) {
            T2Raw_Put(sOut, "DYN ", i, &li->m_Instance, sizeof(CGameObjectInstance));
        }
    }
    fflush(sOut);
}

/* A self test of the swoosh/effect system on the host (structs/swoosh.h): builds a private
 * CSwooshSystem with the game's own code and runs its lists, timers, points and decals.
 * turok2 --game runs it after the game (main.cpp); docs/port/STATUS.md (round 4, s5). */

#include "common.h"
#include "structs/vector.h"
#include "structs/swoosh.h"
#include "globals/engine.h"
#include "globals/particle.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

extern void func_0042D410(CSwooshSystem *world);
extern void func_00236444(CSwooshSystem *, s32 (*)(SwooshTimer *), sptr, s32, f32);
extern void SwooshSystem_UpdateTimers(CSwooshSystem *);
extern s32 *NodePool_Alloc(void *);
extern void NodePool_Free(void *, void *);
extern void func_0026D230(SwooshLinks *);
extern SwooshNode *Swoosh_NewNode(CSwooshSystem *, Swoosh *, void *);
extern void SwooshGroup_Remove(CSwooshSystem *, Swoosh *, SwooshNode *);
extern SwooshDecal *func_002364A4(CSwooshSystem *, CVector3, CVector3, s32, sptr);
extern void SwooshSet_FreeOwner(CSwooshSystem *, sptr);
extern void RibbonVtxPool_Reset(CSwooshSystem *);
extern sptr RibbonVtxPool_Alloc(void *, s32);

static CSwooshSystem sTestSystem;
static s32 sTimerCalls;
static sptr sTimerArg;
static int sFails;

#define T2CHECK(c, what)                                                    \
    do {                                                                    \
        if (!(c)) {                                                         \
            printf("t2: swoosh self test: %s (line %d)\n", what, __LINE__); \
            sFails++;                                                       \
        }                                                                   \
    } while (0)

static s32 timerCallback(SwooshTimer *t) {
    sTimerCalls++;
    sTimerArg = t->arg;
    return 0;
}

/* Walks a DList both ways: returns its length, -1 if the links disagree. */
static int walk(SwooshLinks *l) {
    SwooshNode *n, *prev = NULL;
    int count = 0;

    for (n = l->head; n != NULL; n = n->next) {
        if (n->prev != prev) return -1;
        prev = n;
        count++;
    }
    if (l->tail != prev || l->count != count) return -1;
    return count;
}

static void translation(f32 *m, f32 x, f32 y, f32 z) {
    int i;

    for (i = 0; i < 16; i++) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

int T2Swoosh_SelfTest(void) {
    CSwooshSystem *sys = &sTestSystem;
    f32 savedIncrement = gFrameIncrement;
    s32 savedSpawned = sSwooshSpawned;
    u8 info[0x24];
    f32 m[16];
    f32 life = 5.0f;
    Swoosh *s;
    SwooshNode *n;
    SwooshDecal *d;
    CVector3 pos, dir;
    sptr v0, v1;

    sFails = 0;
    memset(sys, 0xAB, sizeof(*sys));
    func_0042D410(sys);
    T2CHECK(walk(&sys->m_FreeNodes) == 96, "free points");
    T2CHECK(walk(&sys->m_SwooshPool.free) == 32 && sys->m_SwooshPool.used.count == 0, "swoosh pool");
    T2CHECK(walk(&sys->m_TimerPool.free) == 3, "timer pool");
    T2CHECK(sys->m_FreeDecals.m_nCount == 100, "free decals");

    /* the ribbon vertex pool hands out whole host pointers */
    RibbonVtxPool_Reset(sys);
    v0 = RibbonVtxPool_Alloc(sys, 10);
    v1 = RibbonVtxPool_Alloc(sys, 10);
    T2CHECK(v0 == (sptr) sys->m_RibbonVtx[gEvenOdd] && v1 == v0 + 10 * (sptr) sizeof(SwooshVtx), "ribbon vertices");
    T2CHECK(RibbonVtxPool_Alloc(sys, 300) == 0 && sys->m_RibbonVtxLeft == 280, "ribbon pool limit");

    /* a timer: twice, then freed */
    gFrameIncrement = 1.0f;
    sTimerCalls = 0;
    func_00236444(sys, timerCallback, (sptr) sys, 2, 1.0f);
    T2CHECK(sys->m_TimerPool.used.count == 1, "timer started");
    SwooshSystem_UpdateTimers(sys);
    SwooshSystem_UpdateTimers(sys);
    T2CHECK(sTimerCalls == 2 && sTimerArg == (sptr) sys, "timer fired");
    T2CHECK(sys->m_TimerPool.used.count == 0 && walk(&sys->m_TimerPool.free) == 3, "timer freed");

    /* a swoosh with three points 5 and 12 apart */
    memset(info, 0, sizeof(info));
    memcpy(info + 8, &life, sizeof(life));
    s = (Swoosh *) NodePool_Alloc(&sys->m_SwooshPool);
    T2CHECK(s == &sys->m_Swooshes[0] && walk(&sys->m_SwooshPool.used) == 1, "swoosh allocated");
    if (s != NULL) {
        func_0026D230(&s->nodes);
        s->info = info;
        s->flags = 0;
        s->owner = NULL;
        s->length = 0.0f;
        s->unk28 = 0.0f;
        translation(m, 0.0f, 0.0f, 0.0f);
        Swoosh_NewNode(sys, s, m);
        translation(m, 3.0f, 4.0f, 0.0f);
        Swoosh_NewNode(sys, s, m);
        translation(m, 3.0f, 4.0f, 12.0f);
        n = Swoosh_NewNode(sys, s, m);
        T2CHECK(n != NULL && n->life == 5.0f && n->pos.z == 12.0f, "point placed");
        T2CHECK(walk(&s->nodes) == 3 && walk(&sys->m_FreeNodes) == 93, "points taken");
        T2CHECK(fabsf(s->length - 17.0f) < 1e-4f, "swoosh length");
        T2CHECK(n != NULL && fabsf(n->fraction - 1.0f) < 1e-4f && fabsf(n->prev->fraction - 5.0f / 17.0f) < 1e-4f,
                "point fractions");
        SwooshGroup_Remove(sys, s, s->nodes.head);
        T2CHECK(walk(&s->nodes) == 2 && walk(&sys->m_FreeNodes) == 94 && fabsf(s->length - 12.0f) < 1e-4f,
                "point removed");
        NodePool_Free(&sys->m_SwooshPool, s);
        T2CHECK(walk(&sys->m_SwooshPool.free) == 32 && sys->m_SwooshPool.used.count == 0, "swoosh freed");
    }

    /* a decal of kind 3, then freed by its owner */
    pos.x = 1.0f; pos.y = 2.0f; pos.z = 3.0f;
    dir.x = 0.0f; dir.y = 1.0f; dir.z = 0.0f;
    sSwooshSpawned = 0;
    d = func_002364A4(sys, pos, dir, 3, (sptr) &sTestSystem);
    T2CHECK(d != NULL && d->owner == (sptr) &sTestSystem, "decal placed");
    T2CHECK(sys->m_FreeDecals.m_nCount == 99 && sys->m_DecalLists[3].m_nCount == 1, "decal lists");
    SwooshSet_FreeOwner(sys, (sptr) &sTestSystem);
    T2CHECK(sys->m_FreeDecals.m_nCount == 100 && sys->m_DecalLists[3].m_nCount == 0, "decal freed");

    gFrameIncrement = savedIncrement;
    sSwooshSpawned = savedSpawned;
    printf("t2: swoosh self test: %s (%d failed)\n", sFails ? "FAILED" : "OK", sFails);
    return sFails;
}

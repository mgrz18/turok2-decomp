/* t2bosslog.c: DEBUG-ONLY. T2_BOSS_LOG=1 (with T2_STATE_LOG, round 8 w3): the boss fight state of
 * the current level, to follow the fights headless (docs/port/GAMEPLAY.md section 9). Level 1 the
 * Blind One (gBlindOne), 2 the Mantid Queen (gMantidQueen), 3 the Mother (gMotherBoss), 4 the
 * Primagen (gLevelScript). Not a game option. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "structs/collision.h"
#include <stdio.h>
#include <stdlib.h>

static void logActor(const char *what, CGameObjectInstance *a) {
    if (a == NULL) {
        printf("boss:   %s none\n", what);
        return;
    }
    printf("boss:   %s %p hp %d/%d mode %d anim %d pos (%.1f %.1f %.1f) fl %x\n", what, (void *) a,
           (int) a->m_AI.m_Health, (int) a->m_AI.m_InitialHealth, (int) a->m_AI.m_Mode,
           (int) a->m_CurrentAnimation.m_nAnimIndex, a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z,
           (unsigned) a->m_dwFlags);
    printf("boss:   %s type %x bounds (%.1f %.1f %.1f)..(%.1f %.1f %.1f) scale (%.2f %.2f %.2f) rotY %.2f\n", what,
           (unsigned) a->m_nTypeFlag, a->m_vMin.x, a->m_vMin.y, a->m_vMin.z, a->m_vMax.x, a->m_vMax.y, a->m_vMax.z,
           a->m_vScale.x, a->m_vScale.y, a->m_vScale.z, a->m_RotY);
}

void T2Port_LogBosses(void) {
    int i;

    switch (gScene.m_nLevel) {
        case 1: {
            BlindOneState *s = &gBlindOne;
            printf("boss: blind one phase %d step %d counts %d/%d/%d alive %d/%d/%d minions %d/%d\n",
                   (int) s->phase, s->script ? (int) (s->script - sBlindOneScript) : -1, (int) s->count1,
                   (int) s->count2, (int) s->count3, (int) s->alive1, (int) s->alive2, (int) s->alive3,
                   (int) s->minionsAlive, (int) s->maxMinions);
            logActor("boss", s->boss);
            break;
        }
        case 2: {
            WaveBossState *s = &gMantidQueen;
            printf("boss: mantid queen step %d toSpawn %d timer %d members %d wave %d pushed %d spots",
                   (int) s->stepIndex, (int) s->toSpawn, (int) s->timer, (int) s->memberCount,
                   (int) s->waveActive, (int) s->pushed);
            for (i = 0; i < 5; i++) {
                printf(" %d", (int) s->spots[i].health);
            }
            printf("\n");
            logActor("boss", s->boss);
            logActor("owner", s->actor);
            for (i = 0; i < s->memberCount && i < 8; i++) {
                logActor("member", s->members[i]);
            }
            {
                CListedInstance *e;
                int n = 0, live = 0;
                for (e = gScene.m_InstancePool.m_ActiveList.m_pHead; e != NULL; e = e->m_pNext) {
                    n++;
                    live += e->m_Instance.m_AI.m_Health > 0;
                    if (n <= 6) {
                        printf("boss:   inst type %x intel %d mode %d", (unsigned) e->m_Instance.m_nTypeFlag,
                               e->m_Instance.header.m_pIntelligence ? (int) e->m_Instance.header.m_pIntelligence->m_Type : -1,
                               (int) e->m_Instance.m_AI.m_Mode);
                        logActor("", &e->m_Instance);
                    }
                }
                printf("boss:   instances %d live %d\n", n, live);
            }
            break;
        }
        case 3: {
            MotherBossState *s = &gMotherBoss;
            printf("boss: mother phase %d health %d/%d parts %d/%d active %x live %x spots",
                   (int) s->phase, (int) s->health, (int) s->totalHealth, (int) s->partsLeft,
                   (int) s->partCount, (unsigned) s->activeMask, (unsigned) s->liveMask);
            for (i = 0; i < 15; i++) {
                printf(" %d", (int) s->slots[i].health);
            }
            printf("\n");
            logActor("boss", s->obj);
            break;
        }
        case 4: {
            LevelScriptState *s = &gLevelScript;
            printf("boss: primagen stage %d state %d mode %d parts %d/%d spawns %d waves %d alive %d "
                   "scripts A %d/%d B %d/%d C %d/%d\n",
                   (int) s->stage, (int) s->bossState, (int) s->bossMode, (int) s->partsDone,
                   (int) s->part, (int) s->spawnsLeft, (int) s->wavesLeft, (int) s->enemiesAlive,
                   (int) s->script.cursor, (int) s->script.current, (int) s->script2.cursor,
                   (int) s->script2.current, (int) s->script3.cursor, (int) s->script3.current);
            logActor("boss", s->actor);
            logActor("part0", s->found[0]);
            logActor("part1", s->found[1]);
            break;
        }
    }
}

/* T2_BOSS_LOG=1: every hit a boss hurt callback gets (called from the hurt callbacks under
 * TARGET_PORT, DEBUG-ONLY): the damage, the flags, the node bits of the hit frame. */
void T2Port_LogBossHurt(const char *who, void *obj, void *arg) {
    DamageEvent *hit = arg;
    static int on = -1;
    if (on < 0) {
        on = getenv("T2_BOSS_LOG") != NULL;
    }
    if (!on) {
        return;
    }
    printf("bosshurt: %s obj %p vis %x mode %d dmg %d flags %x frame %p nodes %x factor %.2f src %p\n", who, obj,
           obj ? (unsigned) ((CGameObjectInstance *) obj)->m_AI.m_VisBits : 0u,
           obj ? (int) ((CGameObjectInstance *) obj)->m_AI.m_Mode : -1,
           (int) hit->damage, (unsigned) hit->flags, (void *) hit->frame,
           hit->frame ? (unsigned) hit->frame->unk_6C : 0u, hit->frame ? hit->frame->damageFactor : 0.0f,
           hit->source);
    fflush(stdout);
}

/* T2_AIM_NODE=MASK[,R] (game.cpp, every frame; DEBUG-ONLY, round 9 x2): in gameplay, turn the
 * first player (yaw, and the camera pitch the weapons fire along) to the centre of the nearest hit
 * box whose node mask has a bit of MASK (and of the actor's m_VisBits, the parts still there) on
 * the nearest live actor within R (default 4000) that has one: aims scripted fire at a boss's
 * weak spots (the Mother's arms 0xC00, heads 0x5000, the chest 0x2000). The box centre is
 * computed as CGameObjectInstance__GetNearestBox does (the box corners through the node's draw
 * matrix). T2_AIM_PITCH_SIGN=+-1 flips the pitch convention. */
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "structs/world.h"
#include "protos/iset.h"
#include <math.h>
#include <string.h>

/* atan2f from + - * / only (round 10 y4): libm's atan2f differs in the last bit between macOS
 * and glibc, so the aim pitch, the player's shots and everything they hit drifted apart between
 * the hosts (the Mother recipe, Mac against Linux, frame 14,543). Max error ~1e-5 rad. */
static f32 T2Aim_Atan2f(f32 y, f32 x) {
    f32 ax = x < 0.0f ? -x : x, ay = y < 0.0f ? -y : y;
    f32 a, s, r;
    if (ax == 0.0f && ay == 0.0f) {
        return 0.0f;
    }
    a = ax > ay ? ay / ax : ax / ay;
    s = a * a;
    r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;
    if (ay > ax) {
        r = 1.57079637f - r;
    }
    if (x < 0.0f) {
        r = 3.14159274f - r;
    }
    return y < 0.0f ? -r : r;
}

typedef struct T2AimBox {
    CVector3 corners[8];
    f32 size;
    u8 pad64[8];
    s32 mask;
} T2AimBox;
extern void **CGameObjectInstance__LoadGeometry(CGameObjectInstance *actor);
extern void CMtxF__FromMtxAffine(CMtxF *dst, void *src);
extern void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
extern void CCache__Unlock(s32, void *);
extern f32 AI_AngleToPoint(CGameObjectInstance *a, CVector3 point);

static int aimBoxCentre(CGameObjectInstance *o, s32 mask, CVector3 *from, CVector3 *out, s32 *nodeMask) {
    void **handle;
    void *boxes;
    void *mtxs = o->m_pmtDrawMtxs != NULL ? (void *) o->m_pmtDrawMtxs : (void *) o->m_pmtLastFrameDrawMtxs;
    s32 count, i, j;
    f32 best = 3.4e38f;
    int found = 0;

    if (mtxs == NULL || (handle = CGameObjectInstance__LoadGeometry(o)) == NULL) {
        return 0;
    }
    boxes = CISet__GetBlock(*handle, 3);
    count = *(s32 *) boxes;
    for (i = 0; i < count; i++) {
        T2AimBox *b = CISet__GetBlock(CISet__GetBlock(boxes, i), 1);
        CVector3 c = { 0, 0, 0 }, w;
        CMtxF m;
        f32 dx, dy, dz, d;
        if (!(b->mask & mask & o->m_AI.m_VisBits)) {
            continue;
        }
        for (j = 0; j < 8; j++) {
            c.x += b->corners[j].x;
            c.y += b->corners[j].y;
            c.z += b->corners[j].z;
        }
        c.x *= 0.125f;
        c.y *= 0.125f;
        c.z *= 0.125f;
        CMtxF__FromMtxAffine(&m, (u8 *) mtxs + i * 0x40);
        CMtxF__TransformPoint(&m, &c, &w);
        dx = w.x - from->x;
        dy = w.y - from->y;
        dz = w.z - from->z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            best = d;
            *out = w;
            *nodeMask = b->mask;
            found = 1;
        }
    }
    CCache__Unlock(0, handle);
    return found;
}

/* T2_BOSS_FRONT=D[,EVERY] (game.cpp, every frame; DEBUG-ONLY, round 11 z2): the Mantid Queen's open
 * weak spot faces one way (she turns to her path target, not to the player), and her body's boxes
 * (node mask 1, never a weak spot) take every shot fired from her side or back: from the recipe's
 * fixed spot no hit reached spot 3 in mode 15 (MantidQueen_Hurt counts spot 3 only in mode 15).
 * Every EVERY frames (default 600) while a spot is open between her waves, put the first player D
 * units (flat) in front of her, on the line from her centre through the open spot's box: what a
 * player standing in front of her does. The Primagen (level 4): after EVERY frames of its mode-25
 * wait for a line of sight, the player D units to the right of its lair (below). */
extern sptr CScene__FindRegionAt(CScene *world, CVector3 *pos);
static int aimBoxCentre(CGameObjectInstance *o, s32 mask, CVector3 *from, CVector3 *out, s32 *nodeMask);
extern f32 GameObject_GetCollisionHeight(CGameObjectInstance *e);
extern s32 Cinema_PathPlaying(void);
extern s32 Cinema_GetScriptId(void);
const char *T2Port_Scheduled(const char *spec, int frame, char *buf, int n);
void T2Port_BossFront(int frame, const char *spec0) {
    static int sLast = -100000;
    /* "D,EVERY@F;..." (round 12 C27): the spec scheduled for this frame (the Queen's, then the Primagen's) */
    char sbuf[48];
    const char *sched = T2Port_Scheduled(spec0, frame, sbuf, sizeof(sbuf));
    const char *spec = sched != NULL ? sched : ""; /* none yet: "" (returns below) */
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    WaveBossState *q = &gMantidQueen;
    const char *comma = strchr(spec, ',');
    f32 dist = (f32) atof(spec);
    int every = comma != NULL ? atoi(comma + 1) : 600;
    /* ,K (round 12 a2): the Primagen's spot is K D in front of the lair (default 2.5) */
    const char *comma2 = comma != NULL ? strchr(comma + 1, ',') : NULL;
    f32 ahead = comma2 != NULL ? (f32) atof(comma2 + 1) : 2.5f;
    CGameObjectInstance *o;
    CVector3 c, box, pos;
    s32 node = 0;
    f32 dx, dz, len;
    sptr region;

    if (spec[0] == 0 || p == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_WarpState != 0) {
        return;
    }
    if (gScene.m_nLevel == 4) {
        /* the Primagen: mode 25 flies to its lair and waits for the player as its sight target; at
         * the lair its eye sits behind the front node boxes of found[0] (type 0x641, faces at
         * z 2141, x about -300..300 at the player's eye line), so from the middle of the floor no line
         * of sight reaches it. After EVERY frames of that wait, put the player D units to the right of
         * the lair and 2.5 D in front of it (D 450: (451, 0, 1799), the line passes beside found[0]) */
        static int sWait, sTry;
        /* ",K,1" (round 12 C27): each placement of one wait tries the next spot of this list (side,
         * scale of D, scale of K): where he rests at the lair varies with the run before him, and a
         * spot that sees his eye from one rest may not from another (PLAYTHROUGH.md 9.5) */
        static const f32 kSpots[][3] = {{1, 1, 1}, {-1, 1, 1}, {1, 1, 0.8f}, {-1, 1, 0.8f}, {1, 0.67f, 1},
                                        {-1, 0.67f, 1}, {1, 1.33f, 0.85f}, {-1, 1.33f, 0.85f}, {1, 1, 1.2f}, {-1, 1, 1.2f}};
        const char *comma3 = comma2 != NULL ? strchr(comma2 + 1, ',') : NULL;
        int cycle = comma3 != NULL && atoi(comma3 + 1) != 0;
        f32 side = 1.0f, dscale = 1.0f, kscale = 1.0f;
        CGameObjectInstance *b = gLevelScript.actor;
        if (b != NULL && b->m_AI.m_Mode == 24 && frame % 300 == 0) {
            /* DEBUG-ONLY (round 12 a2): mode 24 waits for its timer and the end of an animation loop */
            printf("bossfront: frame %d primagen mode 24 bossMode %d timer %.2f anim %d frame %.2f repeated %d pos (%.1f %.1f %.1f)\n",
                   frame, (int) gLevelScript.bossMode, gLevelScript.timer, (int) b->m_CurrentAnimation.m_nAnimIndex,
                   b->m_CurrentAnimation.m_AnimationFrame, (int) b->m_AI.m_AnimRepeatedCount, b->header.m_vPos.x,
                   b->header.m_vPos.y, b->header.m_vPos.z);
        }
        if (b == NULL || b->m_AI.m_Mode != 25) {
            sWait = 0;
            sTry = 0;
            return;
        }
        if (++sWait < every) {
            if ((sWait == 3 || sWait == every / 2) && getenv("T2_BOSS_FRONT_PROBE") != NULL) {
                /* DEBUG-ONLY (round 12 a2): the sight search the wait needs, two frames after the move */
                extern void *AI_FindBestTarget(void *self, void *plan, s32 checkSight, s32 flag);
                extern CCollisionResults gCollisionResults;
                extern s32 D_800F1F50[];   /* the Primagen's searches run with 0x8000 set (Primagen_UpdateMode19) */
                void *t;
                CGameObjectInstance *a = &p->m_CharacterActor;
                D_800F1F50[0] |= 0x8000;
                t = AI_FindBestTarget(b, &b->m_AI, 1, 0);
                printf("bossfront: probe frame %d sight %p hit %p wall %p edge %d player (%.1f %.1f %.1f) yoff %.1f h %.1f\n",
                       frame, t, (void *) gCollisionResults.m_pInstance, (void *) gCollisionResults.m_pWallRegion,
                       (int) gCollisionResults.m_nWallEdge, a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z,
                       a->m_CollisionYOffset, GameObject_GetCollisionHeight(a));
                if (getenv("T2_BOSS_FRONT_SCAN") != NULL) {
                    /* which floor spots see him: the player moved there for one search each */
                    CVector3 keep = a->header.m_vPos, q;
                    struct CGameRegion *keepR = a->header.m_pCurrentRegion;
                    int gx, gz;
                    for (gz = 2250; gz >= 1500; gz -= 50) {
                        printf("bossfront: scan z %4d:", gz);
                        for (gx = -800; gx <= 800; gx += 50) {
                            sptr rg;
                            q.x = (f32) gx;
                            q.y = 0.0f;
                            q.z = (f32) gz;
                            rg = CScene__FindRegionAt(&gScene, &q);
                            if (rg == 0) {
                                printf(" ");
                                continue;
                            }
                            a->header.m_vPos = q;
                            a->header.m_pCurrentRegion = (struct CGameRegion *) rg;
                            printf("%c", AI_FindBestTarget(b, &b->m_AI, 1, 0) != NULL ? 'S' : '.');
                        }
                        printf("\n");
                    }
                    a->header.m_vPos = keep;
                    a->header.m_pCurrentRegion = keepR;
                }
                D_800F1F50[0] &= ~0x8000;
                if (gLevelScript.found[0] != NULL) {
                    CGameObjectInstance *d = gLevelScript.found[0];
                    printf("bossfront:   found[0] ty %x fl %x mode %d anim %d %.2f pos (%.1f %.1f %.1f)\n",
                           (unsigned) d->m_nTypeFlag, (unsigned) d->m_dwFlags, (int) d->m_AI.m_Mode,
                           (int) d->m_CurrentAnimation.m_nAnimIndex, d->m_CurrentAnimation.m_AnimationFrame,
                           d->header.m_vPos.x, d->header.m_vPos.y, d->header.m_vPos.z);
                }
                if (gCollisionResults.m_pInstance != NULL) {
                    CGameObjectInstance *h = gCollisionResults.m_pInstance;
                    long idx = gScene.m_pActors != NULL ? (long) (h - gScene.m_pActors) : -1;
                    printf("bossfront:   hit actor %ld ty %x it %d fl %x pos (%.1f %.1f %.1f) mode %d\n", idx,
                           (unsigned) h->m_nTypeFlag,
                           h->header.m_pIntelligence != NULL ? (int) h->header.m_pIntelligence->m_Type : -1,
                           (unsigned) h->m_dwFlags, h->header.m_vPos.x, h->header.m_vPos.y, h->header.m_vPos.z,
                           (int) h->m_AI.m_Mode);
                }
            }
            return;
        }
        sWait = 0;
        if (cycle) {
            int k = sTry++ % (int) (sizeof(kSpots) / sizeof(kSpots[0]));
            side = kSpots[k][0];
            dscale = kSpots[k][1];
            kscale = kSpots[k][2];
        }
        pos.x = b->header.m_vPos.x + side * dscale * dist;
        pos.y = 0.0f;
        pos.z = b->header.m_vPos.z - ahead * kscale * dist;
        region = CScene__FindRegionAt(&gScene, &pos);
        printf("bossfront: frame %d primagen (%.1f %.1f %.1f) mode 25 -> player (%.1f %.1f %.1f) region %p"
               " pathfind %d kind1 %d/%d sight %p cinema %d/%d pmode %d aifl %x over %p path %p dest (%.1f %.1f %.1f) bstate %d\n", frame,
               b->header.m_vPos.x, b->header.m_vPos.y, b->header.m_vPos.z, pos.x, pos.y, pos.z, (void *) region,
               (int) b->m_AI.m_PathFindCount, (int) gScene.m_Kind1Cursor, (int) gScene.m_nKind1,
               (void *) b->m_AI.m_pSightTarget, (int) Cinema_PathPlaying(), (int) Cinema_GetScriptId(), (int) p->m_Mode, (unsigned) b->m_AI.m_dwFlags, (void *) b->m_AI.m_pTargetOverride, (void *) b->m_AI.m_pPathTarget, gLevelScript.dest.x, gLevelScript.dest.y, gLevelScript.dest.z, (int) gLevelScript.bossState);
        fflush(stdout);
        if (region != 0) {
            CGameObjectInstance *a = &p->m_CharacterActor;
            a->header.m_vPos = pos;
            a->header.m_pCurrentRegion = (struct CGameRegion *) region;
            a->m_vVelocity.x = a->m_vVelocity.y = a->m_vVelocity.z = 0.0f;
        }
        return;
    }
    if (gScene.m_nLevel != 2 || frame - sLast < every) {
        return;
    }
    if (q->step == NULL || q->waveActive || q->step->weakSpot < 0 || (o = q->actor) == NULL ||
        o->m_AI.m_Health <= 0) {
        return;
    }
    c = o->header.m_vPos;
    if (!aimBoxCentre(o, 1 << q->step->weakSpot, &c, &box, &node)) {
        return;
    }
    dx = box.x - c.x;
    dz = box.z - c.z;
    len = sqrtf(dx * dx + dz * dz);
    if (len < 1.0f) {
        return;
    }
    pos.x = c.x + dx / len * dist;
    pos.y = c.y;
    pos.z = c.z + dz / len * dist;
    region = CScene__FindRegionAt(&gScene, &pos);
    sLast = frame;
    printf("bossfront: frame %d spot %d queen (%.1f %.1f %.1f) box (%.1f %.1f) -> player (%.1f %.1f %.1f) region %p\n",
           frame, (int) q->step->weakSpot, c.x, c.y, c.z, box.x, box.z, pos.x, pos.y, pos.z, (void *) region);
    fflush(stdout);
    if (region != 0) {
        CGameObjectInstance *a = &p->m_CharacterActor;
        a->header.m_vPos = pos;
        a->header.m_pCurrentRegion = (struct CGameRegion *) region;
        a->m_vVelocity.x = a->m_vVelocity.y = a->m_vVelocity.z = 0.0f;
    }
}

/* T2_AIM_NODE=boss: the weak spot the current boss phase wants (the Mother: MotherBoss_Hurt's
 * spot kinds, phase 0 the arms 0xC00 while slots 2..4 live then the heads 0x5000 (6..8), phase 1
 * slot 1 (0xC00) then 5 (0x5000), phase 2 the chest 0x2000); otherwise a number. */
static CGameObjectInstance *sAimSkip;
/* the Blind One (round 10 y2): only the actors its phase waits for (sAimSet), or only the spawned
 * instances (its minions, sAimPoolOnly) */
static CGameObjectInstance *sAimSet[8];
static int sAimSetN, sAimPoolOnly;

static void aimSet(CGameObjectInstance **actors, s32 n) {
    int i;
    for (i = 0; i < n && i < 8; i++) {
        sAimSet[sAimSetN++] = actors[i];
    }
}

static s32 T2Port_AimMask(const char *spec) {
    sAimSkip = NULL;
    sAimSetN = 0;
    sAimPoolOnly = 0;
    if (strncmp(spec, "actor:", 6) == 0) {
        /* actor:N (round 11 z1): only the level actor N (a Sister of Despair among her zombies) */
        int n = atoi(spec + 6);
        if (gScene.m_pActors != NULL && n >= 0 && n < gScene.m_nActors) {
            sAimSet[sAimSetN++] = &gScene.m_pActors[n];
        }
        return -1;
    }
    if (strncmp(spec, "boss", 4) == 0) {
        if (gScene.m_nLevel == 1) {
            /* the Blind One's script (sBlindOneScript): phase 3 its minions, 4/5/6 the actors of
             * group 1/2/3 (BlindOne_PhaseGroupNUpdate waits for aliveN == 0), 7-9 the boss */
            BlindOneState *b = &gBlindOne;
            switch (b->phase) {
                case 3:
                    sAimPoolOnly = 1;
                    break;
                case 4:
                    aimSet(b->actors1, b->count1);
                    break;
                case 5:
                    aimSet(b->actors2, b->count2);
                    break;
                case 6:
                    aimSet(b->actors3, b->count3);
                    break;
                default:
                    if (b->boss != NULL) {
                        sAimSet[sAimSetN++] = b->boss;
                    }
                    break;
            }
            return -1;
        }
        if (gScene.m_nLevel == 3) {
            MotherBossState *s = &gMotherBoss;
            switch (s->phase) {
                case 0:
                    return (s->slots[2].health > 0 || s->slots[3].health > 0 || s->slots[4].health > 0) ? 0xC00 : 0x5000;
                case 1:
                    return s->slots[1].health > 0 ? 0xC00 : 0x5000;
                default:
                    return 0x2000;
            }
        }
        if (gScene.m_nLevel == 2) {
            /* the Mantid Queen: the open spot (1 << step->weakSpot) between the waves; during a
             * wave (or no open spot) the nearest minion, any box (the boss is skipped) */
            WaveBossState *q = &gMantidQueen;
            if (q->step != NULL && !q->waveActive && q->step->weakSpot >= 0) {
                return 1 << q->step->weakSpot;
            }
            sAimSkip = q->boss;
            return -1;
        }
        if (gScene.m_nLevel == 4) {
            /* the Primagen (round 10 y2): Primagen_Hurt counts a hit on node bit i (0..7) when bit i
             * of partsDone is set (the parts the stage opens), or bit 1 only while a part is
             * staggered (partActive) */
            LevelScriptState *ls = &gLevelScript;
            if (ls->enemiesAlive > 0 || !ls->armed) {
                /* its spawner's waves (spawned instances) first: the stage goes on when they die */
                sAimPoolOnly = 1;
                return -1;
            }
            if (ls->partActive) {
                return 2;
            }
            return (ls->partsDone & 0xFF) != 0 ? (ls->partsDone & 0xFF) : -1;
        }
        return -1;
    }
    return (s32) strtol(spec, NULL, 0);
}

/* the portable atan2f for t2debug.c's aims (round 11 z1) */
f32 T2Port_Atan2f(f32 y, f32 x) {
    return T2Aim_Atan2f(y, x);
}

int T2Port_AimNode(int frame, const char *spec) {
    static s32 sLastMask = -1;
    static int sLastFrame;
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    CGameObjectInstance *pl, *best = NULL;
    CVector3 eye, aim, bestAim;
    s32 mask = T2Port_AimMask(spec), nodeMask = 0, bestNode = 0;
    const char *comma = strchr(spec, ',');
    const char *sign = getenv("T2_AIM_PITCH_SIGN");
    f32 r = comma != NULL ? (f32) atof(comma + 1) : 4000.0f, bestD, dx, dy, dz, h;
    int i;

    /* T2_AIM_NODE=...@LEVEL (round 11 z3): only in that level (a whole-level route ends in a boss);
     * @L1/L2/... (round 12 a2): in any of them (level 6's route: the Mother 3, then the Primagen 4) */
    const char *at = strchr(spec, '@');
    if (at != NULL) {
        int in = 0;
        do {
            in |= atoi(at + 1) == (int) gScene.m_nLevel;
            at = strchr(at + 1, '/');
        } while (at != NULL);
        if (!in) {
            return 0;
        }
    }
    if (p == NULL || p->m_pCamera == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_pActors == NULL ||
        gScene.m_WarpState != 0) {
        return 0;
    }
    pl = &p->m_CharacterActor;
    eye = p->m_pCamera->m_vPos;
    bestD = r * r;
    /* the level's actors, then the spawned instances (the bosses' minions) */
    for (i = gScene.m_nFirstActor; i < gScene.m_nActors + 512; i++) {
        static CListedInstance *sNext;
        CGameObjectInstance *o;
        f32 d;
        if (i < gScene.m_nActors) {
            o = &gScene.m_pActors[i];
        } else {
            sNext = i == gScene.m_nActors ? gScene.m_InstancePool.m_ActiveList.m_pHead : sNext->m_pNext;
            if (sNext == NULL) {
                break;
            }
            o = &sNext->m_Instance;
        }
        if (o == pl || o == sAimSkip || o->header.m_pIntelligence == NULL || o->m_AI.m_Health <= 0 || !(o->m_dwFlags & 0x100)) {
            continue;
        }
        if (sAimPoolOnly && i < gScene.m_nActors) {
            continue;
        }
        if (sAimSetN > 0) {
            int k, in = 0;
            for (k = 0; k < sAimSetN; k++) {
                in |= sAimSet[k] == o;
            }
            if (!in) {
                continue;
            }
        }
        dx = o->header.m_vPos.x - eye.x;
        dz = o->header.m_vPos.z - eye.z;
        d = dx * dx + dz * dz;
        if (d < bestD && mask == -1 && !aimBoxCentre(o, mask, &eye, &aim, &nodeMask)) {
            /* any target, no hit boxes drawn (yet): its position, 50 units up */
            aim = o->header.m_vPos;
            aim.y += 50.0f;
            nodeMask = 0;
            bestD = d;
            best = o;
            bestAim = aim;
            bestNode = nodeMask;
        } else if (d < bestD && aimBoxCentre(o, mask, &eye, &aim, &nodeMask)) {
            bestD = d;
            best = o;
            bestAim = aim;
            bestNode = nodeMask;
        }
    }
    if (best == NULL) {
        return 0;
    }
    pl->m_RotY += AI_AngleToPoint(pl, bestAim);
    dx = bestAim.x - eye.x;
    dy = bestAim.y - eye.y;
    dz = bestAim.z - eye.z;
    h = sqrtf(dx * dx + dz * dz);
    p->m_pCamera->m_ViewParams.m_vRotOffset.x = (sign != NULL ? (f32) atof(sign) : 1.0f) * T2Aim_Atan2f(dy, h);
    if (bestNode != sLastMask || frame - sLastFrame >= 300) {
        sLastMask = bestNode;
        sLastFrame = frame;
        printf("aim: frame %d eye (%.1f %.1f %.1f) -> actor %d node %x at (%.1f %.1f %.1f) pitch %.2f\n", frame,
               eye.x, eye.y, eye.z, (best >= gScene.m_pActors && best < gScene.m_pActors + gScene.m_nActors) ? (int) (best - gScene.m_pActors) : -1, (unsigned) bestNode, bestAim.x, bestAim.y,
               bestAim.z, p->m_pCamera->m_ViewParams.m_vRotOffset.x);
        fflush(stdout);
    }
    return 1;
}

/* T2_CHEATS=MASK, T2_FILL_AMMO=N (game.cpp, every frame; DEBUG-ONLY, round 9 x2): OR the game's own cheat flags
 * (gEngineApp_m_Settings.cheatFlags, what the cheat menu sets) in gameplay: 1 invincibility
 * (CPlayer__Hurt returns early), as the cheat menu's toggles do. For long boss fights. */
void T2Port_Cheats(int frame, const char *spec, const char *fill) {
    extern s32 CPlayer__FillAllAmmo(CPlayer *);
    CPlayer *p;

    if (gEngineApp.m_Settings.mode != 13) {
        return;
    }
    if (spec != NULL) {
        /* "MASK@F;..." (round 13 b5): a frame-scheduled value; the bits this switch set are cleared
         * again when the scheduled mask drops them (the cheat menu's own bits are left alone) */
        static s32 sSet = 0;
        char cbuf[16];
        const char *c = T2Port_Scheduled(spec, frame, cbuf, sizeof(cbuf));
        s32 want = c != NULL ? (s32) strtol(c, NULL, 0) : 0;
        if ((sSet & ~want) != 0) {
            gEngineApp_m_Settings.cheatFlags &= ~(sSet & ~want);
        }
        gEngineApp_m_Settings.cheatFlags |= want;
        sSet = want;
    }
    /* T2_FILL_AMMO=N: every N frames the active players' ammo is filled (CPlayer__FillAllAmmo) */
    char fbuf[16];
    fill = T2Port_Scheduled(fill, frame, fbuf, sizeof(fbuf)); /* "N@F;..." (round 12 C27) */
    if (fill != NULL && atoi(fill) > 0 && frame % atoi(fill) == 0) {
        for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            CPlayer__FillAllAmmo(p);
        }
    }
}

/* T2_GIVE_ITEMS=ID[:N][,...] (game.cpp, every frame; DEBUG-ONLY, round 12 a2): once, on the first gameplay
 * frame with a player, set the special pickup counts the earlier levels would have given
 * (m_Data.m_SpecialPickupCount[ID - 4000], low nibble the count held, high nibble the count found,
 * as CPlayer__CollectKeyPiece keeps them): the crystal ion capacitor 4231 (level 3), the red laser
 * cells 4229 (level 5), the blue laser cells 4230 (level 4) that level 6's actions need
 * (CActionIntelligence.m_GoPickupNeeded, Action_CheckRequiredItem). N defaults to 1; "@F": not before
 * frame F (a new game clears the counts: give them once the player is in the hub); several groups
 * "...@F1;...@F2" are each applied once (the count is set, not added). */
void T2Port_GiveItems(int frame, const char *spec) {
    static unsigned sDone = 0;
    CPlayer *p;
    int group = 0;
    if (gEngineApp.m_Settings.mode != 13) {
        return;
    }
    p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    if (p == NULL) {
        return;
    }
    /* groups "ID[:N],...[@F]" separated by ';', each applied once */
    for (; *spec != 0 && group < 32; group++) {
        const char *end = strchr(spec, ';'), *at = strchr(spec, '@');
        if (end == NULL) {
            end = spec + strlen(spec);
        }
        if (at != NULL && at > end) {
            at = NULL;
        }
        if (!(sDone & (1u << group)) && (at == NULL || frame >= atoi(at + 1))) {
            const char *q = spec;
            sDone |= 1u << group;
            while (q < end && *q != '@') {
                char *e;
                long id = strtol(q, &e, 0), n = 1;
                if (e == q) {
                    break;
                }
                q = e;
                if (*q == ':') {
                    n = strtol(q + 1, &e, 0);
                    q = e;
                }
                if (id >= 4000 && id < 4000 + 1024 && n >= 0 && n <= 15) {
                    p->m_Data.m_SpecialPickupCount[id - 4000] = (u8) ((n << 4) | n);
                    printf("debug: T2_GIVE_ITEMS: frame %d level %d item %ld x%ld\n", frame, (int) gScene.m_nLevel, id, n);
                }
                while (*q == ',' || *q == ' ') {
                    q++;
                }
            }
        }
        spec = *end == ';' ? end + 1 : end;
    }
}

/* T2_BOSS_LOG=1 (game.cpp, every frame; DEBUG-ONLY, round 10 y2): one line whenever the boss of
 * the level changes its CAI mode or its fight state (the Blind Ones phase, the Queens step and
 * wave, the Mothers phase, the Primagens stage/state/mode), to follow the phase scripts
 * frame by frame between the T2_STATE_LOG dumps. */
void T2Port_TraceBoss(int frame) {
    static int sMode = -999, sA = -999, sB = -999, sC = -999, sLevel = -1;
    CGameObjectInstance *boss = NULL;
    int a = 0, b = 0, c = 0, mode;

    if (gEngineApp.m_Settings.mode != 13) {
        return;
    }
    switch (gScene.m_nLevel) {
        case 1:
            boss = gBlindOne.boss;
            a = gBlindOne.phase;
            b = gBlindOne.script ? (int) (gBlindOne.script - sBlindOneScript) : -1;
            c = gBlindOne.minionsAlive;
            break;
        case 2:
            boss = gMantidQueen.actor;
            a = gMantidQueen.stepIndex;
            b = gMantidQueen.waveActive;
            c = gMantidQueen.toSpawn;
            break;
        case 3:
            boss = gMotherBoss.obj;
            a = gMotherBoss.phase;
            b = gMotherBoss.partsLeft;
            break;
        case 4:
            boss = gLevelScript.actor;
            a = gLevelScript.stage;
            b = gLevelScript.bossState;
            c = gLevelScript.bossMode;
            break;
        default:
            return;
    }
    mode = boss != NULL ? (int) boss->m_AI.m_Mode : -1;
    if (sLevel != gScene.m_nLevel || mode != sMode || a != sA || b != sB || c != sC) {
        printf("bosstrace: frame %d level %d mode %d -> %d state %d %d %d hp %d\n", frame,
               (int) gScene.m_nLevel, sMode, mode, a, b, c, boss != NULL ? (int) boss->m_AI.m_Health : -1);
        fflush(stdout);
        sLevel = gScene.m_nLevel;
        sMode = mode;
        sA = a;
        sB = b;
        sC = c;
    }
}

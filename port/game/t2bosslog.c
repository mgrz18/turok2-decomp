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
    printf("bosshurt: %s obj %p dmg %d flags %x frame %p nodes %x factor %.2f src %p\n", who, obj,
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
            if (ls->partActive) {
                return 2;
            }
            return (ls->partsDone & 0xFF) != 0 ? (ls->partsDone & 0xFF) : -1;
        }
        return -1;
    }
    return (s32) strtol(spec, NULL, 0);
}

void T2Port_AimNode(int frame, const char *spec) {
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

    if (p == NULL || p->m_pCamera == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_pActors == NULL ||
        gScene.m_WarpState != 0) {
        return;
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
        return;
    }
    pl->m_RotY += AI_AngleToPoint(pl, bestAim);
    dx = bestAim.x - eye.x;
    dy = bestAim.y - eye.y;
    dz = bestAim.z - eye.z;
    h = sqrtf(dx * dx + dz * dz);
    p->m_pCamera->m_ViewParams.m_vRotOffset.x = (sign != NULL ? (f32) atof(sign) : 1.0f) * atan2f(dy, h);
    if (bestNode != sLastMask || frame - sLastFrame >= 300) {
        sLastMask = bestNode;
        sLastFrame = frame;
        printf("aim: frame %d eye (%.1f %.1f %.1f) -> actor %d node %x at (%.1f %.1f %.1f) pitch %.2f\n", frame,
               eye.x, eye.y, eye.z, (best >= gScene.m_pActors && best < gScene.m_pActors + gScene.m_nActors) ? (int) (best - gScene.m_pActors) : -1, (unsigned) bestNode, bestAim.x, bestAim.y,
               bestAim.z, p->m_pCamera->m_ViewParams.m_vRotOffset.x);
        fflush(stdout);
    }
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
        gEngineApp_m_Settings.cheatFlags |= (s32) strtol(spec, NULL, 0);
    }
    /* T2_FILL_AMMO=N: every N frames the active players' ammo is filled (CPlayer__FillAllAmmo) */
    if (fill != NULL && atoi(fill) > 0 && frame % atoi(fill) == 0) {
        for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            CPlayer__FillAllAmmo(p);
        }
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

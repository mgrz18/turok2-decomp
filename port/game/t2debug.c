/* t2debug.c: DEBUG-ONLY bring-up switches for `turok2 --game` (docs/port/STATUS.md, round 4 s2).
 *
 * The game cannot leave mode 0 (the Controller Pak check) until the PIF/Controller Pak is
 * modelled (s1, docs/port/INPUT.md). To follow the boot past it these switches move the engine
 * on by itself. They are bring-up aids, not options for players, and go away once input works:
 *
 *   --skip-pak-check  mode 0 ends as if "continue without saving" had been chosen: gPakCheckDone
 *                     is set once mode 0 has started, so EngineMode00_PakCheck_Retrace takes its
 *                     own exit (CEngineApp__SetupFadeTo(app, 1): the legal screen, then the
 *                     modes the game chooses itself).
 *   --start-mode N    once mode 0 has started, switch to mode N (CEngineApp__SetMode: through
 *                     mode 17 like any switch). 7 is the title screen.
 *
 * Called from game.cpp between retraces, while every game thread is blocked. */
#include "common.h"
#include "structs/game.h"
#include "globals/pak.h"
#include "globals/engine.h"

void CEngineApp__SetMode(CEngineApp *app, s32 mode);

int T2Port_EngineMode(void) {
    return gEngineApp.m_Settings.mode;
}

/* skipPak: 1 for --skip-pak-check; startMode: -1, or the mode for --start-mode. */
void T2Port_DebugSwitches(int skipPak, int startMode) {
    static int sStartDone;

    if (gEngineApp.m_Settings.mode != 0 || gPakCheckStarted == 0) {
        return;
    }
    if (startMode >= 0 && !sStartDone) {
        sStartDone = 1;
        CEngineApp__SetMode(&gEngineApp, startMode);
        return;
    }
    if (skipPak) {
        gPakCheckDone = 1;
    }
}

/* T2_STATE_LOG=1 (game.cpp, every 60 frames): the level and the first active player
 * (position, facing, health, its body's AI mode, weapon), to follow gameplay headless. */
#include "structs/world.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "structs/camera.h"
#include "structs/menu.h"
#include "protos/menu.h"
#include "structs/controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs/rect.h"
#include "globals/engine.h"
#include "structs/weapon.h"

#include <math.h>
#include "structs/intelligence.h"
#include "structs/boss.h"

/* T2_ACTOR_LOG=N (with T2_STATE_LOG, round 7 v3): the level's enemies and turrets (gScene's level
 * actors with intelligence type 1 or 4), nearest to the player first (N of them, default 8), and
 * the live pooled instances: index, type, flags, position, distance, health, AI mode, animation. */
static void T2Port_LogOne(const char *kind, int i, CGameObjectInstance *o, CGameObjectInstance *pl) {
    CIntelligence *info = o->header.m_pIntelligence;
    f32 d = -1.0f;
    if (pl != NULL) {
        f32 dx = o->header.m_vPos.x - pl->header.m_vPos.x, dy = o->header.m_vPos.y - pl->header.m_vPos.y;
        f32 dz = o->header.m_vPos.z - pl->header.m_vPos.z;
        d = sqrtf(dx * dx + dy * dy + dz * dz);
    }
    printf("actor: %s %d it %d id %d fl %x aifl %x pos (%.1f %.1f %.1f) d %.0f hp %d/%d mode %d act %d anim %d %.1f/%d tgt %p ty %x cb %p drop %x/%x aianim %x/%d/%d start %x\n",
           kind, i, info != NULL ? (int) info->m_Type : -1, info != NULL ? (int) info->m_Id : -1,
           (unsigned) o->m_dwFlags, (unsigned) o->m_AI.m_dwFlags, o->header.m_vPos.x, o->header.m_vPos.y,
           o->header.m_vPos.z, d, (int) (o->m_AI.m_Health >> 8), (int) (o->m_AI.m_InitialHealth >> 8),
           (int) o->m_AI.m_Mode, (int) o->m_AI.m_Action, (int) o->m_CurrentAnimation.m_nAnimIndex,
           o->m_CurrentAnimation.m_AnimationFrame, (int) o->m_nAnims, (void *) o->m_AI.m_pSightTarget,
           (unsigned) o->m_nTypeFlag, o->m_AI.m_pModeCurrent != NULL ? (void *) o->m_AI.m_pModeCurrent->m_pfCallback : NULL,
           info != NULL && info->m_Type == INTELLIGENCE_ENEMY ? (unsigned) info->m_Enemy.m_DropMask : 0u,
           info != NULL && info->m_Type == INTELLIGENCE_ENEMY ? (unsigned) info->m_Enemy.m_DropMask2 : 0u,
           (unsigned) (u16) o->m_AI.m_AnimType, (int) o->m_AI.m_AnimIndex, (int) o->m_AI.m_AnimRepeatedCount,
           info != NULL && info->m_Type == INTELLIGENCE_ENEMY ? (unsigned) info->m_Enemy.m_StartAnim : 0u);
}

static void T2Port_LogActors(CPlayer *p) {
    CGameObjectInstance *pl = (p != NULL) ? &p->m_CharacterActor : NULL;
    int n = atoi(getenv("T2_ACTOR_LOG"));
    int i, k, used[64];
    int nUsed = 0;
    if (n <= 0) {
        n = 8;
    }
    if (n > 64) {
        n = 64;
    }
    {
        int hist[16] = {0};
        for (i = gScene.m_nFirstActor; gScene.m_pActors != NULL && i < gScene.m_nActors; i++) {
            CIntelligence *info = gScene.m_pActors[i].header.m_pIntelligence;
            hist[info == NULL ? 15 : (info->m_Type & 15)]++;
        }
        printf("actors: %p first %d n %d by type:", (void *) gScene.m_pActors, (int) gScene.m_nFirstActor,
               (int) gScene.m_nActors);
        for (i = 0; i < 16; i++) {
            if (hist[i] != 0) {
                printf(" %d:%d", i, hist[i]);
            }
        }
        printf("\n");
    }
    if (gScene.m_pActors != NULL) {
        for (k = 0; k < n; k++) {
            int best = -1;
            f32 bestD = 1e30f;
            for (i = gScene.m_nFirstActor; i < gScene.m_nActors; i++) {
                CGameObjectInstance *o = &gScene.m_pActors[i];
                CIntelligence *info = o->header.m_pIntelligence;
                int j, skip = 0;
                f32 dx, dz, d;
                /* T2_ACTOR_TYPES=a,b,... (round 8 w5): log these intelligence types instead (6 doors, 8 pickups...) */
                static int sMask = -1;
                if (sMask < 0) {
                    const char *t = getenv("T2_ACTOR_TYPES");
                    sMask = (1 << INTELLIGENCE_ENEMY) | (1 << INTELLIGENCE_TURRET);
                    if (t != NULL) {
                        sMask = 0;
                        while (*t != 0) {
                            sMask |= 1 << (atoi(t) & 31);
                            while (*t != 0 && *t != ',') t++;
                            if (*t == ',') t++;
                        }
                    }
                }
                if (info == NULL || !(sMask & (1 << (info->m_Type & 31)))) {
                    continue;
                }
                for (j = 0; j < nUsed; j++) {
                    skip |= used[j] == i;
                }
                if (skip) {
                    continue;
                }
                dx = pl != NULL ? o->header.m_vPos.x - pl->header.m_vPos.x : 0.0f;
                dz = pl != NULL ? o->header.m_vPos.z - pl->header.m_vPos.z : 0.0f;
                d = dx * dx + dz * dz;
                if (d < bestD) {
                    bestD = d;
                    best = i;
                }
            }
            if (best < 0) {
                break;
            }
            used[nUsed++] = best;
            T2Port_LogOne("lvl", best, &gScene.m_pActors[best], pl);
        }
    }
    {
        CListedInstance *li = (CListedInstance *) gScene.m_InstancePool.m_ActiveList.m_pHead;
        for (k = 0; li != NULL && k < 32; k++, li = li->m_pNext) {
            T2Port_LogOne("dyn", k, &li->m_Instance, pl);
        }
    }
}

void T2Port_LogState(int frame) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    printf("state: frame %d mode %d level %d warp %d paused %d", frame, (int) gEngineApp.m_Settings.mode,
           (int) gScene.m_nLevel, (int) gScene.m_WarpState, (int) gEngineApp.m_Settings.paused);
    if (p != NULL) {
        CGameObjectInstance *a = &p->m_CharacterActor;
        printf(" player pos (%.1f %.1f %.1f) rotY %.2f health %d aimode %d weapon %d",
               a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z, a->m_RotY,
               (int) (p->m_Data.m_Health >> 8), (int) a->m_AI.m_Mode, (int) p->m_Data.m_CurrentWeapon);
        if (a->header.m_pCurrentRegion != NULL) {
            extern f32 CGameRegion__GetHeight(void *, f32, f32);
            printf(" region %p floor %.1f", (void *) a->header.m_pCurrentRegion,
                   CGameRegion__GetHeight(a->header.m_pCurrentRegion, a->header.m_vPos.x, a->header.m_vPos.z));
        } else {
            printf(" region NULL");
        }
        printf(" lives %d", (int) p->m_Data.m_Lives);
        {
            /* round 7 (v2): the current weapon's ammo, what the HUD counter shows */
            extern s32 CPlayer__GetWeaponAmmo(CPlayer *, s32);
            printf(" ammo %d", (int) CPlayer__GetWeaponAmmo(p, p->m_Data.m_CurrentWeapon));
        }
        /* weapons (round 6): the requested weapon, the wheel (state, count, cursor), the pressed
         * bits of this frame */
        printf(" reqw %d wheel %d/%d/%d single %x", (int) p->m_RequestedWeapon, (int) p->m_Barrel.m_State,
               (int) p->m_Barrel.m_nCount, (int) p->m_Barrel.m_nCursor, (unsigned) p->m_ContState.m_dwButtonsSingle);
        /* the weapon actor: its AI mode, animation repeats, animation index and frame */
        printf(" wact %d/%d anim %d %.1f", (int) p->m_WeaponActor.m_AI.m_Mode,
               (int) p->m_WeaponActor.m_AI.m_AnimRepeatedCount,
               (int) p->m_WeaponActor.m_CurrentAnimation.m_nAnimIndex,
               p->m_WeaponActor.m_CurrentAnimation.m_AnimationFrame);
        printf(" pmode %d buttons %x move (%.2f %.2f) look (%.2f %.2f) contp %p", (int) p->m_Mode,
               (unsigned) p->m_ContState.m_dwButtons, p->m_ContState.m_MoveX, p->m_ContState.m_MoveY,
               p->m_ContState.m_LookX, p->m_ContState.m_LookY, (void *) p->m_ContState.m_pContState);
        if (p->m_ContState.m_pContState != NULL) {
            ControllerProfile *c = p->m_ContState.m_pContState;
            printf(" pad %x/%x stick %d %d", (unsigned) c->inputButtons, (unsigned) c->buttons, c->stickX, c->stickY);
        }
    }
    if (p != NULL && p->m_pCamera != NULL && ((MenuPool *) p->m_pCamera->m_MenuPool)->head != NULL) {
        Menu *m = ((MenuPool *) p->m_pCamera->m_MenuPool)->head;
        printf(" cammenu %p cursor %d", (void *) m->desc, (int) m->cursor);
    }
    {
        extern s32 gCameraPoolHideViews;
        CCameraPool *pool = gEngineApp.m_Game.m_CameraPool;
        printf(" cams %d hide %d", (int) pool->m_ActiveList.m_nCount, (int) gCameraPoolHideViews);
    }
    printf("\n");
    if (getenv("T2_SCENE_LOG") != NULL) {
        /* T2_SCENE_LOG=1 (with T2_STATE_LOG): the view of the first player's camera, what
         * CScene__DrawGeometry culls with, and the loaded grid sections */
        CCamera *c = (p != NULL) ? p->m_pCamera : NULL;
        int i;
        if (c != NULL) {
            printf("scene: cam pos (%.1f %.1f %.1f) view (%.1f %.1f %.1f)-(%.1f %.1f %.1f) far %.1f fov %.1f vis %x region %p\n",
                   c->m_vPos.x, c->m_vPos.y, c->m_vPos.z, c->m_ViewMin.x, c->m_ViewMin.y, c->m_ViewMin.z,
                   c->m_ViewMax.x, c->m_ViewMax.y, c->m_ViewMax.z, c->m_FarClip, c->m_FieldOfView,
                   (unsigned) c->m_ViewParams.m_VisBits, (void *) c->m_ViewParams.m_pCurrentRegion);
        }
        {
            extern CBox gCameraVisibleBox;
            extern CBox gCameraViewBox;
            printf("scene: visible box (%.1f %.1f %.1f)-(%.1f %.1f %.1f) view box (%.1f %.1f %.1f)-(%.1f %.1f %.1f)\n",
                   gCameraVisibleBox.m_vMin.x, gCameraVisibleBox.m_vMin.y, gCameraVisibleBox.m_vMin.z,
                   gCameraVisibleBox.m_vMax.x, gCameraVisibleBox.m_vMax.y, gCameraVisibleBox.m_vMax.z,
                   gCameraViewBox.m_vMin.x, gCameraViewBox.m_vMin.y, gCameraViewBox.m_vMin.z,
                   gCameraViewBox.m_vMax.x, gCameraViewBox.m_vMax.y, gCameraViewBox.m_vMax.z);
            if (c != NULL) {
                printf("scene: cam anim (%.1f %.1f %.1f)-(%.1f %.1f %.1f) next %p\n", c->m_AnimMin.x, c->m_AnimMin.y,
                       c->m_AnimMin.z, c->m_AnimMax.x, c->m_AnimMax.y, c->m_AnimMax.z, (void *) c->m_pNext);
            }
        }
        printf("scene: level %d sectors %d difficulty %x multiplayer %d cheats %x\n", (int) gScene.m_nLevel,
               (int) gScene.m_nSectors, (unsigned) gEngineApp_m_Settings.difficulty,
               (int) gEngineApp_m_Settings.multiplayer, (unsigned) gEngineApp_m_Settings.cheatFlags);
        for (i = 0; i < gScene.m_nSectors && i < 8; i++) {
            CRect *r = (CRect *) gScene.m_Sectors[i].m_pBounds;
            printf("scene:   sector %d x %.0f..%.0f z %.0f..%.0f\n", i, r->minX, r->maxX, r->minZ, r->maxZ);
        }
    }
    if (getenv("T2_ACTOR_LOG") != NULL) {
        T2Port_LogActors(p);
    }
    if (getenv("T2_BOSS_LOG") != NULL) {
        extern void T2Port_LogBosses(void); /* t2bosslog.c, DEBUG-ONLY */
        T2Port_LogBosses();
    }
    {
        /* a totem defense (the energy totem levels, TotemDefense_*): state 0 fight, 1 saved,
         * 2 failed; the time left, the kills against the definition's target (round 9 x1) */
        extern TotemDefenseState gTotemDefense;
        if (gTotemDefense.obj != NULL && gScene.m_nLevel > 0) {
            CDestructibleIntelligence *d = (CDestructibleIntelligence *) gTotemDefense.def;
            printf("totem: state %d time %.0f/%d s kills %d/%d attackers %d warned %d totem hp %d\n",
                   (int) gTotemDefense.state, gTotemDefense.timer / 15.0f, d != NULL ? (int) d->m_TotemMissionDuration : -1,
                   (int) gTotemDefense.kills, d != NULL ? (int) d->m_EnemiesToKillToSaveTotem : -1,
                   d != NULL ? (int) d->m_EnemiesAttackingTurok : -1, (int) gTotemDefense.warned,
                   (int) (gTotemDefense.obj->m_AI.m_Health >> 8));
        }
    }
    fflush(stdout);
}

/* T2_GIVE_WEAPONS=1 (game.cpp, every frame; DEBUG-ONLY, round 7 v2): in gameplay (mode 13, a
 * level loaded) every active player gets every weapon of gWeaponInfoTable that can be used on
 * land or under water, with full ammo (CPlayer__FillAllAmmo), once per player and level. To
 * test firing and the weapons without finding their pickups; not a game option. T2_WEAPON=<n>
 * also requests weapon n, T2_WEAPON_LIST=1 prints the weapon table. */
void T2Port_GiveWeapons(void) {
    extern s32 CPlayer__FillAllAmmo(CPlayer *);
    static CPlayer *sGiven[4];
    static int sLevel = -1;
    static int sEnabled = -1;
    CPlayer *p;
    int i;

    if (sEnabled < 0) {
        const char *e = getenv("T2_GIVE_WEAPONS");
        sEnabled = (e != NULL && e[0] == '1');
    }
    if (!sEnabled || gEngineApp.m_Settings.mode != 13 || gScene.m_nLevel <= 0) {
        return;
    }
    if (gScene.m_nLevel != sLevel) {
        sLevel = gScene.m_nLevel;
        for (i = 0; i < 4; i++) {
            sGiven[i] = NULL;
        }
    }
    for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        int slot = -1;
        for (i = 0; i < 4; i++) {
            if (sGiven[i] == p) {
                slot = -2;
                break;
            }
            if (sGiven[i] == NULL && slot == -1) {
                slot = i;
            }
        }
        if (slot < 0) {
            continue;
        }
        sGiven[slot] = p;
        for (i = 0; i < 35; i++) {
            if (gWeaponInfoTable[i] != NULL && (gWeaponInfoTable[i]->flags & 3) != 0) {
                p->m_Data.m_WeaponList[i] = 1;
            }
        }
        CPlayer__FillAllAmmo(p);
        if (getenv("T2_WEAPON") != NULL) {
            /* T2_WEAPON=<n>: request weapon n (the weapon actor switches to it as from the wheel) */
            p->m_RequestedWeapon = (s16) atoi(getenv("T2_WEAPON"));
        }
        if (getenv("T2_WEAPON_LIST") != NULL) {
            /* T2_WEAPON_LIST=1: the weapon table (name, flags, model, first ammo class) */
            for (i = 0; i < 35; i++) {
                WeaponInfo *w = gWeaponInfoTable[i];
                if (w == NULL) {
                    continue;
                }
                printf("debug: weapon %2d %-24s flags %x model %d fireDelay %.2f ammo0 %d/%d proj %d cost %d\n", i,
                       w->name != NULL ? (const char *) w->name : "-", (unsigned) w->flags, (int) w->model,
                       w->fireDelay, w->ammo[0] != NULL ? w->ammo[0]->type : -1,
                       w->ammo[0] != NULL ? w->ammo[0]->max : -1, w->ammo[0] != NULL ? w->ammo[0]->projectile : -1,
                       w->ammo[0] != NULL ? w->ammo[0]->cost : -1);
            }
        }
        printf("debug: T2_GIVE_WEAPONS: level %d player %p: all weapons, full ammo\n", (int) gScene.m_nLevel,
               (void *) p);
    }
}

/* T2_WARP_TO=<id>[@frame][,<id>[@frame]...] (DEBUG-ONLY, round 7 v5): a level warp through the
 * game's own path. At <frame> (default: 60 frames into gameplay: mode 13 in a level > 0, a player,
 * no warp running) CPlayer__Warp(player, id, 2, 0) runs as if the player had entered a region
 * whose CRegionInfo.m_WarpID is <id>: the hub/key redirects, CScene__FindWarpDests, then either
 * a move inside the loaded level or CScene__RequestWarp (the fade, CScene__UpdateWarp,
 * CScene__LoadLevel of the destination). Warp ids: tools/assets/levels.py (warp_dests.json).
 * The entries run in order, each once its frame is reached and no warp is running. */
void CPlayer__Warp(CPlayer *player, s32 warp, s32 arg2, s32 respawn);

void T2Port_DebugWarp(int frame) {
    static const char *spec;
    static int init;
    static int sGameplayStart = -1;
    static int sDone; /* how many entries of the list have run */
    CPlayer *p;
    const char *s;
    char *end;
    long id;
    long at;
    int i;

    if (!init) {
        init = 1;
        spec = getenv("T2_WARP_TO");
    }
    if (spec == NULL) {
        return;
    }
    p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    /* gameplay: mode 13 in a level (level 0 is the backdrop of the single-player menu) */
    if (gEngineApp.m_Settings.mode != 13 || gScene.m_nLevel <= 0) {
        sGameplayStart = -1;
    } else if (sGameplayStart < 0 && p != NULL && gScene.m_WarpState == 0) {
        sGameplayStart = frame;
    }
    s = spec;
    for (i = 0; i < sDone && s != NULL; i++) {
        s = strchr(s, ',');
        if (s != NULL) {
            s++;
        }
    }
    if (s == NULL || *s == 0) {
        return;
    }
    id = strtol(s, &end, 0);
    at = (*end == '@') ? strtol(end + 1, NULL, 0) : -1;
    if (at < 0 ? (sGameplayStart < 0 || frame < sGameplayStart + 60) : frame < at) {
        return;
    }
    if (p == NULL || gScene.m_WarpState != 0 || sGameplayStart < 0) {
        return;
    }
    printf("debug: frame %d: T2_WARP_TO: CPlayer__Warp(player, %ld, 2, 0) in level %d\n", frame, id,
           (int) gScene.m_nLevel);
    fflush(stdout);
    sDone++;
    CPlayer__Warp(p, (s32) id, 2, 0);
}

/* T2_TELEPORT=F:x,y,z[,rotY][!][;F:x,y,z...] (DEBUG-ONLY, round 7 v3): at frame F, in gameplay, put the
 * first active player at (x, y, z) (its region looked up with CScene__FindRegionAt), to reach a
 * level's enemies without a long --input script. Called every frame by game.cpp. */
extern sptr CScene__FindRegionAt(CScene *world, CVector3 *pos);
void T2Port_Teleport(int frame, const char *spec) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    const char *s = spec;

    if (p == NULL || gEngineApp.m_Settings.mode != 13) {
        return;
    }
    while (s != NULL && *s != '\0') {
        int f, n;
        float x, y, z, ry;
        if ((n = sscanf(s, "%d:%f,%f,%f,%f", &f, &x, &y, &z, &ry)) >= 4 && f == frame) {
            CGameObjectInstance *a = &p->m_CharacterActor;
            CVector3 pos;
            sptr region;
            pos.x = x;
            pos.y = y;
            pos.z = z;
            region = CScene__FindRegionAt(&gScene, &pos);
            printf("teleport: frame %d player (%.1f %.1f %.1f) -> (%.1f %.1f %.1f) region %p\n", frame,
                   a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z, x, y, z, (void *) region);
            if (region != 0) {
                a->header.m_vPos = pos;
                a->header.m_pCurrentRegion = (struct CGameRegion *) region;
                a->m_vVelocity.x = a->m_vVelocity.y = a->m_vVelocity.z = 0.0f;
                if (n == 5) {
                    a->m_RotY = ry; /* F:x,y,z,rotY: also the facing (round 9 x1) */
                }
                {
                    /* F:x,y,z[,rotY]! (round 10 y1): also fire the region's "entered" triggers
                     * (CGameRegion__FireTriggers(region, 1), what the player walking into it does in
                     * CPlayer's move, func_0024C47C): the level's own region events (enemy wake
                     * ups, "graveyard located." ...) that a teleport would otherwise skip */
                    const char *end = strchr(s, ';');
                    const char *bang = strchr(s, '!');
                    if (bang != NULL && (end == NULL || bang < end)) {
                        extern void CGameRegion__FireTriggers(sptr arg0, s32 arg1, sptr arg2);
                        printf("teleport: frame %d: the region's enter triggers\n", frame);
                        CGameRegion__FireTriggers(region, 1, (sptr) p);
                    }
                }
            }
            fflush(stdout);
        }
        s = strchr(s, ';');
        if (s != NULL) {
            s++;
        }
    }
}

/* T2_WAKE_ENEMIES=R (DEBUG-ONLY, round 7 v3): in gameplay, wake every dormant level enemy within R
 * units of the player (default 1500) the way its level event does (EnemyAI_SleepIfOffscreen, the
 * enemy event handler 1: flags 0x2100 and mode 2). Enemies whose definition has flag 0x1000 sleep until a
 * level trigger wakes them; this reaches them without the trigger. Called every frame. */
extern void EnemyAI_SleepIfOffscreen(CGameObjectInstance *actor, CAI *ai, s32 unused);
void T2Port_WakeEnemies(int frame, const char *spec) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    f32 r = (f32) atof(spec);
    int i;

    if (p == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_pActors == NULL) {
        return;
    }
    if (r <= 0.0f) {
        r = 1500.0f;
    }
    for (i = gScene.m_nFirstActor; i < gScene.m_nActors; i++) {
        CGameObjectInstance *o = &gScene.m_pActors[i];
        CIntelligence *info = o->header.m_pIntelligence;
        f32 dx, dz;
        s32 dummy = 0;
        if (info == NULL || info->m_Type != INTELLIGENCE_ENEMY || (o->m_dwFlags & 0x100)) {
            continue;
        }
        dx = o->header.m_vPos.x - p->m_CharacterActor.header.m_vPos.x;
        dz = o->header.m_vPos.z - p->m_CharacterActor.header.m_vPos.z;
        if (dx * dx + dz * dz < r * r) {
            EnemyAI_SleepIfOffscreen(o, &o->m_AI, dummy);
            printf("wake: frame %d enemy %d type %x at (%.1f %.1f %.1f) flags %x\n", frame, i, (unsigned) o->m_nTypeFlag,
                   o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, (unsigned) o->m_dwFlags);
        }
    }
}

/* T2_FACE_NEAREST=R (DEBUG-ONLY, round 8 w2): in gameplay, every frame, turn the first player to
 * face the nearest live level enemy (intelligence type 1, the live flag 0x100, health > 0)
 * within R units (default 2000), by adding AI_AngleToPoint (the angle from his heading to the
 * enemy) to his yaw. An aim assist for scripted firing tests (--input Z@...): the stick cannot
 * aim at a moving enemy from a script. Not a game option. */
extern f32 AI_AngleToPoint(CGameObjectInstance *a, CVector3 point);
void T2Port_FaceNearest(int frame, const char *spec) {
    static CGameObjectInstance *sLast;
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    CGameObjectInstance *pl, *best = NULL;
    f32 r = (f32) atof(spec), bestD;
    int i;

    if (p == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_pActors == NULL || gScene.m_WarpState != 0) {
        return;
    }
    if (strchr(spec, '@') != NULL && atoi(strchr(spec, '@') + 1) != gScene.m_nLevel) {
        return; /* R@LEVEL: only in that level (round 9 x1: the totem fight of a scripted run) */
    }
    if (r <= 0.0f) {
        r = 2000.0f;
    }
    pl = &p->m_CharacterActor;
    bestD = r * r;
    for (i = gScene.m_nFirstActor; i < gScene.m_nActors; i++) {
        CGameObjectInstance *o = &gScene.m_pActors[i];
        CIntelligence *info = o->header.m_pIntelligence;
        f32 dx, dz, d;
        if (info == NULL || info->m_Type != INTELLIGENCE_ENEMY || !(o->m_dwFlags & 0x100) || o->m_AI.m_Health <= 0) {
            continue;
        }
        dx = o->header.m_vPos.x - pl->header.m_vPos.x;
        dz = o->header.m_vPos.z - pl->header.m_vPos.z;
        d = dx * dx + dz * dz;
        if (d < bestD) {
            bestD = d;
            best = o;
        }
    }
    {
        /* also the pooled instances (generator spawns: the totem defense waves; round 9 x1) */
        CListedInstance *li;
        for (li = gScene.m_InstancePool.m_ActiveList.m_pHead; li != NULL; li = li->m_pNext) {
            CGameObjectInstance *o = &li->m_Instance;
            CIntelligence *info = o->header.m_pIntelligence;
            f32 dx, dz, d;
            if (info == NULL || info->m_Type != INTELLIGENCE_ENEMY || o->m_AI.m_Health <= 0) {
                continue;
            }
            dx = o->header.m_vPos.x - pl->header.m_vPos.x;
            dz = o->header.m_vPos.z - pl->header.m_vPos.z;
            d = dx * dx + dz * dz;
            if (d < bestD) {
                bestD = d;
                best = o;
            }
        }
    }
    if (best == NULL) {
        return;
    }
    pl->m_RotY += AI_AngleToPoint(pl, best->header.m_vPos);
    if (best != sLast) {
        sLast = best;
        printf("face: frame %d player (%.1f %.1f %.1f) -> enemy %d type %x at (%.1f %.1f %.1f) d %.0f rotY %.2f\n", frame,
               pl->header.m_vPos.x, pl->header.m_vPos.y, pl->header.m_vPos.z, (best >= gScene.m_pActors && best < gScene.m_pActors + gScene.m_nActors) ? (int) (best - gScene.m_pActors) : -1 /* -1: a pooled instance */,
               (unsigned) best->m_nTypeFlag, best->header.m_vPos.x, best->header.m_vPos.y, best->header.m_vPos.z,
               sqrtf(bestD), pl->m_RotY);
        fflush(stdout);
    }
}

/* T2_AIM_AT=F1-F2:x,y,z[;...] (DEBUG-ONLY, round 10 y1): from frame F1 to F2, in gameplay, turn the
 * first player (yaw, and the camera pitch as T2_AIM_NODE does) at the point (x, y, z): a scripted
 * shot at a level object that is not an enemy (the River of Souls' soul gates, a target). */
void T2Port_AimAt(int frame, const char *spec) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    const char *s = spec;

    if (p == NULL || p->m_pCamera == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_WarpState != 0) {
        return;
    }
    while (s != NULL && *s != '\0') {
        int f1, f2;
        float x, y, z;
        if (sscanf(s, "%d-%d:%f,%f,%f", &f1, &f2, &x, &y, &z) == 5 && frame >= f1 && frame <= f2) {
            CGameObjectInstance *pl = &p->m_CharacterActor;
            CVector3 eye = p->m_pCamera->m_vPos, at;
            f32 dx = x - eye.x, dy = y - eye.y, dz = z - eye.z;
            at.x = x;
            at.y = y;
            at.z = z;
            pl->m_RotY += AI_AngleToPoint(pl, at);
            p->m_pCamera->m_ViewParams.m_vRotOffset.x = atan2f(dy, sqrtf(dx * dx + dz * dz));
            if (frame == f1) {
                printf("aimat: frame %d eye (%.1f %.1f %.1f) -> (%.1f %.1f %.1f) pitch %.2f\n", frame, eye.x, eye.y,
                       eye.z, x, y, z, p->m_pCamera->m_ViewParams.m_vRotOffset.x);
                fflush(stdout);
            }
            return;
        }
        s = strchr(s, ';');
        if (s != NULL) {
            s++;
        }
    }
}

/* T2_AIM_ENEMY=F1-F2[,R][;...] (DEBUG-ONLY, round 10 y1): T2_AIM_NODE=-1 (yaw and pitch at the
 * nearest live actor within R, default 4000) only from frame F1 to F2: the fights of a scripted run
 * between its walks (a flying Sister of Despair needs the pitch). */
void T2Port_AimNode(int frame, const char *spec);
void T2Port_AimEnemy(int frame, const char *spec) {
    const char *s = spec;

    while (s != NULL && *s != '\0') {
        int f1, f2, r = 4000;
        if (sscanf(s, "%d-%d,%d", &f1, &f2, &r) >= 2 && frame >= f1 && frame <= f2) {
            char buf[32];
            snprintf(buf, sizeof(buf), "-1,%d", r);
            T2Port_AimNode(frame, buf);
            return;
        }
        s = strchr(s, ';');
        if (s != NULL) {
            s++;
        }
    }
}

/* T2_SAVE_PORTAL=F (DEBUG-ONLY, round 8 w5): at frame F, in gameplay, open the save portal menu
 * (gSavePortalMenuDesc) for the first player, as CPlayerPool__Update does when a save portal's
 * cinema (script 400) ends, to reach the game's own save path (PakMenu_InitSaveGame,
 * SaveGame_Build, PakMenu_WriteSaveGame) without walking to a portal. Called every frame. */
extern struct MenuDesc gSavePortalMenuDesc;
void T2Port_SavePortal(int frame, const char *spec) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    if (p == NULL || p->m_pCamera == NULL || gEngineApp.m_Settings.mode != 13 || frame != atoi(spec)) {
        return;
    }
    printf("debug: frame %d: T2_SAVE_PORTAL: the save portal menu in level %d\n", frame, (int) gScene.m_nLevel);
    fflush(stdout);
    MenuPool_OpenMenu((struct MenuPool *) p->m_pCamera->m_MenuPool, &gSavePortalMenuDesc, (sptr) p,
                      (sptr) p->m_ContState.m_pContState, 0);
}

/* T2_PICKUP_LOG=1 (DEBUG-ONLY, round 8 w5): CPlayer__TouchPickup reports every pickup the player
 * touches: its object type id, where it lies, whether CPlayer__CollectPickup took it, and the
 * player's health, life force, lives and current weapon ammo after it. */
void T2Port_LogPickup(sptr player, void *actor, s32 id, s32 got) {
    static int on = -1;
    CPlayer *p = (CPlayer *) player;
    CInstanceHeader *h = (CInstanceHeader *) actor;
    extern s32 CPlayer__GetWeaponAmmo(CPlayer *, s32);

    if (on < 0) {
        on = getenv("T2_PICKUP_LOG") != NULL;
    }
    if (!on) {
        return;
    }
    printf("pickup: level %d id %d at (%.1f %.1f %.1f) %s: health %d life force %d lives %d weapon %d ammo %d\n",
           (int) gScene.m_nLevel, (int) id, h->m_vPos.x, h->m_vPos.y, h->m_vPos.z, got ? "collected" : "not taken",
           (int) (p->m_Data.m_Health >> 8), (int) p->m_Data.m_LifeForce, (int) p->m_Data.m_Lives,
           (int) p->m_Data.m_CurrentWeapon, (int) CPlayer__GetWeaponAmmo(p, p->m_Data.m_CurrentWeapon));
    fflush(stdout);
}

/* T2_DOOR_OPEN=F (DEBUG-ONLY, round 8 w5): at frame F, every door (intelligence type 6) within 800
 * units of the player gets the event a level trigger or a key sends it (Door_EventOpen:
 * CAI__SetMode(door, ai, 0)), to check the door's open path (Door_OpenEnter, Door_OpenUpdate,
 * the open animation, the saved bit) without the trigger. Called every frame. */
extern void CAI__SetMode(CGameObjectInstance *actor, CAI *ai, s32 mode);
void T2Port_DoorOpen(int frame, const char *spec) {
    CPlayer *p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    int i;

    if (p == NULL || gEngineApp.m_Settings.mode != 13 || gScene.m_pActors == NULL || frame != atoi(spec)) {
        return;
    }
    for (i = gScene.m_nFirstActor; i < gScene.m_nActors; i++) {
        CGameObjectInstance *o = &gScene.m_pActors[i];
        CIntelligence *info = o->header.m_pIntelligence;
        f32 dx, dz;
        if (info == NULL || info->m_Type != INTELLIGENCE_DOOR) {
            continue;
        }
        dx = o->header.m_vPos.x - p->m_CharacterActor.header.m_vPos.x;
        dz = o->header.m_vPos.z - p->m_CharacterActor.header.m_vPos.z;
        if (dx * dx + dz * dz < 800.0f * 800.0f) {
            printf("door: frame %d door %d at (%.1f %.1f %.1f) mode %d -> open\n", frame, i, o->header.m_vPos.x,
                   o->header.m_vPos.y, o->header.m_vPos.z, (int) o->m_AI.m_Mode);
            CAI__SetMode(o, &o->m_AI, 0);
        }
    }
    fflush(stdout);
}

/* T2_EVENT_LOG=1 (DEBUG-ONLY, round 9 x1): LevelEvent_Fire reports every level event it fires: the
 * gameplay frame, the level, the event's actor/level, its kind and handler (gLevelEventHandlers),
 * the message (gLevelMessages: "distress beacon activated.", "child rescued.", "portal
 * activated." ...) and the music cue. To follow a level's objectives, switches and portals. */
#include "structs/event.h"
extern s32 CScene__GetLinkGroupCount(CScene *lv, s32 key);
int gT2HostFrame; /* game.cpp's frame number, for the logs */

/* The hub level (1..6, Mission_LevelObjectivesDone) a level index belongs to: its sub-levels,
 * its talisman and flesh-eater portal levels and its totem (tools/assets/levels.py names). */
int T2Port_HubLevelOf(int level) {
    if ((level >= 125 && level <= 135) || level == 73 || level == 79 || level == 110) {
        return 2;
    }
    if ((level >= 61 && level <= 71) || level == 74 || level == 80 || level == 111) {
        return 3;
    }
    if ((level >= 98 && level <= 108) || level == 75 || level == 81 || level == 112) {
        return 4;
    }
    if ((level >= 84 && level <= 96) || level == 76 || level == 82 || level == 113) {
        return 5;
    }
    if (level == 124) { /* HIVE TOP: level 5's start (round 11 z4) */
        return 5;
    }
    if ((level >= 114 && level <= 123) || level == 77 || level == 83) {
        return 6;
    }
    return 1;
}
void T2Port_LogEvent(LevelEvent *e, const char *text) {
    static int on = -1;
    char buf[96];
    int i;

    if (on < 0) {
        on = getenv("T2_EVENT_LOG") != NULL;
    }
    if (!on) {
        return;
    }
    for (i = 0; text != NULL && text[i] != 0 && i < (int) sizeof(buf) - 1; i++) {
        buf[i] = (text[i] == '\n') ? '|' : text[i];
    }
    buf[i] = 0;
    /* the objective counters of the level's hub (Mission_LevelObjectivesDone's link group types:
     * level 1: 2 = distress beacons, 3 = children; level 2 (round 10 y1): 4 = soul gates, 5 =
     * sisters of despair; 3: 6/7; 4: 8 only; level 5 (round 11 z4): 9 = embryos, 10 = the master
     * computer; level 6: 11 = power generators, 12 = assembly plants) */
    static const signed char kObjTypes[7][2] = { { 2, 3 }, { 2, 3 }, { 4, 5 }, { 6, 7 }, { 8, -1 }, { 9, 10 }, { 11, 12 } };
    int k = T2Port_HubLevelOf((int) gScene.m_nLevel);
    printf("event: frame %d (gameplay %d) level %d mode %d: actor %d of level %d kind %d handler %d msg %d \"%s\" music %d flags %x"
           " key %d objectives %d/%d\n",
           gT2HostFrame, (int) gFrameCountGameplay, (int) gScene.m_nLevel, (int) gScene.m_Mode, (int) e->actor, (int) e->level,
           (int) e->kind, (int) e->handler, (int) e->message, buf, (int) e->music, (unsigned) e->flags, (int) e->padF,
           (int) CScene__GetLinkGroupCount(&gScene, kObjTypes[k][0]),
           kObjTypes[k][1] < 0 ? -1 : (int) CScene__GetLinkGroupCount(&gScene, kObjTypes[k][1]));
    fflush(stdout);
}

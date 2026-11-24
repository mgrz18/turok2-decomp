#include "common.h"
#include "structs/cinema.h"
#include "globals/cinema.h"

void Cinema__Initialize(void) {
    Cinema *p = gCinemaPlayerP;

    p->owner = 0;
    p->resource = 0;
    p->unk8 = 0;
    p->onClose = 0;
    p->onEnd = 0;
    p->unk14 = 0;
    p->target = 0;
    *(s32 *)&p->time = 0;
    p->prevTime = 0;
    p->time24 = 0;
    p->clipStart = 0;
    p->clipEnd = 0;
    p->startTime = 0;
    p->endTime = 0;
    p->active = 0;
    p->finished = 0;
    p->started = 0;
    p->ended = 0;
    p->closed = 0;
    p->restored = 0;
    p->unk50 = 0;
    p->resetPlayer = 0;
    p->resetPlayerNext = 0;
    p->restoreAtEnd = 0;
    p->fading = 0;
    p->fade = 0;
    p->stopAtEnd = 0;
    p->saveStart = 0;
    p->startLevel = -1;
    p->startWarp = -1;
    p->startPos.x = 0;
    p->startPos.y = 0;
    p->startPos.z = 0;
    p->startRotY = 0;
    p->startRegion = 0;
    p->warpId = -1;
    p->entryIndex = -1;
    p->scriptId = -1;
    p->nextOnClose = 0;
    p->nextOnEnd = 0;
    p->unkEC = 0;
    p->position.x = 0;
    p->position.y = 0;
    p->position.z = 0;
    p->yaw = 0;
}

#include "structs/vector.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/rumble.h"
#include "protos/scene.h"
#include "cinema_scene.h"

void CPlayer__ResetControlState(CPlayer *player);
void CCameraPool__ReleaseAllTexts(void *game);
void RumblePool_FreeAll(RumblePool *pool);
s32 Cinema_NoWaitCues(void);

/* Runs once (until the state is cleared): resets the current player when
 * asked, remembering where it starts the first time; warps to the pending
 * warp id unless it is the current one; then marks the state started. */
void Cinema_Start(void) {
    CPlayer *player;
    s32 warp;
    void *world;
    s32 out[3];

    warp = gCinemaPlayerP->warpId;
    if (gCinemaPlayerP->started != 0) {
        return;
    }
    if (gCinemaPlayerP->resetPlayer != 0) {
        player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
        if (player != NULL) {
            CPlayer__ResetControlState(player);
            if (gCinemaPlayerP->saveStart != 0 && gCinemaPlayerP->startLevel == -1
                && gCinemaPlayerP->startWarp == gCinemaPlayerP->startLevel) {
                gCinemaPlayerP->startLevel = gScene_m_LevelID[0];
                gCinemaPlayerP->startWarp = gScene_m_nLevel[0];
                gCinemaPlayerP->restored = 0;
                gCinemaPlayerP->startPos = player->m_CharacterActor.header.m_vPos;
                gCinemaPlayerP->startRotY = player->m_CharacterActor.m_RotY;
                gCinemaPlayerP->startRegion = CScene__GetRegionIndex(&gScene, player->m_CharacterActor.header.m_pCurrentRegion);
            }
            player->m_WeaponActor.m_AI.m_VarBuffer[0] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[1] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[5] = 0;
        }
        CCameraPool__ReleaseAllTexts(&gEngineApp_m_Game_m_CameraPool);
        RumblePool_FreeAll(&gRumblePool);
    }
    if (warp != -1) {
        world = &gScene;
        if (gScene_m_nLevel[0] != warp) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            gScene_m_LevelID[0] = CScene__FindNearestWarpDest(world, out, warp, &gScene_m_WarpSpawn);
            gScene_m_bWarpSpawn = gScene_m_LevelID[0] != 0;
            CScene__LoadLevel(world, warp, gScene_m_bWarpSpawn);
        }
    }
    gCinemaPlayerP->restoreAtEnd = Cinema_NoWaitCues();
    gCinemaPlayerP->unk50 = 1;
    gCinemaPlayerP->warpId = -1;
    gCinemaPlayerP->started = 1;
}

void CPlayer__UpdateOrbitCamera(CPlayer *player, CPlayer *other);

/* Puts the current player back where Cinema_Start saw it start (once,
 * when gEngineApp_m_Settings_mode is 13 and a start was saved), warping there first if
 * needed; any other gEngineApp_m_Settings_mode drops the saved start. */
void Cinema_RestorePlayer(void) {
    void *world;
    CPlayer *player;
    CVector3 pos;
    f32 rotY;
    s32 region;
    s32 level;
    s32 warp;

    world = &gScene;
    level = gCinemaPlayerP->startLevel;
    warp = gCinemaPlayerP->startWarp;
    if (gEngineApp_m_Settings.mode != 13) {
        gCinemaPlayerP->restored = 1;
        gCinemaPlayerP->saveStart = 0;
        gCinemaPlayerP->startLevel = -1;
        gCinemaPlayerP->startWarp = -1;
        return;
    }
    if (gCinemaPlayerP->restored == 0 && level != -1 && warp != -1 && gCinemaPlayerP->saveStart != 0) {
        gCinemaPlayerP->restored = 1;
        pos = gCinemaPlayerP->startPos;
        rotY = gCinemaPlayerP->startRotY;
        region = gCinemaPlayerP->startRegion;
        if (gScene_m_nLevel[0] != warp) {
            CScene__LoadLevel(world, warp, 1);
        }
        player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
        gScene_m_WarpFade = 0;
        gScene_m_WarpState = 0;
        gScene_m_WarpTime = 0;
        gScene_m_LevelID[0] = level;
        gScene_m_WarpID = level;
        if (player != NULL) {
            player->m_CharacterActor.header.m_vPos = pos;
            player->m_CharacterActor.m_RotY = rotY;
            player->m_CharacterActor.header.m_pCurrentRegion = CScene__GetRegion(world, region);
            CPlayer__UpdateOrbitCamera(player, player);
        }
        gCinemaPlayerP->saveStart = 0;
        gCinemaPlayerP->startLevel = -1;
        gCinemaPlayerP->startWarp = -1;
    }
}

#include "globals/engine.h"

extern void Cinema_Load(s32);
s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
extern void Cinema_FireCues(void);

s32 Cinema_StartPending(void) {
    s32 none;
    Cinema *ctx;
    f32 value;
    f32 limit;

    none = -1;
    if (gCinemaPlayerP->entryIndex == none) {
        return 0;
    }
    Cinema_Load(gCinemaPlayerP->entryIndex);
    gCinemaPlayerP->entryIndex = none;
    if (Cinema_IsActive() != 0) {
        if (gEngineApp_m_Settings.paused != 0 || Cinema_IsFinished() != 0) {
            gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
        } else {
            ctx = gCinemaPlayerP;
            ctx->prevTime = *(volatile f32 *)&ctx->time;
            ctx->time += gFrameIncrement * 0.06666667f;
            if (ctx->startTime <= ctx->time) {
                Cinema_Start();
            }
            value = gCinemaPlayerP->time;
            limit = gCinemaPlayerP->clipEnd;
            if (limit <= value) {
                gCinemaPlayerP->time = limit;
                if (gCinemaPlayerP->stopAtEnd != 0) {
                    gCinemaPlayerP->finished = 1;
                    gCinemaPlayerP->stopAtEnd = 0;
                }
            }
        }
    }
    Cinema_FireCues();
    return 1;
}

#include "protos/cache.h"

typedef struct GameState {
    s32 busy;               /* 0x0000 */
    u8 pad4[0x1450 - 0x4];
    s32 paused;             /* 0x1450 */
} GameState;
extern GameState gEngineApp_m_Game_m_PlayerList_m_bCinema; /* local type: the field's real type (or the member path) changes the ROM's code here */
void Cinema_RunEnd(s32 restore);
void Cinema_Close(void);
extern void Sound__StopAllExcept(s32);

void Cinema_Update(void) {
    Cinema *ctx;
    f32 value;
    f32 limit;

    if (sCinemaLastFrame == gFrameCount) {
        return;
    }
    sCinemaLastFrame = gFrameCount;
    if (Cinema_StartPending() == 0) {
        if (Cinema_IsActive() != 0) {
            if (gEngineApp_m_Settings.paused != 0 || Cinema_IsFinished() != 0) {
                gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
            } else {
                ctx = gCinemaPlayerP;
                ctx->prevTime = *(volatile f32 *)&ctx->time;
                ctx->time += gFrameIncrement * 0.06666667f;
                if (ctx->startTime <= ctx->time) {
                    Cinema_Start();
                }
                value = gCinemaPlayerP->time;
                limit = gCinemaPlayerP->clipEnd;
                if (limit <= value) {
                    gCinemaPlayerP->time = limit;
                    if (gCinemaPlayerP->stopAtEnd != 0) {
                        gCinemaPlayerP->finished = 1;
                        gCinemaPlayerP->stopAtEnd = 0;
                    }
                }
            }
        }
        Cinema_FireCues();
    }
    Cinema_StartPending();
    if (gCinemaPlayerP->fading != 0 && (gCinemaPlayerP->fade -= 2.0f * (gFrameIncrement * 0.06666667f)) < -0.5f) {
        gCinemaPlayerP->fading = 0;
        gCinemaPlayerP->fade = 0.0f;
        if (Cinema_IsActive() != 0) {
            Cinema_RunEnd(1);
            Cinema_Close();
            if (gCinemaPlayerP->owner != 0) {
                CCache__Deallocate(0, gCinemaPlayerP->owner);
            }
            gCinemaPlayerP->owner = 0;
            gCinemaPlayerP->resource = 0;
            gCinemaPlayerP->active = 0;
            gCinemaPlayerP->finished = 0;
            gCinemaPlayerP->fading = 0;
        }
        return;
    }
    Cinema_StartPending();
#ifdef TARGET_PORT
    /* The port: the GameState view reads its second word at N64 offset 0x1450 from
     * m_PlayerList.m_bCinema, which is gEngineApp_m_Settings.match.unk44 (0x801198A4 + 0x1450 =
     * 0x8011AAD8 + 0x1D8 + 0x44). On the host that offset lands elsewhere in CGame (round 5: in
     * the settings once CAutomap grew m_pHostRegion), the test failed and no cinema ever ended. */
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0 && gEngineApp_m_Game_m_PlayerList_m_bCinema.busy == 0 && gEngineApp_m_Settings.match.unk44 == 0) {
#else
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0 && gEngineApp_m_Game_m_PlayerList_m_bCinema.busy == 0 && gEngineApp_m_Game_m_PlayerList_m_bCinema.paused == 0) {
#endif
        if (gCinemaPlayerP->time >= gCinemaPlayerP->endTime && gCinemaPlayerP->ended == 0) {
            CinemaCallback cue;

            Sound__StopAllExcept(-1);
            if (gCinemaPlayerP->restoreAtEnd != 0) {
                Cinema_RestorePlayer();
            }
            ctx = gCinemaPlayerP;
            cue = ctx->onEnd;
            if (cue != 0) {
                ctx->onEnd = 0;
                cue();
            }
            gCinemaPlayerP->ended = 1;
        }
        if (gCinemaPlayerP->time >= gCinemaPlayerP->clipEnd && gCinemaPlayerP->closed == 0) {
            if (gCinemaPlayerP->owner != 0) {
                CCache__Deallocate(0, gCinemaPlayerP->owner);
            }
            {
                CinemaCallback end = gCinemaPlayerP->onClose;

                gCinemaPlayerP->owner = 0;
                gCinemaPlayerP->resource = 0;
                gCinemaPlayerP->active = 0;
                gCinemaPlayerP->finished = 0;
                gCinemaPlayerP->fading = 0;
                if (end != 0) {
                    gCinemaPlayerP->onClose = 0;
                    end();
                }
            }
            gCinemaPlayerP->closed = 1;
        }
    }
    Cinema_StartPending();
    if (gCinemaPlayerP->active != 0) {
        CCache__SetFrameDelay(0xA);
    }
}

s32 Cinema_RequestById();                                /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Cinema_RequestById receives Cinema_SetNextCallbacks's own argument
 * registers (pass-through); the host passes them. */
s32 Cinema_SetNextCallbacks(s32 arg0, void (*arg1)(Cinema *), void (*arg2)()) {
    s32 temp_s3;

    temp_s3 = Cinema_RequestById(arg0);
    if (gCinemaPlayerP->scriptId == arg0) {
        gCinemaPlayerP->nextOnClose = arg1;
        gCinemaPlayerP->nextOnEnd = arg2;
    } else {
        if (arg1 != NULL) {
            arg1(gCinemaPlayerP);
        }
        if (arg2 != NULL) {
            arg2();
        }
    }
    return temp_s3;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802456A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Cinema_Stop(void);

void Cinema_Stop(void) {
    if (Cinema_IsActive() != 0) {
        sptr temp_a1;
        Cinema *record;
        Cinema_RunEnd(1);
        Cinema_Close();
        temp_a1 = (sptr)gCinemaPlayerP->owner;
        if (temp_a1 != 0) {
            CCache__Deallocate(0, temp_a1);
        }
        record = gCinemaPlayerP;
        record->owner = 0;
        record->resource = 0;
        record->active = 0;
        record->finished = 0;
        record->fading = 0;
    }
}
s32 Cinema_IsSkippable(void);

void Cinema_TrySkip(void) {
    if ((Cinema_IsActive() != 0) && (Cinema_IsSkippable() != 0) && (gCinemaPlayerP->fading == 0)) {
        gCinemaPlayerP->fading = 1;
        gCinemaPlayerP->fade = 1.0f;
    }
}

s32 Cinema_IsSkippable(void) {
    return gCinemaPlayerP->scriptId == 0x50;
}

s32 Cinema_IsPending(void) {
    return ~gCinemaPlayerP->entryIndex != 0;
}

s32 Cinema_IsActive(void) {
    return gCinemaPlayerP->active;
}

/* The cinema state: a path playing between two times. */


s32 Cinema_PathPlaying(void) {
    f32 temp_f1;
    s32 var_a0;

    var_a0 = 0;
    if (gCinemaPlayerP->active != 0) {
        temp_f1 = gCinemaPlayerP->time;
        if ((gCinemaPlayerP->startTime <= temp_f1) && (temp_f1 <= gCinemaPlayerP->endTime)) {
            var_a0 = 1;
        }
    }
    return var_a0;
}
s32 Cinema_PathDone(void);

s32 Cinema_PathDone(void) {
    if (gCinemaPlayerP->active != 0) {
        if (gCinemaPlayerP->time > gCinemaPlayerP->endTime) {
            return 1;
        }
        return 0;
    }
    return 1;
}

#include "m2c_macros.h"

s32 Cinema_IsIdle(void) {
    s32 var_v0;

    var_v0 = 0;
    if (Cinema_IsActive() == 0) {
        var_v0 = Cinema_IsPending() == 0;
    }
    return var_v0;
}

s32 Cinema_IsFinished(void) {
    return gCinemaPlayerP->finished;
}

void Cinema_UpdateCamera();                               /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Cinema_UpdateCamera receives Cinema_UpdateCameraIfActive's own argument
 * registers (pass-through); the host passes them. */
void Cinema_UpdateCameraIfActive(void * pt0) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_UpdateCamera(pt0);
    }
}

s32 Cinema_DrawFade();                            /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Cinema_DrawFade receives Cinema_DrawOverlay's own argument
 * registers (pass-through); the host passes them. */
void Cinema_DrawOverlay(void * pt0) {
    if (gCinemaPlayerP->active != 0) {
        Cinema_DrawFade(pt0);
    }
}

void Cinema_Enable(void) {
    gCinemaEnabled = 1;
}

void Cinema_Disable(void) {
    gCinemaEnabled = 0;
}

s32 Cinema_IsEnabled(void) {
    return gCinemaEnabled;
}

void Cinema_ClearFinished(void) {
    gCinemaPlayerP->finished = 0;
}

s32 Cinema_FollowsPlayer(void)
{
  int new_var2;
  Cinema *new_var;
  if (Cinema_PathPlaying() == 0)
  {
    return 0;
  }
  new_var2 = 0;
  new_var = gCinemaPlayerP;
  return (new_var->spec.flags & 2) != new_var2;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* The playing cinema's second music override (0 when none plays). */
s32 Cinema_OverridesMusic2(void) {
    if (gCinemaPlayerP->active == 0) {
        return 0;
    }
    return gCinemaPlayerP->spec.unk44;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* The playing cinema's first music override (0 when none plays). */
s32 Cinema_OverridesMusic1(void) {
    if (gCinemaPlayerP->active == 0) {
        return 0;
    }
    return gCinemaPlayerP->spec.unk40;
}

extern s32 gCinemaHoldFade;

void Cinema_SetHoldFade(s32 arg0) {
    gCinemaHoldFade = arg0;
}

f32 Cinema_GetFOV(f32);

f32 Cinema_GetCameraFov(void) {
    f32 value;

    if (gCinemaPlayerP->active != 0) {
        value = Cinema_GetFOV(gCinemaPlayerP->time);
    } else {
        value = 47.5f;
    }
    return value;
}

f32 Cinema_GetFarClip(void) {
    return gCinemaPlayerP->spec.scale * 10.24f;
}

s32 Cinema_GetFogColor(void) {
    return gCinemaPlayerP->spec.unk48;
}

s32 Cinema_GetScriptId(void) {
    return gCinemaPlayerP->scriptId;
}

void Cinema_Reset(void)
{
  sptr temp_a1;
  temp_a1 = (sptr) gCinemaPlayerP->owner;
  if (temp_a1 != 0)
  {
    CCache__Deallocate(0, temp_a1);
  }
  gCinemaPlayerP->fading = (gCinemaPlayerP->finished = (gCinemaPlayerP->active = (sptr) (gCinemaPlayerP->resource = (gCinemaPlayerP->owner = 0))));
}

void Sound__StopAllExcept(s32);

/* De-faked in round 21 (j4): the draft cleared the callback in two identical arms under a test of
 * gCinemaPlayerP; reading the cinema into a local first gives the ROM order (found by the permuter). */

/* Ends the playing cinema once: stops the sounds, gives the player back if asked, runs and clears
 * its end callback. */
void Cinema_RunEnd(s32 restore) {
    Cinema *cinema;
    CinemaCallback onEnd;

    if (gCinemaPlayerP->ended == 0) {
        Sound__StopAllExcept(-1);
        if (gCinemaPlayerP->restoreAtEnd != 0 || restore != 0) {
            Cinema_RestorePlayer();
        }
        cinema = gCinemaPlayerP;
        onEnd = cinema->onEnd;
        if (onEnd != NULL) {
            cinema->onEnd = NULL;
            onEnd();
        }
        gCinemaPlayerP->ended = 1;
    }
}

void Cinema_Close(void) {
    if (gCinemaPlayerP->closed == 0) {
        {
            void *temp_a1 = gCinemaPlayerP->owner;
            if (temp_a1 != 0) {
                CCache__Deallocate(0, temp_a1);
            }
        }
        {
            void *record = gCinemaPlayerP;
            CinemaCallback fn = ((Cinema *)(record))->onClose;
            ((Cinema *)(record))->owner = 0;
            ((Cinema *)(record))->resource = 0;
            ((Cinema *)(record))->active = 0;
            ((Cinema *)(record))->finished = 0;
            ((Cinema *)(record))->fading = 0;
            if (fn != 0) {
                ((Cinema *)(record))->onClose = 0;
                fn();
            }
        }
        gCinemaPlayerP->closed = 1;
    }
}

/* Advances the clock at gCinemaPlayerP by one frame. */
void Cinema_AdvanceClock(void) {
    if (Cinema_IsActive()) {
        if (gEngineApp_m_Settings.paused != 0 || Cinema_IsFinished()) {
            gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
            return;
        }
        gCinemaPlayerP->prevTime = gCinemaPlayerP->time;
        gCinemaPlayerP->time += gFrameIncrement * (1.0f / 15);
        if (gCinemaPlayerP->time >= gCinemaPlayerP->startTime) {
            Cinema_Start();
        }
        if (gCinemaPlayerP->time >= gCinemaPlayerP->clipEnd) {
            gCinemaPlayerP->time = gCinemaPlayerP->clipEnd;
            if (gCinemaPlayerP->stopAtEnd) {
                gCinemaPlayerP->finished = 1;
                gCinemaPlayerP->stopAtEnd = 0;
            }
        }
    }
}

s32 Cinema_GetActive(void) {
    return gCinemaPlayerP->active;
}

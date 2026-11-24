#include "common.h"
#ifdef TARGET_PORT
#include "structs/game.h"
#include "structs/camera.h"
#endif
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/event.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "globals/text.h"
#include "globals/event.h"

extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, sptr arg3, f32 time);
void Sound__PlaySpeech(s32 arg0);
extern void Cinema_RequestById(s32 track);

void LevelEvent_Fire(LevelEvent *event) {
    LevelEventHandler *entry;
    u8 *game;
    void *camera;
    u8 *actor;

#ifdef TARGET_PORT
    {
        extern void T2Port_LogEvent(LevelEvent *event, const char *text);
        T2Port_LogEvent(event, event->message != 0 ? gLevelMessages[event->message].text : NULL);
    }
#endif
    entry = gLevelEventHandlers[event->kind];
    if (entry == NULL) {
        return;
    }
    game = &gEngineApp_m_Settings.multiplayer;
#ifdef TARGET_PORT /* the camera pool by name (host layout) */
    camera = &gEngineApp_m_Game.m_CameraPool->m_MainCamera;
    if (*game == 0) {
        camera = gEngineApp_m_Game.m_CameraPool->m_ActiveList.m_pHead;
        if (camera == NULL) {
            camera = &gEngineApp_m_Game.m_CameraPool->m_MainCamera;
        }
    }
#else
    camera = game - 0x1201;                         /* gEngineApp_m_Game_m_CameraPool_m_MainCamera: fallback camera */
    if (*game == 0) {
        camera = *(void **)(game - 0x1221);         /* gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead: first viewer */
        if (camera == NULL) {
            camera = game - 0x1201;
        }
    }
#endif
    if (gScene.m_Mode == 4) {
        if (event->message != 0) {
            CCameraPool__AddMultiLineText(&gEngineApp_m_Game_m_CameraPool, camera, gLevelMessages[event->message].text, gFrameCountGameplay, 4.0f);
            if (gLevelMessages[event->message].speech != 0) {
                Sound__PlaySpeech(gLevelMessages[event->message].speech);
            }
        }
        if (event->music >= 2) {
            Cinema_RequestById(event->music);
        }
    }
    entry += event->handler;
    if (event->level == gScene.m_nLevel) {
        actor = (u8 *)gScene.m_pActors + event->actor * (s32)sizeof(CGameObjectInstance);
        if (entry->onActor != NULL) {
            entry->onActor(actor, actor + T2_OFFSETOF(CGameObjectInstance, m_AI), event);
        }
    } else if (entry->onEvent != NULL) {
        entry->onEvent(event);
    }
}

#include "protos/scene.h"
#include "protos/game.h"

struct Trigger;
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_15.c, func_80278E7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void LevelEvent_ProcessTriggers(Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, sptr arg);

void GameObject_FireTriggers(CGameObjectInstance *arg0, s32 arg1, sptr arg2) {
    sptr sp20;
    s32 sp24;

    if (!(arg0->m_dwFlags & 0x80000)) {
        CScene__GetInstanceEventRecords(&gScene, arg0, &sp20, &sp24);
        if (sp24 != 0) {
            LevelEvent_ProcessTriggers((struct Trigger *) sp20, sp24, 0, arg1 & 0x3FC1FF, arg1 & 0x400000, 0x400000, arg2);
        }
    }
}

void CGameRegion__FireTriggers(sptr arg0, s32 arg1, sptr arg2) {
    sptr sp20;
    s32 sp24;

    CScene__GetRegionEventRecords(&gScene, arg0, &sp20, &sp24);
    if (sp24 != 0) {
        LevelEvent_ProcessTriggers((struct Trigger *) sp20, sp24, 0, arg1 & 0x7FF, arg1 & 0x3800, 0x3800, arg2);
    }
}

#include "structs/world.h"

void LevelEvent_Trigger(void *arg0) {
    func_80278EEC_S1 *object = arg0;
    object->unkE |= 0x10;
    CScene__SetEventFired(&gScene, arg0, 1);
    if ((gScene.m_Mode == 4) && (object->unkE & 2)) {
        CScene__QueueDelayedEvent(&gScene, arg0);
        return;
    }
    LevelEvent_Fire(arg0);
}

void LevelEvent_MarkFired(Record14 *arg0) {
    Record14 copy;
    u8 *bytes = (u8 *)arg0;
    bytes[0xE] |= 0x20;
    if (bytes[0x11] == 1) {
        copy = *arg0;
        ((u8 *)&copy)[0x12] = 2;
        ((u8 *)&copy)[0x10] = 0;
        LevelEvent_Fire((LevelEvent *) &copy);
    }
}

#include "m2c_macros.h"

void Event_Hide(CGameObjectInstance *arg0) {
    arg0->m_dwFlags &= ~0x2000;
    arg0->m_dwFlags &= ~0x100;
}

void CGameObjectInstance__AdvanceAnimation();                               /* extern */

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__AdvanceAnimation receives Event_Show's own argument
 * registers (pass-through); the host passes them. */
void Event_Show(CGameObjectInstance *arg0) {
    CGameObjectInstance__AdvanceAnimation(arg0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}

void Cinema_RequestForObject(void *owner, LevelEvent *obj);

void Event_PlayCinema(sptr arg0, sptr arg1, sptr arg2) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject((void *) arg0, (LevelEvent *) arg2);
    }
}

void Event_PlayCinemaRemote(sptr arg0) {
    if (gScene.m_Mode == 4) {
        Cinema_RequestForObject(NULL, (LevelEvent *) arg0);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 sound;
} SoundEvent;
void Music_SetTrack(s32 arg0);

void Event_PlaySoundOrMusic(s32 arg0, s32 arg1, SoundEvent *arg2) {
    if ((u16) arg2->sound >= 0x100U) {
        Sound__PlaySpeech(arg2->sound);
        return;
    }
    Music_SetTrack((s32) arg2->sound);
}

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */

s32 Sound__Play2D(s32 arg0);

void Event_PlaySound(s32 arg0, s32 arg1, LevelEvent *arg2) {
    Sound__Play2D((s16) arg2->param);
}

/* The mode parameter this AI callback is handed: only the field it reads. */

void Event_SetMusicRemote(LevelEvent *arg0) {
    Music_SetTrack(arg0->param);
}

void Event_PlaySoundRemote(LevelEvent *arg0) {
    Sound__Play2D(arg0->sound);
}

#include "structs/sound.h"

void Sound__SetAmbientTrack(s32 arg0);

void Event_SetAmbient(s32 arg0, s32 arg1, SoundEffectInfo *arg2) {
    Sound__SetAmbientTrack(arg2->flags);
}

void Event_SetAmbientRemote(LevelEvent *arg0) {
    Sound__SetAmbientTrack(arg0->param);
}

void Event_DisableCollision(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}

void Event_EnableCollision(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}
s32 LevelEvent_CheckRequiredItem(LevelEvent *arg0, sptr arg1);

void LevelEvent_ProcessTriggers(Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, sptr arg) {
    s32 i;
    s32 active;
    s32 usable;
    CScene *world;
    Trigger *trigger;
    for (trigger = triggers, i = 0; i < count; trigger++, i++) {
        world = &gScene;
        usable = 1;
        if (world->m_Mode != 4) {
            usable = CScene__IsEventFired(world, trigger) == 0;
        }
        if (!usable) {
            continue;
        }
        active = 1;
        if (!(trigger->flags.bytes.state & 0x20) && enter != 0 && (trigger->mask & enter)) {
            trigger->flags.bytes.state |= 0x40;
            if (trigger->flags.bytes.id != 0) {
                if (CScene__AreEventsDone(world, trigger->flags.bytes.id) != 0) {
                    CScene__FireEventsByKeyOnce(world, trigger->flags.bytes.id);
                }
            } else {
                LevelEvent_MarkFired((Record14 *) trigger);
            }
            active = 1;
        }
        if (require != 0) {
            active = 0;
            if (trigger->mask & require) {
                active = 1;
            }
        }
        if (enter != 0 && !(trigger->mask & enter)) {
            active = 0;
        }
        if ((trigger->mask & group) && !(trigger->mask & group & exclude)) {
            active = 0;
        }
#ifdef TARGET_PORT /* the big-endian word's 0x1100 bits are the state byte's 0x11 (byte 0xE) */
        if ((trigger->flags.bytes.state & 0x11) == 0x10) {
#else
        if ((trigger->flags.word & 0x1100) == 0x1000) {
#endif
            active = 0;
        }
        if (LevelEvent_CheckRequiredItem((LevelEvent *) trigger, arg) == 0) {
            active = 0;
        }
        if (!active) {
            continue;
        }
        if (trigger->flags.bytes.id != 0) {
            if (!(trigger->flags.bytes.state & 0x80)) {
                CScene__SetEventFired(world, trigger, 1);
                trigger->flags.bytes.state |= 0x80;
                CScene__DecLinkGroupCount(world, trigger->flags.bytes.id);
                if (CScene__IsLinkGroupDone(world, trigger->flags.bytes.id) != 0) {
                    CScene__FireEventsByKey(world, trigger->flags.bytes.id);
                }
            }
        } else {
            LevelEvent_Trigger(trigger);
        }
    }
}

s32 Action_CheckRequiredItem(CGameObjectInstance *, CAI *, sptr); /* extern */

s32 LevelEvent_CheckRequiredItem(LevelEvent *arg0, sptr arg1) {
    CGameObjectInstance *temp_a1;

    if (arg0->level == gScene.m_nLevel) {
        if (arg1 != 0) {
            temp_a1 = &gScene.m_pActors[arg0->actor];
            if ((arg0->kind == 0xA) && (arg0->handler == 1)) {
                return Action_CheckRequiredItem(temp_a1, &temp_a1->m_AI, arg1);
            }
        }
        return 1;
    }
    return 1;
}

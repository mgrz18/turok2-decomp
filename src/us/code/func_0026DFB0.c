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

extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, s32 arg3, f32 time);
extern void Sound__PlaySpeech(s32 sound);
extern void Cinema_RequestById(s32 track);

void LevelEvent_Fire(LevelEvent *event) {
    LevelEventHandler *entry;
    u8 *game;
    void *camera;
    u8 *actor;

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

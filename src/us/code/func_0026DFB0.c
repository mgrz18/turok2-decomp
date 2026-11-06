#include "common.h"
#include "structs/event.h"
#include "globals/game.h"
#include "globals/camera.h"

typedef struct {
    void (*onActor)(void *actor, void *ai, LevelEvent *event);
    void (*onEvent)(LevelEvent *event);
} EventHandler;

extern EventHandler *gLevelEventHandlers[];  /* handler tables by kind */
extern char *gLevelMessages[];          /* {text, sound} pairs */
extern s32 D_800B69DC[];            /* gLevelMessages + 4 */
extern s32 gFrameCountGameplay;
extern s32 gScene;              /* world: game state */
extern u8 *gScene_m_pActors;              /* world + 0xE0: level actors (0x288 bytes each) */
extern s32 gScene_m_nLevel;              /* current level */

extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, s32 arg3, f32 time);
extern void Sound__PlaySpeech(s32 sound);
extern void Cinema_RequestById(s32 track);

void LevelEvent_Fire(LevelEvent *event) {
    EventHandler *entry;
    u8 *game;
    void *camera;
    u8 *actor;

    entry = gLevelEventHandlers[event->kind];
    if (entry == NULL) {
        return;
    }
    game = &gEngineApp_m_Settings.multiplayer;
    camera = game - 0x1201;                         /* gEngineApp_m_Game_m_CameraPool_m_MainCamera: fallback camera */
    if (*game == 0) {
        camera = *(void **)(game - 0x1221);         /* gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead: first viewer */
        if (camera == NULL) {
            camera = game - 0x1201;
        }
    }
    if (gScene == 4) {
        if (event->message != 0) {
            CCameraPool__AddMultiLineText(&gEngineApp_m_Game_m_CameraPool, camera, gLevelMessages[event->message * 2], gFrameCountGameplay, 4.0f);
            if (D_800B69DC[event->message * 2] != 0) {
                Sound__PlaySpeech(D_800B69DC[event->message * 2]);
            }
        }
        if (event->music >= 2) {
            Cinema_RequestById(event->music);
        }
    }
    entry += event->handler;
    if (event->level == gScene_m_nLevel) {
        actor = gScene_m_pActors + event->actor * 0x288;
        if (entry->onActor != NULL) {
            entry->onActor(actor, actor + 0x140, event);
        }
    } else if (entry->onEvent != NULL) {
        entry->onEvent(event);
    }
}

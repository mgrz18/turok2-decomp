#include "common.h"
#include "structs/event.h"

typedef struct {
    void (*onActor)(void *actor, void *ai, LevelEvent *event);
    void (*onEvent)(LevelEvent *event);
} EventHandler;

extern EventHandler *D_800B2628[];  /* handler tables by kind */
extern char *D_800B69D8[];          /* {text, sound} pairs */
extern s32 D_800B69DC[];            /* D_800B69D8 + 4 */
extern s32 D_800B6D20;
extern s32 D_800F7078;              /* world: game state */
extern u8 *D_800F7158;              /* world + 0xE0: level actors (0x288 bytes each) */
extern s32 D_80110020;              /* current level */
extern u8 D_801198B0[];             /* game */
extern u8 D_8011AAF1;               /* game + 0x1241: multiplayer flag */

extern void CCameraPool__AddMultiLineText(void *game, void *camera, char *text, s32 arg3, f32 time);
extern void Sound__PlaySpeech(s32 sound);
extern void Cinema_RequestById(s32 track);

void LevelEvent_Fire(LevelEvent *event) {
    EventHandler *entry;
    u8 *game;
    void *camera;
    u8 *actor;

    entry = D_800B2628[event->kind];
    if (entry == NULL) {
        return;
    }
    game = &D_8011AAF1;
    camera = game - 0x1201;                         /* D_801198F0: fallback camera */
    if (*game == 0) {
        camera = *(void **)(game - 0x1221);         /* D_801198D0: first viewer */
        if (camera == NULL) {
            camera = game - 0x1201;
        }
    }
    if (D_800F7078 == 4) {
        if (event->message != 0) {
            CCameraPool__AddMultiLineText(D_801198B0, camera, D_800B69D8[event->message * 2], D_800B6D20, 4.0f);
            if (D_800B69DC[event->message * 2] != 0) {
                Sound__PlaySpeech(D_800B69DC[event->message * 2]);
            }
        }
        if (event->music >= 2) {
            Cinema_RequestById(event->music);
        }
    }
    entry += event->handler;
    if (event->level == D_80110020) {
        actor = D_800F7158 + event->actor * 0x288;
        if (entry->onActor != NULL) {
            entry->onActor(actor, actor + 0x140, event);
        }
    } else if (entry->onEvent != NULL) {
        entry->onEvent(event);
    }
}

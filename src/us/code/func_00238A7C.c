#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


typedef struct SoundPair {
    s32 sound;          /* 0x0 */
    s32 effect;         /* 0x4 */
} SoundPair;

typedef struct EventParams {
    s32 index;          /* 0x0 */
    s32 unk4;           /* 0x4 */
} EventParams;

extern u8 gEngineApp_m_Game_m_CameraPool[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern SoundPair gLevelMessages[];
extern void CCameraPool__AddText(void *arg0, void *arg1, s32 arg2);
extern void Sound__PlaySpeech(s32 arg0);

void ImpactEvent_PlaySoundAndEffect(s32 arg0, CGameObjectInstance *actor, s32 arg2, CVector3 pos, EventParams params) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        CCameraPool__AddText(gEngineApp_m_Game_m_CameraPool, ((CPlayer *)actor->m_AI.m_pParent)->m_pCamera, gLevelMessages[params.index].sound);
    } else {
        CCameraPool__AddText(gEngineApp_m_Game_m_CameraPool, gEngineApp_m_Game_m_CameraPool + 0x40, gLevelMessages[params.index].sound);
    }
    if (gLevelMessages[params.index].effect != 0) {
        Sound__PlaySpeech(gLevelMessages[params.index].effect);
    }
}

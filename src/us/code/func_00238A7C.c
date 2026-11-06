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

extern u8 D_801198B0[];
extern SoundPair D_800B69D8[];
extern void CCameraPool__AddText(void *arg0, void *arg1, s32 arg2);
extern void Sound__PlaySpeech(s32 arg0);

void ImpactEvent_PlaySoundAndEffect(s32 arg0, CGameObjectInstance *actor, s32 arg2, CVector3 pos, EventParams params) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        CCameraPool__AddText(D_801198B0, ((CPlayer *)actor->m_AI.m_pParent)->m_pCamera, D_800B69D8[params.index].sound);
    } else {
        CCameraPool__AddText(D_801198B0, D_801198B0 + 0x40, D_800B69D8[params.index].sound);
    }
    if (D_800B69D8[params.index].effect != 0) {
        Sound__PlaySpeech(D_800B69D8[params.index].effect);
    }
}

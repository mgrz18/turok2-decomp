#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

s32 CGameObjectInstance__AdvanceAnimation(sptr);

/* The mode parameter this AI callback is handed: only the field it reads. */
typedef struct AIModeParam {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 value;
} AIModeParam;


void InteractiveAnim_EventPlay(CGameObjectInstance *arg0, sptr arg1, AIModeParam *arg2) {
    AI_PlayAnim(arg0, arg1, (s32) arg2->value);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    CGameObjectInstance__AdvanceAnimation(arg0);
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
}

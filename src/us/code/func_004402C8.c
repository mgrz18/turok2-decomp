#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

s32 AI_StartSound(s32, s32, s32);
s32 Sound__Play(s32, s32, s32, s32, s32, s32);

/* The (s8 *) casts keep these stores char-based, as the original code had
 * them; plain member stores schedule differently. */
void Primagen_EnterMode2_2(CGameObjectInstance *arg0, s32 arg1) {
    *(f32 *) (s8 *) &arg0->m_AI.m_AnimSpeed = 1.0f;
    *(s8 *) &gLevelScript.unk3CA = 0;
    *(s32 *) ((s8 *) &gLevelScript.unk3CA + 0x1A) = 0;
    *(s32 *) ((s8 *) &arg0->m_dwFlags) |= 0x2000;
    Sound__Play(0x2779, *(s32 *) &arg0->header.m_vPos.x, *(s32 *) &arg0->header.m_vPos.y, *(s32 *) &arg0->header.m_vPos.z, 0, -1);
    AI_StartSound((s32) arg0, arg1, 0x3B21);
}

#include "common.h"
#include "structs/game_object.h"

s32 AI_StartSound(s32, s32, s32);
s32 Sound__Play(s32, s32, s32, s32, s32, s32);

extern f32 D_800C0508;
extern s32 D_8012F9D2;          /* a byte flag, and a word at +0x1A */

/* The (s8 *) casts keep these stores char-based, as the original code had
 * them; plain member stores schedule differently. */
void Primagen_EnterMode2_2(CGameObjectInstance *arg0, s32 arg1) {
    *(f32 *) (s8 *) &arg0->m_AI.m_AnimSpeed = (f32) D_800C0508;
    *(s8 *) &D_8012F9D2 = 0;
    *(s32 *) ((s8 *) &D_8012F9D2 + 0x1A) = 0;
    *(s32 *) ((s8 *) &arg0->m_dwFlags) |= 0x2000;
    Sound__Play(0x2779, *(s32 *) &arg0->header.m_vPos.x, *(s32 *) &arg0->header.m_vPos.y, *(s32 *) &arg0->header.m_vPos.z, 0, -1);
    AI_StartSound((s32) arg0, arg1, 0x3B21);
}

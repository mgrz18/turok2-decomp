#include "common.h"
#include "structs/game_object.h"

s32 AI_StartSound(s32, s32, s32);
s32 Sound__Play(s32, s32, sptr, s32, s32, s32);

/* Field access by byte offset: a struct member reference here would let the
 * scheduler reorder these stores around the global ones. */
#define OBJ_FIELD(obj, type, field) (*(type *)((s8 *)(obj) + (s32) &((CGameObjectInstance *)0)->field))
extern s8 gLevelScript_unk3CA[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode2(void *arg0, sptr arg1) {
    OBJ_FIELD(arg0, f32, m_AI.m_AnimSpeed) = 1.0f;
    gLevelScript_unk3CA[0] = 0;
    *(s32 *)&gLevelScript_unk3CA[0x1A] = 0;
    OBJ_FIELD(arg0, s32, m_dwFlags) |= 0x2000;
    Sound__Play(0x2779, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.x, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.y, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.z, 0, -1);
    AI_StartSound((sptr) arg0, arg1, 0x3B21);
}

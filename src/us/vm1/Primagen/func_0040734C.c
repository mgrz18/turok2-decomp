#include "common.h"
#include "structs/game_object.h"

/* Two flags at gLevelScript_armed; the word 8 bytes before it is gLevelScript_bossState. */
typedef struct {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ s8 unk1;
} BossFlags;

extern BossFlags gLevelScript_armed_v0040734C __asm__("gLevelScript_armed"); /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode5(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
    gLevelScript_armed_v0040734C.unk0 = 0;
    gLevelScript_armed_v0040734C.unk1 = 0;
    ((s32 *) &gLevelScript_armed_v0040734C)[-2] = 1;
}

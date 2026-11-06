#include "common.h"
#include "structs/game_object.h"

/* gLevelScript_armed: two flags; gLevelScript_bossState (8 bytes before it) is set through it too. */
typedef struct {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ s8 unk1;
} Flags9E0;

extern Flags9E0 gLevelScript_armed; /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode5_2(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags |= 0x2000;
    gLevelScript_armed.unk0 = 0;
    gLevelScript_armed.unk1 = 0;
    ((s32 *) &gLevelScript_armed)[-2] = 1;
}

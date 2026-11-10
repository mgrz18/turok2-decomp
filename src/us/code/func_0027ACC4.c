#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 y;
    /* 0x18 */ u8 pad18[0x38 - 0x18];
    /* 0x38 */ s32 index;
} Cursor;

#ifdef TARGET_PORT /* COverlay's members (structs/player.h): its m_pScript is pointer-wide, so the
                    * N64 offsets of the view put the position in m_Mode/m_Time (no HUD drew) */
#include "structs/player.h"
void COverlay__Construct(COverlay *arg0, s16 arg1, s16 arg2) {
    arg0->m_Value = -1;
    arg0->m_PositionX = arg1;
    arg0->m_bOnScreen = 0;
    arg0->m_Mode = 0;
    arg0->m_PositionY = arg2;
}
#else
void COverlay__Construct(Cursor *arg0, s16 arg1, s16 arg2) {
    arg0->index = -1;
    arg0->x = arg1;
    arg0->unk0 = 0;
    arg0->unk8 = 0;
    arg0->y = arg2;
}
#endif

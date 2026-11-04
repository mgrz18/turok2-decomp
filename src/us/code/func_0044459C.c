#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 state;
} ModeData;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ ModeData *data;
} CAIModeInfo;

void func_0044459C(s32 arg0, CAI *ai) {
    ((CAIModeInfo *) ai->m_pModeCurrent)->data->state = 7;
    if (ai->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) ai, 0xB);
    }
}

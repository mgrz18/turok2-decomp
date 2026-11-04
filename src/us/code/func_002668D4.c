#include "common.h"
#include "structs/game_object.h"

s32 func_0021A7EC(s32);
s32 func_00220260(s32, s32, s32);
s32 func_002666B0(s32, s32, s32);

/* The mode parameter this AI callback is handed: only the field it reads. */
typedef struct AIModeParam {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 value;
} AIModeParam;

extern s32 D_800F7078;

void func_002668D4(CGameObjectInstance *arg0, s32 arg1, AIModeParam *arg2) {
    func_002666B0(arg0, arg1, (s32) arg2->value);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    func_0021A7EC(arg0);
    func_00220260((s32) &D_800F7078, arg0, 0);
}

#include "common.h"

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
} Arg0;

s32 func_00226160(s32 *, u8, s16);          /* extern */
extern s32 D_800F7078;

void CPathTrack__GetCurrentPoint(Arg0 *arg0) {
    func_00226160(&D_800F7078, arg0->unk1, arg0->unk2);
}

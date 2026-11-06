#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u16 unkA;
} TriggerData;

s32 CScene__SetInstanceSavedBitInLevel(s32 *, u16, u16, s32); /* extern */

void Generator_EventOnRemote(TriggerData *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->unkA, arg0->unk4, 1);
}

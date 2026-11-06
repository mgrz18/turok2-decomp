#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

s32 CCache__Deallocate(s32, s32);

s32 CPlayerPool__GetFirstActive(CPlayerPool *);                       /* extern */
s32 Sound__StopFlags(s32);                     /* extern */
s32 Sound__DetachAll();                            /* extern */
s32 LoopSounds_StopAll(s32);                         /* extern */
s32 LoopSounds_Get();                                /* extern */
s32 CPlayer__StopLoopingSounds_2(s32);                         /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ s32 handles[14];  /* resources released on exit */
} Unk004658FC;

void CScene__UnloadLevel_2(Unk004658FC *arg0) {
    s32 temp_v0;

    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (temp_v0 != 0) {
        CPlayer__StopLoopingSounds_2(temp_v0);
    }
    LoopSounds_StopAll(LoopSounds_Get());
    Sound__DetachAll();
    Sound__StopFlags(0x1000);
    CCache__Deallocate(0, arg0->handles[0]);
    CCache__Deallocate(0, arg0->handles[1]);
    CCache__Deallocate(0, arg0->handles[2]);
    CCache__Deallocate(0, arg0->handles[3]);
    CCache__Deallocate(0, arg0->handles[4]);
    CCache__Deallocate(0, arg0->handles[6]);
    CCache__Deallocate(0, arg0->handles[7]);
    CCache__Deallocate(0, arg0->handles[13]);
    CCache__Deallocate(0, arg0->handles[5]);
    CCache__Deallocate(0, arg0->handles[12]);
}

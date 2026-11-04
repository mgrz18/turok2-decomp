#include "common.h"
#include "structs/game.h"

s32 func_002052D8(s32, s32);

s32 func_002532A8(CPlayerList *);                       /* extern */
s32 func_002758DC(s32);                     /* extern */
s32 func_00275B58();                            /* extern */
s32 func_00275EFC(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */
s32 func_0042EA24(s32);                         /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ s32 handles[14];  /* resources released on exit */
} Unk004658FC;

void func_004658FC(Unk004658FC *arg0) {
    s32 temp_v0;

    temp_v0 = func_002532A8(&D_80119870.m_PlayerList);
    if (temp_v0 != 0) {
        func_0042EA24(temp_v0);
    }
    func_00275EFC(func_00275F7C());
    func_00275B58();
    func_002758DC(0x1000);
    func_002052D8(0, arg0->handles[0]);
    func_002052D8(0, arg0->handles[1]);
    func_002052D8(0, arg0->handles[2]);
    func_002052D8(0, arg0->handles[3]);
    func_002052D8(0, arg0->handles[4]);
    func_002052D8(0, arg0->handles[6]);
    func_002052D8(0, arg0->handles[7]);
    func_002052D8(0, arg0->handles[13]);
    func_002052D8(0, arg0->handles[5]);
    func_002052D8(0, arg0->handles[12]);
}

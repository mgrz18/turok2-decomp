#include "common.h"
#include "structs/player.h"

/* The record at D_80119870 + 0x1268: flags at 4. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 flags;
} PlayerState1268;

s32 func_00243414(s32, s32, s32);
s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(s32 *);                     /* extern */
extern s32 D_80119870;
extern s32 D_8012FB70;

void func_004457DC(void) {
    CPlayer *temp_v0;
    PlayerState1268 *temp_v1;

    func_00243414(D_8012FB70, D_8012FB70 + 0x140, 1);
    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != NULL) {
        temp_v1 = (PlayerState1268 *) ((s8 *) &D_80119870 + 0x1268);
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->flags |= 4;
        func_00248BA8((s32) temp_v0, 0x1B57, 2, 0);
    }
}

#include "common.h"

/* A frame task: a buffer at 0, its size at 4, a message block at 8, and the result range at 0x48 / 0x4C. */
typedef struct {
    /* 0x00 */ s32 buffer;
    /* 0x04 */ u16 size;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u8 msg[0x48 - 8];
    /* 0x48 */ s32 start;
    /* 0x4C */ s32 length;
} FrameTask;

s32 func_002949A4(s32, s32, s32, s32);
s32 func_002985C0(s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 func_0029BF80(s32);
void osWritebackDCacheAll(void);

s32 func_0026FEA0();                            /* extern */
s32 func_0027592C();                            /* extern */
s32 func_00275B7C();                            /* extern */
s32 func_00275BA0();                            /* extern */
s32 func_0028B3E8(s32 *, void *);           /* extern */
extern s32 D_800B2668;
extern s32 D_800F27C0;
extern s32 D_800F2820;
extern u16 D_800F283A;
extern s32 D_8011AD90;

s32 func_0026FD88(FrameTask *arg0) {
    s32 sp10;
    s32 sp14;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    func_0026FEA0();
    temp_s0 = func_0029BF80(arg0->buffer);
    arg0->size = D_800F283A;
    func_00275B7C();
    temp_s0_2 = func_002949A4(*(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + (D_800B2668 * 4)))), (s32) &sp10, temp_s0, (s32) (s16) arg0->size);
    func_00275BA0();
    if (sp10 == 0) {
        return 0;
    }
    temp_v0 = D_800B2668 * 4;
    arg0->start = (s32) *(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + temp_v0)));
    arg0->length = (s32) (((s32) (temp_s0_2 - *(((__typeof__(&D_800F27C0))((s8 *)&D_800F27C0 + temp_v0)))) >> 3) * 8);
    osWritebackDCacheAll();
    func_0028B3E8(&D_8011AD90, arg0->msg);
    D_800B2668 ^= 1;
    func_0027592C();
    osRecvMesg((s32) &D_800F2820, (s32) &sp14, 1);
    func_002985C0(arg0->buffer, (s16) arg0->size * 4);
    return 1;
}

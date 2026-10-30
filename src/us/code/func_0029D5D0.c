/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/osCartRomInit.c, osCartRomInit, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

typedef struct PiHandle {
    /* 0x00 */ struct PiHandle *next;
    /* 0x04 */ u8 type;
    /* 0x05 */ u8 latency;
    /* 0x06 */ u8 pageSize;
    /* 0x07 */ u8 relDuration;
    /* 0x08 */ u8 pulse;
    /* 0x09 */ u8 domain;
    /* 0x0C */ u32 baseAddress;
    /* 0x10 */ u32 speed;
    /* 0x14 */ u8 transferInfo[0x60];
} PiHandle;

extern PiHandle D_80124D30;     /* CartRomHandle */
extern PiHandle *D_800B897C;    /* __osPiTable */
extern s32 func_0029B410(u32 devAddr, u32 *data);   /* osPiRawReadIo */
extern void func_00266C5C(void *p, s32 c, s32 n);   /* memset */
extern u32 func_0029DFF0(void);                     /* __osDisableInt */
extern void func_0029E010(u32 mask);                /* __osRestoreInt */

PiHandle *func_0029D5D0(void) {
    u32 piConfig;
    u32 saveMask;

    piConfig = 0;

    if (D_80124D30.baseAddress != 0xB0000000) {
        D_80124D30.type = 0;
        D_80124D30.baseAddress = 0xB0000000;
        func_0029B410(0, &piConfig);
        D_80124D30.latency = piConfig & 0xFF;
        D_80124D30.pulse = (piConfig >> 8) & 0xFF;
        D_80124D30.pageSize = (piConfig >> 16) & 0xF;
        D_80124D30.relDuration = (piConfig >> 20) & 0xF;
        D_80124D30.domain = 0;
        D_80124D30.speed = 0;
        func_00266C5C(&D_80124D30.transferInfo, 0, 0x60);
        saveMask = func_0029DFF0();
        D_80124D30.next = D_800B897C;
        D_800B897C = &D_80124D30;
        func_0029E010(saveMask);
    }

    return &D_80124D30;
}

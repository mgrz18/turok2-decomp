/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/osCartRomInit.c, osCartRomInit, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"
#include "globals/libultra.h"

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

extern PiHandle CartRomHandle;     /* CartRomHandle */
extern s32 osPiRawReadIo(u32 devAddr, u32 *data);   /* osPiRawReadIo */
extern void memset(void *p, s32 c, s32 n);   /* memset */
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);

PiHandle *osCartRomInit(void) {
    u32 piConfig;
    u32 saveMask;

    piConfig = 0;

    if (CartRomHandle.baseAddress != 0xB0000000) {
        CartRomHandle.type = 0;
        CartRomHandle.baseAddress = 0xB0000000;
        osPiRawReadIo(0, &piConfig);
        CartRomHandle.latency = piConfig & 0xFF;
        CartRomHandle.pulse = (piConfig >> 8) & 0xFF;
        CartRomHandle.pageSize = (piConfig >> 16) & 0xF;
        CartRomHandle.relDuration = (piConfig >> 20) & 0xF;
        CartRomHandle.domain = 0;
        CartRomHandle.speed = 0;
        memset(&CartRomHandle.transferInfo, 0, 0x60);
        saveMask = __osDisableInt();
        CartRomHandle.next = __osPiTable;
        __osPiTable = &CartRomHandle;
        __osRestoreInt(saveMask);
    }

    return &CartRomHandle;
}

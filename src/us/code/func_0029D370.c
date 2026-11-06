/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* libultra osLeoDiskInit: registers the 64DD drive (PI domain 2) as a PI
 * device and returns its handle (LeoDiskHandle, LeoDiskHandle). */
#include "common.h"
#include "globals/libultra.h"

typedef struct OSPiHandle {
    /* 0x00 */ struct OSPiHandle *next;
    /* 0x04 */ u8 type;
    /* 0x05 */ u8 latency;
    /* 0x06 */ u8 pageSize;
    /* 0x07 */ u8 relDuration;
    /* 0x08 */ u8 pulse;
    /* 0x09 */ u8 domain;
    /* 0x0C */ u32 baseAddress;
    /* 0x10 */ u32 speed;
    /* 0x14 */ u8 transferInfo[0x60]; /* __OSTranxInfo */
} OSPiHandle;

#define IO_WRITE(addr, data) (*(volatile u32 *) (addr) = (u32) (data))
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define DEVICE_TYPE_64DD 2
#define PI_DOMAIN2 1

extern OSPiHandle LeoDiskHandle;   /* LeoDiskHandle */
extern OSPiHandle *__osDiskHandle;  /* __osDiskHandle */

extern void memset(void *p, s32 c, u32 n); /* memset */
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);

OSPiHandle *osLeoDiskInit(void) {
    u32 saveMask;

    LeoDiskHandle.type = DEVICE_TYPE_64DD;
    LeoDiskHandle.baseAddress = 0xA5000000; /* PHYS_TO_K1(LEO_BASE_REG) */
    LeoDiskHandle.latency = 3;
    LeoDiskHandle.pulse = 6;
    LeoDiskHandle.pageSize = 6;
    LeoDiskHandle.relDuration = 2;
    LeoDiskHandle.domain = PI_DOMAIN2;
    IO_WRITE(PI_BSD_DOM2_LAT_REG, LeoDiskHandle.latency);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, LeoDiskHandle.pulse);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, LeoDiskHandle.pageSize);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, LeoDiskHandle.relDuration);
    LeoDiskHandle.speed = 0;
    memset(&LeoDiskHandle.transferInfo, 0, sizeof(LeoDiskHandle.transferInfo));
    saveMask = __osDisableInt();
    LeoDiskHandle.next = __osPiTable;
    __osPiTable = &LeoDiskHandle;
    __osDiskHandle = &LeoDiskHandle;
    __osRestoreInt(saveMask);
    return &LeoDiskHandle;
}

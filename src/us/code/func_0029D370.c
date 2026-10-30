/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* libultra osLeoDiskInit: registers the 64DD drive (PI domain 2) as a PI
 * device and returns its handle (LeoDiskHandle, D_80123A60). */
#include "common.h"

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

extern OSPiHandle D_80123A60;   /* LeoDiskHandle */
extern OSPiHandle *D_80123AD4;  /* __osDiskHandle */
extern OSPiHandle *D_800B897C;  /* __osPiTable */

extern void func_00266C5C(void *p, s32 c, u32 n); /* memset */
extern u32 func_0029DFF0(void);                    /* __osDisableInt */
extern void func_0029E010(u32 mask);               /* __osRestoreInt */

OSPiHandle *func_0029D370(void) {
    u32 saveMask;

    D_80123A60.type = DEVICE_TYPE_64DD;
    D_80123A60.baseAddress = 0xA5000000; /* PHYS_TO_K1(LEO_BASE_REG) */
    D_80123A60.latency = 3;
    D_80123A60.pulse = 6;
    D_80123A60.pageSize = 6;
    D_80123A60.relDuration = 2;
    D_80123A60.domain = PI_DOMAIN2;
    IO_WRITE(PI_BSD_DOM2_LAT_REG, D_80123A60.latency);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, D_80123A60.pulse);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, D_80123A60.pageSize);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, D_80123A60.relDuration);
    D_80123A60.speed = 0;
    func_00266C5C(&D_80123A60.transferInfo, 0, sizeof(D_80123A60.transferInfo));
    saveMask = func_0029DFF0();
    D_80123A60.next = D_800B897C;
    D_800B897C = &D_80123A60;
    D_80123AD4 = &D_80123A60;
    func_0029E010(saveMask);
    return &D_80123A60;
}

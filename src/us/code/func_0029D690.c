/* Boundary fixed in versions/cuts.us.txt: splat's range holds 2 trailing junk words (0x29DA28: mul.s/mfc1, never executed,
 * also in Rage Wars before osEPiRawStartDma); the function itself is 0x398 bytes.
 * After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/__osDevMgrMain.c, __osDevMgrMain, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

typedef struct {
    /* 0x00 */ u32 errStatus;
    /* 0x04 */ void *dramAddr;
    /* 0x08 */ void *C2Addr;
    /* 0x0C */ u32 sectorSize;
    /* 0x10 */ u32 C1ErrNum;
    /* 0x14 */ u32 C1ErrSector[4];
} __OSBlockInfo; /* size 0x24 */

typedef struct {
    /* 0x00 */ u32 cmdType;
    /* 0x04 */ u16 transferMode;
    /* 0x06 */ u16 blockNum;
    /* 0x08 */ s32 sectorNum;
    /* 0x0C */ u32 devAddr;
    /* 0x10 */ u32 bmCtlShadow;
    /* 0x14 */ u32 seqCtlShadow;
    /* 0x18 */ __OSBlockInfo block[2];
} __OSTranxInfo;

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
    /* 0x14 */ __OSTranxInfo transferInfo;
} OSPiHandle;

typedef struct {
    /* 0x00 */ u16 type;
    /* 0x02 */ u8 pri;
    /* 0x03 */ u8 status;
    /* 0x04 */ void *retQueue;
    /* 0x08 */ void *dramAddr;
    /* 0x0C */ u32 devAddr;
    /* 0x10 */ u32 size;
    /* 0x14 */ OSPiHandle *piHandle;
} OSIoMesg;

typedef struct {
    /* 0x00 */ s32 initialized;
    /* 0x04 */ void *thread;
    /* 0x08 */ void *cmdQueue;
    /* 0x0C */ void *evtQueue;
    /* 0x10 */ void *acsQueue;
    /* 0x14 */ s32 (*dma_func)(s32, u32, void *, u32);
    /* 0x18 */ s32 (*edma_func)(OSPiHandle *, s32, u32, void *, u32);
} OSMgrArgs;

extern s32 osRecvMesg(void *mq, void **msg, s32 flag);
extern s32 osSendMesg(void *mq, void *msg, s32 flag);
extern void func_0029DEE0(u32 mask);                                /* __osResetGlobalIntMask */
extern void func_0029DF30(u32 mask);                                /* __osSetGlobalIntMask */
extern s32 func_0029DBD0(OSPiHandle *pihandle, u32 devAddr, u32 data);   /* osEPiRawWriteIo */
extern s32 func_0029DA30(OSPiHandle *pihandle, u32 devAddr, u32 *data);  /* osEPiRawReadIo */
extern void func_0029DF70(void);                                    /* osYieldThread */

void func_0029D690(void *args) {
    OSIoMesg *mb;
    void *em;
    void *dummy;
    s32 ret;
    OSMgrArgs *dm;
    s32 messageSend;

    messageSend = 0;
    mb = NULL;
    ret = 0;
    dm = (OSMgrArgs *)args;

    while (1) {
        osRecvMesg(dm->cmdQueue, (void **)&mb, 1);

        if (mb->piHandle != NULL && mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 || mb->piHandle->transferInfo.cmdType == 1)) {
            __OSBlockInfo *blockInfo;
            __OSTranxInfo *info;
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;
            if (info->transferMode != 3) {
                blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }
            if (info->transferMode == 2 && mb->piHandle->transferInfo.cmdType == 0) {
                messageSend = 1;
            } else {
                messageSend = 0;
            }
            osRecvMesg(dm->acsQueue, &dummy, 1);
            func_0029DEE0(0x100401);
            func_0029DBD0(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x80000000);

        again:
            osRecvMesg(dm->evtQueue, &em, 1);
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            if (blockInfo->errStatus == 0x1D) {
                u32 stat;
                func_0029DBD0(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x10000000);
                func_0029DBD0(mb->piHandle, 0x05000510, info->bmCtlShadow);
                func_0029DA30(mb->piHandle, 0x05000508, &stat);
                if (stat & 0x02000000) {
                    func_0029DBD0(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x01000000);
                }
                blockInfo->errStatus = 4;
                *(volatile u32 *)0xA4600010 = 2; /* PI_STATUS_REG */
                func_0029DF30(0x100C01);
            }
            osSendMesg(mb->retQueue, mb, 0);
            if (messageSend == 1 && mb->piHandle->transferInfo.block[0].errStatus == 0) {
                messageSend = 0;
                goto again;
            }
            osSendMesg(dm->acsQueue, NULL, 0);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                func_0029DF70();
            }
        } else {
            switch (mb->type) {
                case 11:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->dma_func(0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 12:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->dma_func(1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 15:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->edma_func(mb->piHandle, 0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 16:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->edma_func(mb->piHandle, 1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 10:
                    osSendMesg(mb->retQueue, mb, 0);
                    ret = -1;
                    break;
                default:
                    ret = -1;
                    break;
            }
            if (ret == 0) {
                osRecvMesg(dm->evtQueue, &em, 1);
                osSendMesg(mb->retQueue, mb, 0);
                osSendMesg(dm->acsQueue, NULL, 0);
            }
        }
    }
}

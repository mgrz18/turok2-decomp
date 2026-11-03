/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* From decompals/ultralib (libultra 2.0I): src/io/pfsinitpak.c, osPfsInitPak. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;

typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    /* 0x00 */ s32 status;
    /* 0x04 */ OSMesgQueue *queue;
    /* 0x08 */ s32 channel;
    /* 0x0C */ u8 id[32];
    /* 0x2C */ u8 label[32];
    /* 0x4C */ s32 version;
    /* 0x50 */ s32 dir_size;
    /* 0x54 */ s32 inode_table;
    /* 0x58 */ s32 minode_table;
    /* 0x5C */ s32 dir_table;
    /* 0x60 */ s32 inode_start_page;
    /* 0x64 */ u8 banks;
    /* 0x65 */ u8 activebank;
} OSPfs;

typedef struct {
    /* 0x00 */ u32 repaired;
    /* 0x04 */ u32 random;
    /* 0x08 */ u64 serial_mid;
    /* 0x10 */ u64 serial_low;
    /* 0x18 */ u16 deviceid;
    /* 0x1A */ u8 banks;
    /* 0x1B */ u8 version;
    /* 0x1C */ u16 checksum;
    /* 0x1E */ u16 inverted_checksum;
} __OSPackId;

#define PFS_INITIALIZED 1
#define PFS_ERR_DEVICE 11

extern void func_00299FE4(void); /* __osSiGetAccess */
extern void func_0029A050(void); /* __osSiRelAccess */
extern s32 func_00427F80(OSMesgQueue *queue, int channel); /* __osPfsGetStatus */
extern s32 func_00429EC4(OSPfs *pfs); /* __osPfsSelectBank */
extern s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer); /* __osContRamRead */
extern s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum); /* __osIdCheckSum */
extern s32 __osCheckPackId(OSPfs *pfs, __OSPackId *temp); /* __osCheckPackId */
extern s32 __osRepairPackId(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid); /* __osRepairPackId */
extern s32 func_00428330(OSPfs *pfs); /* osPfsChecker */

s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, int channel) {
    int k;
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[32];
    __OSPackId *id;
    __OSPackId newid;

    func_00299FE4();
    ret = func_00427F80(queue, channel);
    func_0029A050();
    if (ret != 0) {
        return ret;
    }

    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    pfs->activebank = 0;
    if ((ret = func_00429EC4(pfs)) != 0) {
        return ret;
    }
    if ((ret = func_00299680(pfs->queue, pfs->channel, 1, temp)) != 0) {
        return ret;
    }

    __osIdCheckSum((u16 *)temp, &sum, &isum);
    id = (__OSPackId *)temp;

    if (id->checksum != sum || id->inverted_checksum != isum) {
        ret = __osCheckPackId(pfs, id);
        if (ret != 0) {
            return ret;
        } else if (ret != 0) { /* duplicated check, as in 2.0I */
            return ret;
        }
    }

    if (!(id->deviceid & 1)) {
        ret = __osRepairPackId(pfs, id, &newid);
        if (ret != 0) {
            return ret;
        }
        id = &newid;
        if (!(id->deviceid & 1)) {
            return PFS_ERR_DEVICE;
        }
    }

    for (k = 0; k < 32; k++) {
        pfs->id[k] = ((u8 *)id)[k];
    }

    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + 2 + (2 * pfs->banks);
    pfs->dir_size = 2 * 8;
    pfs->inode_table = 1 * 8;
    pfs->minode_table = (1 + pfs->banks) * 8;
    pfs->dir_table = pfs->minode_table + (pfs->banks * 8);

    if ((ret = func_00299680(pfs->queue, pfs->channel, 7, pfs->label)) != 0) {
        return ret;
    }

    ret = func_00428330(pfs);
    pfs->status |= PFS_INITIALIZED;
    return ret;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/OS_2/802B7B80_28.CPP, func_802B7FD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
typedef struct __OSContRamReadFormat __OSContRamReadFormat;
struct __OSContRamReadFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 address;
    u8 data[32];
    u8 datacrc;
};
struct __OSContRamReadFormat;
typedef struct OSPfs OSPfs;
struct OSPfs {
    int status;
    struct OSMesgQueue *queue;
    int channel;
    u8 id[32];
    u8 label[32];
    int version;
    int dir_size;
    int inode_table;
    int minode_table;
    int dir_table;
    int inode_start_page;
    u8 banks;
    u8 activebank;
};
struct OSPfs;
struct OSMesgQueue;
struct OSPifRam;
struct OSPifRam {
    u32 ramarray[15];
    u32 pifstatus;
};
struct OSMesgQueue;
struct OSPifRam;
typedef struct OSPifRam OSPifRam;
extern OSPifRam D_80121430[4];
extern u8 D_80121530[32];
extern OSPifRam D_80121550[4];
extern u8 D_80121650[32];
extern s32 D_800B7A40[4];
extern s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
extern s32 func_00299A40(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
extern u8 func_00299E00(u16 address);
static __inline__ void _MakeMotorData(int channel, u16 address, u8 *buffer, OSPifRam *mdata) {
    u8 *ptr = (u8 *)mdata->ramarray;
    __OSContRamReadFormat ramreadformat;
    int i;
    for (i = 0; i < ((s32)(sizeof(mdata->ramarray) / sizeof(mdata->ramarray[0]))); i++) {
        mdata->ramarray[i] = 0;
    }
    mdata->pifstatus = 1;
    ramreadformat.dummy = 0xFF;
    ramreadformat.txsize = 35;
    ramreadformat.rxsize = 1;
    ramreadformat.cmd = 3;
    ramreadformat.address = (address << 0x5) | func_00299E00(address);
    ramreadformat.datacrc = 0xFF;
    for (i = 0; i < ((s32)(sizeof(ramreadformat.data) / sizeof(ramreadformat.data[0]))); i++) {
        ramreadformat.data[i] = *buffer++;
    }
    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = 0;
        }
    }
    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = 0xFE;
}
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel)
{
    int i;
    s32 ret;
    u8 temp[32];
    u8 *startbuf;
    u8 *stopbuf;
    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 128;
    for (i = 0; i < ((s32)(sizeof(temp) / sizeof(temp[0]))); i++) {
        temp[i] = 254;
    }
    ret = func_00299A40(mq, channel, 0x400, temp, 0);
    if (ret == 2) {
        ret = func_00299A40(mq, channel, 0x400, temp, 0);
    }
    if (ret != 0) {
        return ret;
    }
    ret = func_00299680(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }
    if (temp[31] == 254) {
        return 11;
    }
    for (i = 0; i < ((s32)(sizeof(temp) / sizeof(temp[0]))); i++) {
        temp[i] = 128;
    }
    ret = func_00299A40(mq, channel, 0x400, temp, 0);
    if (ret == 2) {
        ret = func_00299A40(mq, channel, 0x400, temp, 0);
    }
    if (ret != 0) {
        return ret;
    }
    ret = func_00299680(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }
    if (temp[31] != 0x80) {
        return 11;
    }
    if (!D_800B7A40[channel]) {
        for (i = 0; i < ((s32)(sizeof(D_80121530) / sizeof(D_80121530[0]))); i++) {
            startbuf = D_80121530;
            startbuf[i] = 1;
            stopbuf = D_80121650;
            stopbuf[i] = 0;
        }
        _MakeMotorData(channel, 0x600, D_80121530, &D_80121430[channel]);
        _MakeMotorData(channel, 0x600, D_80121650, &D_80121550[channel]);
        D_800B7A40[channel] = 1;
    }
    return 0;
}

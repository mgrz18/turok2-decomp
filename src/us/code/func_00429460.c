/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/__osRepairPackId.c, __osRepairPackId.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef union {
    struct { f32 f_odd; f32 f_even; } f;
} __OSfp;
typedef struct {
               u64 at, v0, v1, a0, a1, a2, a3;
               u64 t0, t1, t2, t3, t4, t5, t6, t7;
               u64 s0, s1, s2, s3, s4, s5, s6, s7;
               u64 t8, t9, gp, sp, s8, ra;
                u64 lo, hi;
                u32 sr, pc, cause, badvaddr, rcp;
                u32 fpcsr;
    __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;
typedef struct OSThread_s {
               struct OSThread_s *next;
               OSPri priority;
               struct OSThread_s **queue;
               struct OSThread_s *tlnext;
               u16 state;
               u16 flags;
               OSId id;
               int fp;
               __OSThreadprofile_s *thprof;
               __OSThreadContext context;
} OSThread;
typedef struct OSMesgQueue_s {
               OSThread *mtqueue;
               OSThread *fullqueue;
               s32 validCount;
               s32 first;
               s32 msgCount;
               OSMesg *msg;
} OSMesgQueue;
typedef struct {
               s32 status;
               OSMesgQueue *queue;
               s32 channel;
               u8 id[32];
               u8 label[32];
               s32 version;
               s32 dir_size;
               s32 inode_table;
               s32 minode_table;
               s32 dir_table;
               s32 inode_start_page;
               u8 banks;
               u8 activebank;
} OSPfs;
u32 func_0029DFC0(void);
s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer);
s32 func_00299A40(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force);
typedef struct {
               u32 repaired;
               u32 random;
               u64 serial_mid;
               u64 serial_low;
               u16 deviceid;
               u8 banks;
               u8 version;
               u16 checksum;
               u16 inverted_checksum;
} __OSPackId;
s32 func_00429EC4(OSPfs *pfs);
static __inline__ s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data;
    u32 j;
    *csum = *icsum = 0;
    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u8 *)ptr + j);
        *csum += data;
        *icsum += ~data;
    }
    return 0;
}
s32 __osRepairPackId(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid) {
    u8 buf[32];
    u8 temp2[32];
    u16 addr[4];
    s32 i;
    s32 j;
    s32 ret;
    s32 dev;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429EC4(pfs);
        if (ret != 0) {
            return ret;
        }
    }
    newid->repaired = -1;
    newid->random = func_0029DFC0();
    newid->serial_mid = badid->serial_mid;
    newid->serial_low = badid->serial_low;
    for (i = 0; i < 62; i++) {
        pfs->activebank = i;
        ret = func_00429EC4(pfs);
        if (ret != 0) {
            return ret;
        }
        ret = func_00299680(pfs->queue, pfs->channel, 0, buf);
        if (ret != 0) {
            return ret;
        }
        buf[0] = i | 0x80;
        for (j = 1; j < 32; j++) {
            buf[j] = ~buf[j];
        }
        ret = func_00299A40(pfs->queue, pfs->channel, 0, buf, 0);
        if (ret != 0) {
            return ret;
        }
        ret = func_00299680(pfs->queue, pfs->channel, 0, temp2);
        if (ret != 0) {
            return ret;
        }
        for (j = 0; j < 32; j++) {
            if (temp2[j] != buf[j]) {
                break;
            }
        }
        if (j != 32) {
            break;
        }
        if (i > 0) {
            pfs->activebank = 0;
            ret = func_00429EC4(pfs);
            if (ret != 0) {
                return ret;
            }
            ret = func_00299680(pfs->queue, pfs->channel, 0, buf);
            if (ret != 0) {
                return ret;
            }
            if (buf[0] != 0x80) {
                break;
            }
        }
    }
    pfs->activebank = 0;
    ret = func_00429EC4(pfs);
    if (ret != 0) {
        return ret;
    }
    dev = i > 0;
    newid->deviceid = dev | (badid->deviceid & 0xFFFE);
    newid->banks = i;
    newid->version = badid->version;
    __osIdCheckSum((u16 *)newid, &newid->checksum, &newid->inverted_checksum);
    addr[0] = 1;
    addr[1] = 3;
    addr[2] = 4;
    addr[3] = 6;
    for (j = 0; j < 4; j++) {
        ret = func_00299A40(pfs->queue, pfs->channel, addr[j], (u8 *)newid, 1);
        if (ret != 0) {
            return ret;
        }
    }
    ret = func_00299680(pfs->queue, pfs->channel, 1, buf);
    if (ret != 0) {
        return ret;
    }
    for (j = 0; j < 32; j++) {
        if (buf[j] != ((u8 *)newid)[j]) {
            return 10;
        }
    }
    return 0;
}

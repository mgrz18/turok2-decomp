/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SDK/PFS/80447BB0_20.CPP, func_80448BFC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
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
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 __osCheckId(OSPfs *pfs);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
s32 __osCheckId(OSPfs *pfs)
{
    int k;
    u8 temp[32];
    s32 ret;
    if (pfs->activebank != 0) {
        pfs->activebank = 0;
        ret = func_00429DE4(pfs);
        if (ret != 0)
            return ret;
    }
    ret = func_00299680(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0) {
        if (ret != 2) {
            return ret;
        }
        ret = func_00299680(pfs->queue, pfs->channel, 1, temp);
        if (ret != 0)
            return ret;
    }
    for (k = 0; k < 32; k++) {
        if (pfs->id[k] != temp[k])
            return 2;
    }
    return 0;
}

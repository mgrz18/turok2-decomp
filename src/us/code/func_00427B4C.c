/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_2.c, func_8044694C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSPfs;
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
typedef struct OSPfs OSPfs;
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00429EC4(OSPfs *pfs);
extern s32 func_00427B4C(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank);
extern s32 func_00429EC4(OSPfs *pfs);
extern u16 func_00429D84(u8 *bytes, s32 count);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
s32 func_00427B4C(OSPfs *pfs, u8 page_no, u16 *sum, u8 bank)
{
    int i;
    s32 ret;
    u8 data[32];
    ret = 0;
    pfs->activebank = bank;
    ret = func_00429EC4(pfs);
    if (ret != 0)
        return ret;
    for (i = 0; i < 8; i++) {
        ret = func_00299680(pfs->queue, pfs->channel, page_no * 8 + i, data);
        if (ret != 0) {
            pfs->activebank = 0;
            func_00429EC4(pfs);
            return ret;
        }
        *sum = *sum + func_00429D84(data, sizeof(data));
    }
    pfs->activebank = 0;
    ret = func_00429EC4(pfs);
    return ret;
}

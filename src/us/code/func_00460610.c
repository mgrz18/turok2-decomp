/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_2.c, func_804464F0_de.
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
extern s32 func_00460610(OSPfs *pfs, int page_no, u8 *data, u8 bank);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00429DE4(OSPfs *pfs);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);
s32 func_00460610(OSPfs *pfs, int page_no, u8 *data, u8 bank)
{
    int i;
    s32 ret;
    ret = 0;
    pfs->activebank = bank;
    ret = func_00429DE4(pfs);
    if (ret != 0)
        return ret;
    for (i = 0; i < 8; i++) {
        ret = func_00299A40(pfs->queue, pfs->channel, page_no * 8 + i, data, 0);
        if (ret != 0) {
            break;
        }
    }
    pfs->activebank = 0;
    ret = func_00429DE4(pfs);
    return ret;
}
extern s32 func_00299A40(void *queue, int channel, u16 address, u8 *buffer, int force);

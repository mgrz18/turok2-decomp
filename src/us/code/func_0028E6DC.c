/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C85C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
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
struct OSMesgQueue;
struct Draw;
typedef struct Draw Draw;
struct OSMesgQueue;
struct Draw;
struct Draw {
    char pad0[8];
    s32 allocationMode;
    void *model;
    char reserved10[0x10];
    IntrusiveList recycled;
};
extern void func_0028E6DC(s32 arg0);
struct Block_func_8023C85C_de;
typedef struct Block_func_8023C85C_de Block_func_8023C85C_de;
struct Block_func_8023C85C_de;
struct Block_func_8023C85C_de {
    s16 type;
    s16 pad;
    s32 value;
    void *data;
};
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 D_801206C4;
extern void func_002052D8(void *arg0, void *arg1);
void func_0029B030(void *arg0, s32 arg1, s32 arg2);
void osSendMesg(void *arg0, void *arg1, s32 arg2);
extern void func_0029B030(void *arg0, s32 arg1, s32 arg2);
extern void osSendMesg(void *arg0, void *arg1, s32 arg2);
void func_0028E6DC(s32 arg0) {
    char sp10[0x18];
    Block_func_8023C85C_de block;
    s32 sp38;
    void *sp3C;
    func_0029B030(sp10, (s32)&sp38, 1);
    block.type = 2;
    block.value = arg0;
    block.data = sp10;
    osSendMesg(&D_801206C4, &block, 1);
    osRecvMesg(sp10, &sp3C, 1);
    func_002052D8(0, ((Draw *)(sp3C))->model);
}

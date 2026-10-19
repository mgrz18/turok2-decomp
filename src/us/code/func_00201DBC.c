/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de.c, func_8025637C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
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
struct OSMesgQueue;
struct Chunk;
struct CartridgeReadWorker;
struct CartridgeReadWorker {
    u8 reserved[0x230];
    u8 requestQueueStorage;
    u8 reserved231[0xA48 - 0x231];
    u8 completionQueueStorage;
    u8 reservedA49[0x5068 - 0xA49];
    IntrusiveList available;
    IntrusiveList pending;
};
struct Chunk;
struct Chunk {
    u32 source;
    void *destination;
    s32 length;
    void *queue;
    s32 value;
};
struct Chunk;
struct Chunk;
typedef struct Chunk Chunk;
struct CartridgeReadWorker;
typedef struct CartridgeReadWorker CartridgeReadWorker;
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern Chunk *func_0020205C(CartridgeReadWorker *arg0);
extern s32 osJamMesg(void *arg0, void *arg1, s32 arg2);
extern s32 osJamMesg(void *, void *, s32);
void func_00201DBC(struct CartridgeReadWorker *arg0, u32 arg1, s32 arg2, void *arg3) {
    extern void func_0029B030(void *arg0, s32 arg1, s32 arg2);
    char queue[0x18];
    s32 queue_init;
    s32 completed;
    s32 remaining;
    s32 length;
    Chunk *chunk;
    func_0029B030(queue, (s32) &queue_init, 1);
    remaining = arg2;
    do {
        chunk = func_0020205C(arg0);
        length = 0x2000;
        if (chunk == 0) {
            break;
        }
        chunk->source = arg1 + (arg2 - remaining);
        chunk->destination = (char *) arg3 + (arg2 - remaining);
        if ((u32) remaining < 0x2001U) {
            length = remaining;
        }
        chunk->length = length;
        chunk->queue = queue;
        osJamMesg(&arg0->requestQueueStorage, chunk, 1);
        osRecvMesg(queue, &completed, 1);
        remaining -= length;
    } while (remaining != 0);
}

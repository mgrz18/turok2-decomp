/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de.c, func_80256454_de.
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
struct Queue;
struct Queue {
    char pad[0x14];
    s32 items[4];
};
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
struct Queue;
struct Chunk;
typedef struct Chunk Chunk;
struct CartridgeReadWorker;
typedef struct CartridgeReadWorker CartridgeReadWorker;
extern Chunk *func_0020205C(CartridgeReadWorker *arg0);
extern s32 osJamMesg(void *arg0, void *arg1, s32 arg2);
typedef struct Queue Queue;
extern s32 osJamMesg(void *, void *, s32);
extern s32 osSendMesg(void *, s32, s32);
void func_00201E94(struct CartridgeReadWorker *arg0, u32 source, s32 length, void *destination,
                   Queue *queue, s32 value, s32 direct) {
    s32 remaining;
    s32 chunk_length;
    Chunk *chunk;
    if (direct != 0) {
        chunk = func_0020205C(arg0);
        if (chunk != 0) {
            chunk->source = source;
            chunk->destination = destination;
            chunk->length = length;
            chunk->queue = queue;
            chunk->value = value;
            osJamMesg(&arg0->requestQueueStorage, chunk, 1);
        }
    } else {
        remaining = length;
        do {
            chunk = func_0020205C(arg0);
            chunk_length = 0x2000;
            if (chunk == 0) {
                break;
            }
            chunk->source = source + (length - remaining);
            chunk->destination = (char *)destination + (length - remaining);
            if ((u32)remaining < 0x2001U) {
                chunk_length = remaining;
            }
            remaining -= chunk_length;
            chunk->length = chunk_length;
            if (remaining != 0) {
                chunk->queue = 0;
                chunk->value = 0;
            } else {
                chunk->queue = queue;
                chunk->value = value;
            }
            osSendMesg(&arg0->requestQueueStorage, (s32)chunk, 1);
        } while ((chunk != 0) && (remaining != 0));
    }
}

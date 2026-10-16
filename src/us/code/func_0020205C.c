/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_4.c, func_8025661C_de.
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
struct CartridgeReadRequest;
struct CartridgeReadRequest {
    u32 devAddr;
    void *buffer;
    s32 size;
    void *replyQueue;
    void *replyMsg;
    s32 active;
    void *next;
    void *prev;
};
struct CartridgeReadWorker;
typedef struct CartridgeReadWorker CartridgeReadWorker;
struct CartridgeReadRequest;
typedef struct CartridgeReadRequest CartridgeReadRequest;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern void CList__Remove(void *, s32);
extern s32 CList__AddHead(void *, s32);
void *func_0020205C(CartridgeReadWorker *arg0) {
    u32 temp_s2;
    void *temp_s0;
    temp_s2 = func_0029DFF0();
    temp_s0 = arg0->available.head;
    if (temp_s0 != 0) {
        CList__Remove(&arg0->available, (s32) temp_s0);
        ((CartridgeReadRequest *)temp_s0)->active = 1;
        CList__AddHead(&arg0->pending, (s32) temp_s0);
    }
    func_0029E010(temp_s2);
    return temp_s0;
}
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern void CList__Remove(void *, s32);
extern s32 CList__AddHead(void *, s32);

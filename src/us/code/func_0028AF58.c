/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F8BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct OSSched;
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
struct OSMesgQueue;
struct OSScClientWork;
struct OSScClientWork {
    struct OSScClientWork *next;
    OSMesgQueue *queue;
};
struct OSScTask_s;
struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    struct { s32 type; } list;
};
struct OSSched;
extern void func_0028AF58(struct OSSched *arg0, void *arg1);
struct OSScTask_s;
typedef struct OSScTask_s OSScTask_s;
struct OSScClientWork;
struct OSScTask_s;
struct OSSched;
struct OSSched {
    char pad0[0x20];
    s16 retraceMessage;
    char pad22[0x40 - 0x22];
    OSMesgQueue interruptQueue;
    char pad58[0x78 - 0x58];
    OSMesgQueue commandQueue;
    char pad90[0x2E0 - 0x90];
    struct OSScClientWork *clientList;
    struct OSScTask_s *audioListHead;
    struct OSScTask_s *gfxListHead;
    struct OSScTask_s *audioListTail;
    struct OSScTask_s *gfxListTail;
    struct OSScTask_s *curRSPTask;
    struct OSScTask_s *curRDPTask;
    u32 frameCount;
    s32 doAudio;
};
struct OSScClientWork;
typedef struct OSScClientWork OSScClientWork;
struct OSSched;
typedef struct OSSched OSSched;
struct OSMesgQueue;
struct OSMesgQueue;
s32 func_0029E230(s32);
extern s32 func_0029E230(s32);
void func_0028AF58(OSSched *arg0, void *arg1) {
    s32 saved;
    OSScClientWork *cur;
    OSScClientWork *prev;
    prev = 0;
    cur = arg0->clientList;
    saved = func_0029E230(1);
    while (cur != 0) {
        if (cur == arg1) {
            if (prev != 0) {
                prev->next = cur->next;
            } else {
                arg0->clientList = cur->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    func_0029E230(saved);
}

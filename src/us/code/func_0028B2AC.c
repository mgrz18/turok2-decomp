/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FC10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
struct OSSched;
extern void func_0028AFF0(struct OSSched *arg0, void *arg1);
extern void func_0028B274(void *arg0);
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
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
struct OSMesgQueue;
struct OSMesgQueue;
extern int D_800B6E88;
struct View_func_8028FA60_de;
typedef struct View_func_8028FA60_de View_func_8028FA60_de;
struct Scene_func_8028FA60_de;
struct Scene_func_8028FA60_de {
    s32 pad0;
    s32 flags;
    s32 options;
    s32 padC;
    s32 mode;
    char pad14[0x38 - 0x14];
    s32 music;
    s64 *time;
};
struct OSScClientWork;
struct OSScClientWork {
    struct OSScClientWork *next;
    OSMesgQueue *queue;
};
struct Scene_func_8028FA60_de;
struct View_func_8028FA60_de;
struct View_func_8028FA60_de {
    char pad0[0x78];
    struct OSMesgQueue messages;
    char pad90[0x2F4 - 0x90];
    struct Scene_func_8028FA60_de *current;
    struct Scene_func_8028FA60_de *next;
    char pad2FC[4];
    s32 pending;
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
struct Scene_func_8028FA60_de;
typedef struct Scene_func_8028FA60_de Scene_func_8028FA60_de;
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
struct Node_func_8028FC10_de;
typedef struct Node_func_8028FC10_de Node_func_8028FC10_de;
struct Node_func_8028FC10_de;
struct Node_func_8028FC10_de {
    char pad0[8];
    u32 flags;
    char padC[4];
    s32 type;
};
struct Ctx_func_8028F544_de;
struct Node_func_8028F544_de;
struct Scene_func_8028FA60_de;
struct View_func_8028FA60_de;
extern s32 func_0028ABE0(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void func_0028B0FC(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
struct OSMesgQueue;
struct OSMesgQueue;
struct Ctx_func_8028F544_de;
typedef struct Ctx_func_8028F544_de Ctx_func_8028F544_de;
struct Node_func_8028F544_de;
typedef struct Node_func_8028F544_de Node_func_8028F544_de;
struct Node_func_8028F544_de;
struct Node_func_8028F544_de {
    struct Node_func_8028F544_de *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};
struct Ctx_func_8028F544_de;
struct Node_func_8028F544_de;
struct Ctx_func_8028F544_de {
    char p0[0x2E4];
    struct Node_func_8028F544_de *unk2E4;
    struct Node_func_8028F544_de *unk2E8;
    s32 unk2EC;
    s32 unk2F0;
    char p1[0xC];
    s32 unk300;
};
extern void func_0029B9B0(void *arg0, s32 arg1);
extern s32 D_800B6E54;
extern s32 D_800D6D68;
s32 func_0028ABE0(Ctx_func_8028F544_de *arg0, Node_func_8028F544_de **arg1, Node_func_8028F544_de **arg2, s32 arg3);
void func_0028AFF0(OSSched *arg0, void *arg1);
void func_0028B0FC(View_func_8028FA60_de *view, Scene_func_8028FA60_de *current, Scene_func_8028FA60_de *next);
void func_0028B274(void *arg0);
void func_0028B2AC(void *arg0) {
    Node_func_8028FC10_de *node = 0;
    s32 value1 = 0;
    s32 value2 = 0;
    s32 flags;
    while (osRecvMesg(&((View_func_8028FA60_de *)arg0)->messages, &node, 0) != -1) {
        if (node->type == 1 && (node->flags & 0x20) && D_800B6E54 == 0) {
            D_800B6E54 = 2;
            func_0029B9B0(&D_800D6D68, D_800B6E88);
        }
        func_0028AFF0(arg0, node);
    }
    if (((View_func_8028FA60_de *)arg0)->pending != 0 &&
        ((View_func_8028FA60_de *)arg0)->current != 0) {
        func_0028B274(arg0);
        return;
    }
    flags = (((View_func_8028FA60_de *)arg0)->current == 0) * 2 |
            (((View_func_8028FA60_de *)arg0)->next == 0);
    if (((s32 (*)(void *, s32 *, s32 *, s32))func_0028ABE0)(arg0, &value1, &value2, flags) != flags) {
        ((void (*)(void *, s32, s32))func_0028B0FC)(arg0, value1, value2);
    }
}

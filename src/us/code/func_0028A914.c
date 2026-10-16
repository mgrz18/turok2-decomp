/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8028F278_de.c, func_8028F278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
struct OSSched;
extern void func_0029AC90(s32 arg0);
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
struct FrameSchedule;
struct FrameSchedule {
    char pad0[0x110];
    s32 task;
    char pad114[0x10];
    u32 duration;
    char pad128[0x18];
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
struct FrameSchedule;
typedef struct FrameSchedule FrameSchedule;
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
struct Ctx_func_8028F544_de;
struct Node_func_8028F544_de;
struct Scene_func_8028FA60_de;
struct View_func_8028FA60_de;
extern s32 func_0028ABE0(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void func_0028B0FC(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern s32 func_0028B0A8(void *,void *);
struct OSMesgQueue;
struct OSMesgQueue;
extern FrameSchedule *D_8011AD18[];
extern s64 D_8011F168;
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
extern void func_00228BBC();
extern u32 D_800C2210;
extern u64 func_0029B170(void);
extern u32 D_800F6CA8;
extern u32 D_8011AD14;
extern u32 D_8011B09C;
extern u32 D_8011B10C;
extern float D_800A9E40;
extern float D_800B6D44;
extern FrameSchedule D_800F6CB0[];
extern u32 D_8011AD88;
extern s32 D_8011B098;
extern s32 D_8011F130;
extern s32 D_8011F138;
extern u32 D_8011F134;
void func_0028A914(OSSched *sc)
{
    s32 value1 = 0;
    s32 value2 = 0;
    OSScTask_s *task = sc->curRDPTask;
    void *framebuffer;
    FrameSchedule *schedule;
    FrameSchedule *schedules;
    s32 flags;
    task->state &= ~1;
    if (task->list.type == 1) {
        if ((task->flags & 0x20) != 0) {
            D_800B6D44 = (f32)(((func_0029B170() - D_8011F168) << 6) / 3) * D_800A9E40;
        }
        sc->curRDPTask = 0;
        if ((task->flags & 0x20) != 0) {
            schedules = D_800F6CB0;
            framebuffer = task->framebuffer;
            if (framebuffer == (void *)schedules[0].task) {
                schedule = &schedules[0];
            } else if (framebuffer == (void *)schedules[1].task) {
                schedule = &schedules[1];
            } else if (framebuffer == (void *)schedules[2].task) {
                schedule = &schedules[2];
            } else {
                schedule = &schedules[0];
            }
            D_8011B09C = D_8011AD14;
            D_8011AD14 = D_800F6CA8;
            D_800F6CA8 = D_8011B10C;
            D_8011B10C = D_8011AD88 + 1;
            if (D_8011B10C >= schedule->duration && D_8011F138 == 0 && D_8011F130 == 0) {
                D_8011B098 = schedule->task;
                func_0029AC90(schedule->task);
                D_8011F130++;
            } else {
                D_8011F138++;
                D_8011AD18[D_8011F134] = schedule;
                D_8011F134++;
                if (D_8011F134 >= D_800C2210) {
                    D_8011F134 = 0;
                }
            }
            func_00228BBC(schedule);
        }
    }
    func_0028B0A8(sc, task);
    flags = (sc->curRSPTask == 0) * 2 | (sc->curRDPTask == 0);
    if (func_0028ABE0(sc, &value1, &value2, flags) != flags) {
        func_0028B0FC(sc, value1, value2);
    }
}

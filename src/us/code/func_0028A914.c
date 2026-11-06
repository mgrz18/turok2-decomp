/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8028F278_de.c, func_8028F278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void osViSwapBuffer(s32 arg0);
extern s32 __scSchedule(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void __scExec(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern s32 __scTaskComplete(void *,void *);
extern FrameSchedule *D_8011AD18[];
extern s64 D_8011F168;
extern void func_00228BBC();
extern u32 D_800C2210;
extern u64 osGetTime(void);
extern u32 D_800F6CA8;
extern u32 D_8011AD14;
extern u32 D_8011B09C;
extern u32 D_8011B10C;
extern float D_800A9E40;
extern float D_800B6D44;
extern FrameSchedule gEngineApp[];
extern u32 D_8011AD88;
extern s32 D_8011B098;
extern s32 D_8011F130;
extern s32 D_8011F138;
extern u32 D_8011F134;
void __scHandleRDP(OSSched *sc)
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
            D_800B6D44 = (f32)(((osGetTime() - D_8011F168) << 6) / 3) * D_800A9E40;
        }
        sc->curRDPTask = 0;
        if ((task->flags & 0x20) != 0) {
            schedules = gEngineApp;
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
                osViSwapBuffer(schedule->task);
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
    __scTaskComplete(sc, task);
    flags = (sc->curRSPTask == 0) * 2 | (sc->curRDPTask == 0);
    if (__scSchedule(sc, &value1, &value2, flags) != flags) {
        __scExec(sc, value1, value2);
    }
}

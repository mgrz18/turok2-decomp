/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028ED80.c, func_0028ED80.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
typedef struct {
               u32 type;
               u32 flags;
               u64 *ucode_boot;
               u32 ucode_boot_size;
               u64 *ucode;
               u32 ucode_size;
               u64 *ucode_data;
               u32 ucode_data_size;
               u64 *dram_stack;
               u32 dram_stack_size;
               u64 *output_buff;
               u64 *output_buff_size;
               u64 *data_ptr;
               u32 data_size;
               u64 *yield_data_ptr;
               u32 yield_data_size;
} OSTask_t;
typedef union {
    OSTask_t t;
    long long int force_structure_alignment;
} OSTask;
OSTime func_0029B170(void);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 func_0029A510(OSTask *task);
typedef struct Qn Qn;
struct Qn { Qn *next; OSMesgQueue *q; };
typedef struct Job {
                 struct Job *next;
                 u32 flags4;
                 u32 flags8;
                 void *unkC;
                 s32 unk10;
                 u8 pad14[0x24];
                 s32 unk38;
                 u64 *unk3C;
} Job;
typedef struct Vid {
                 s16 unk0;
                 u8 pad2[0x1E];
                 s16 unk20;
                 u8 pad22[0x1E];
                 OSMesgQueue q40;
                 OSMesg buf58[8];
                 OSMesgQueue q78;
                 OSMesg buf90[8];
                 OSThread thread;
                 u8 pad260[0x80];
                 Qn *unk2E0;
                 Job *head2E4;
                 Job *head2E8;
                 Job *tail2EC;
                 Job *tail2F0;
                 Job *cur2F4;
                 Job *next2F8;
                 s32 unk2FC;
                 s32 unk300;
} Vid;
typedef struct {
    u8 pad0[0x110];
    u32 fb;
    u32 unk114;
    u8 pad118[0x28];
} View;
OSTime func_0029B170(void);
void func_0020578C(s32 a, s32 b);
void func_0028B0A8(Vid *o, Job *j);
s32 func_0028ABE0(Vid *o, Job **a, Job **b, s32 mode);
void func_0028B0FC(Vid *o, Job *a, Job *b);
extern u64 D_8011F160;
extern u64 D_8011F150;
extern f32 D_8011F158;
extern f32 D_800B6D40;
extern f32 D_800B6D3C;
extern s32 D_800B6E50;
extern View D_800F6CB0[];
extern u32 D_80130A84;
void func_0028A43C(Vid *o) {
    Job *j;
    Job *a;
    Job *b;
    View *s;
    View *base;
    u32 fb;
    s32 mode;
    j = o->cur2F4;
    o->cur2F4 = 0;
    a = 0;
    b = 0;
    if ((j->flags4 & 0x10) && func_0029A510((OSTask *)((u8 *)j + 0x10)) != 0) {
        j->flags4 |= 0x20;
        if ((j->flags8 & 7) == 3) {
            j->next = o->head2E8;
            o->head2E8 = j;
            if (o->tail2F0 == 0) {
                o->tail2F0 = j;
            }
        }
        if (j->flags8 & 0x20) {
            D_8011F158 = D_8011F158 + (f32)(((func_0029B170() - D_8011F160) * 64) / 3) * 0.000001f;
        }
    } else {
        j->flags4 &= ~2;
        if (j->unk10 == 1) {
            D_800B6E50 = 0;
            if (!(j->flags8 & 0x20)) {
                goto done;
            }
            D_8011F158 = D_8011F158 + (f32)(((func_0029B170() - D_8011F160) * 64) / 3) * 0.000001f;
            D_800B6D40 = D_8011F158;
            D_800B6D3C = (f32)(((func_0029B170() - D_8011F150) * 64) / 3) * 0.000001f;
            base = D_800F6CB0;
            fb = (u32)j->unkC;
            if (fb == base[0].fb) {
                s = base;
                goto sel;
            }
            if (fb == base[1].fb) {
                s = base + 1;
                goto sel;
            }
            s = base;
            if (fb == base[2].fb) {
                s += 2;
            }
sel:
            func_0020578C(0, s->unk114 == D_80130A84);
            if (osSendMesg((OSMesgQueue *)((s32 *)j)[0x50 / 4], (OSMesg)((s32 *)j)[0x54 / 4], 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        } else {
            D_800B6D3C = (f32)(((func_0029B170() - D_8011F150) * 64) / 3) * 0.000001f;
            if (osSendMesg((OSMesgQueue *)((s32 *)j)[0x50 / 4], (OSMesg)((s32 *)j)[0x54 / 4], 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        }
done:
        func_0028B0A8(o, j);
    }
    mode = (o->cur2F4 == 0) << 1 | (o->next2F8 == 0);
    if (func_0028ABE0(o, &a, &b, mode) != mode) {
        func_0028B0FC(o, a, b);
    }
}

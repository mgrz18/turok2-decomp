#include "common.h"

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u64 *ucode_boot;
    /* 0x0C */ u32 ucode_boot_size;
    /* 0x10 */ u64 *ucode;
    /* 0x14 */ u32 ucode_size;
    /* 0x18 */ u64 *ucode_data;
    /* 0x1C */ u32 ucode_data_size;
    /* 0x20 */ u64 *dram_stack;
    /* 0x24 */ u32 dram_stack_size;
    /* 0x28 */ u64 *output_buff;
    /* 0x2C */ u64 *output_buff_size;
    /* 0x30 */ u64 *data_ptr;
    /* 0x34 */ u32 data_size;
    /* 0x38 */ u64 *yield_data_ptr;
    /* 0x3C */ u32 yield_data_size;
} RspTask; /* OSTask_t */

typedef struct ScTask {
    /* 0x00 */ struct ScTask *next;
    /* 0x04 */ u32 state;
    /* 0x08 */ u32 flags;
    /* 0x0C */ void *framebuffer;
    /* 0x10 */ RspTask list;
    /* 0x50 */ void *msgQ;
    /* 0x54 */ void *msg;
    /* 0x58 */ u64 startTime;
    /* 0x60 */ u64 totalTime;
} ScTask; /* OSScTask, size 0x68 */

typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ ScTask sc[2];
    /* 0x0F0 */ s16 msg;
    /* 0x0F2 */ u8 padF2[0x110 - 0xF2];
    /* 0x110 */ void *framebuffer;
    /* 0x114 */ u64 *dl[2];
    /* 0x11C */ u32 dlSize[2];
    /* 0x124 */ u8 pad124[4];
    /* 0x128 */ s32 twoTasks;
    /* 0x12C */ u8 pad12C[0x140 - 0x12C];
} GfxTask; /* size 0x140 */

typedef struct {
    /* 0x000 */ GfxTask tasks[3];
    /* 0x3C0 */ GfxTask *cur;
} GfxState;

extern u64 D_800BC330[]; /* rspboot */
extern u64 D_800BC400[]; /* gfx ucode text */
extern u64 rspbootTextEnd[]; /* = 0x800BC400, same address as D_800BC400: needs a linker symbol */
extern u64 D_800BD790[]; /* second gfx ucode text */
extern u64 D_800BF740[]; /* gfx ucode data */
extern u64 D_800BFB60[]; /* second gfx ucode data */
extern u64 D_800C4000[]; /* yield buffer */
extern u8 D_8011AD28[];  /* message queue */
extern u8 D_8011AD90[];
extern u32 D_80130A44;   /* fifo size */
extern u64 *D_80130A48;  /* fifo buffer */
extern u32 D_800B6D18;   /* frame counter */
extern s32 D_800B6D1C;   /* double-buffer index */
extern s32 D_800B6D20;
extern u32 D_800C2210;   /* task count */
extern s32 D_80130A4C;
extern s32 D_80130A50;
extern s32 D_80130A80;
extern s32 D_80130A84;

extern void func_0029E3B0(void);
extern void func_0028B3E8(void *, ScTask *);

/* Builds the RSP task(s) for a frame, hands them to the scheduler, moves to the next frame. */
void func_00286B58(GfxState *gfx, GfxTask *task) {
    u32 dataSize;
    u32 dataSize2;
    ScTask *t;
    u32 idx;
    GfxTask *cur;

    t = &task->sc[0];
    t->list.data_ptr = task->dl[0];
    dataSize = task->dlSize[0];
    t->list.type = 1;
    t->list.ucode_boot = D_800BC330;
    t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)D_800BC330;
    t->list.ucode_size = 0x1000;
    t->list.ucode_data_size = 0x800;
    t->list.yield_data_ptr = D_800C4000;
    t->list.yield_data_size = 0xC00;
    t->list.ucode = D_800BC400;
    t->list.ucode_data = D_800BF740;
    t->list.flags = 0;
    t->list.dram_stack = NULL;
    t->list.dram_stack_size = 0;
    t->list.output_buff = D_80130A48;
    t->list.output_buff_size = (u64 *)((u8 *)D_80130A48 + D_80130A44);
    t->list.data_size = dataSize;
    task->sc[0].next = NULL;
    t->msgQ = D_8011AD28;
    t->msg = &task->msg;
    t->framebuffer = task->framebuffer;
    if (task->twoTasks) {
        t->flags = 3;
    } else {
        t->flags = 0x23;
    }
    if (task->twoTasks) {
        t = &task->sc[1];
        t->list.data_ptr = task->dl[1];
        dataSize2 = task->dlSize[1];
        t->list.type = 1;
        t->list.ucode_boot = D_800BC330;
        t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)D_800BC330;
        t->list.ucode_size = 0x1000;
        t->list.ucode_data_size = 0x800;
        t->list.yield_data_ptr = D_800C4000;
        t->list.yield_data_size = 0xC00;
        t->list.ucode = D_800BD790;
        t->list.ucode_data = D_800BFB60;
        t->list.flags = 0;
        t->list.dram_stack = NULL;
        t->list.dram_stack_size = 0;
        t->list.output_buff = D_80130A48;
        t->list.output_buff_size = (u64 *)((u8 *)D_80130A48 + D_80130A44);
        t->list.data_size = dataSize2;
        task->sc[1].next = NULL;
        t->flags = 0x23;
        t->msgQ = D_8011AD28;
        t->msg = &task->msg;
        t->framebuffer = task->framebuffer;
    }
    func_0029E3B0();
    func_0028B3E8(D_8011AD90, &task->sc[0]);
    if (task->twoTasks) {
        func_0028B3E8(D_8011AD90, &task->sc[1]);
    }
    D_800B6D18++;
    idx = D_800B6D18 % D_800C2210;
    D_800B6D20++;
    D_800B6D1C ^= 1;
    gfx->cur = &gfx->tasks[idx];
    gfx->tasks[idx].dl[0] = D_800B6D1C == 0 ? D_80130A80 : D_80130A84;
    cur = gfx->cur;
    cur->dl[1] = D_800B6D1C == 0 ? D_80130A50 : D_80130A4C;
}

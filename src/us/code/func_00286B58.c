#include "common.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "globals/ucode.h"

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

extern u64 rspbootTextEnd[]; /* = 0x800BC400, same address as gspF3DEX_NoN_fifoTextStart: needs a linker symbol */
extern u64 gGfxYieldBuffer[]; /* yield buffer */
extern u8 gGfxFrameMessageQueue[];  /* message queue */
extern u8 gScheduler[];
extern u32 gRspFifoSize;   /* fifo size */
extern u64 *gRspFifoBuffer;  /* fifo buffer */

extern void osWritebackDCacheAll(void);
extern void Sched_SubmitTask(void *, ScTask *);

/* Builds the RSP task(s) for a frame, hands them to the scheduler, moves to the next frame. */
void CEngineApp__SubmitGfxTask(GfxState *gfx, GfxTask *task) {
    u32 dataSize;
    u32 dataSize2;
    ScTask *t;
    u32 idx;
    GfxTask *cur;

    t = &task->sc[0];
    t->list.data_ptr = task->dl[0];
    dataSize = task->dlSize[0];
    t->list.type = 1;
    t->list.ucode_boot = rspbootTextStart;
    t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
    t->list.ucode_size = 0x1000;
    t->list.ucode_data_size = 0x800;
    t->list.yield_data_ptr = gGfxYieldBuffer;
    t->list.yield_data_size = 0xC00;
    t->list.ucode = gspF3DEX_NoN_fifoTextStart;
    t->list.ucode_data = gspF3DEX_NoN_fifoDataStart;
    t->list.flags = 0;
    t->list.dram_stack = NULL;
    t->list.dram_stack_size = 0;
    t->list.output_buff = gRspFifoBuffer;
    t->list.output_buff_size = (u64 *)((u8 *)gRspFifoBuffer + gRspFifoSize);
    t->list.data_size = dataSize;
    task->sc[0].next = NULL;
    t->msgQ = gGfxFrameMessageQueue;
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
        t->list.ucode_boot = rspbootTextStart;
        t->list.ucode_boot_size = (u8 *)rspbootTextEnd - (u8 *)rspbootTextStart;
        t->list.ucode_size = 0x1000;
        t->list.ucode_data_size = 0x800;
        t->list.yield_data_ptr = gGfxYieldBuffer;
        t->list.yield_data_size = 0xC00;
        t->list.ucode = gspL3DEX_fifoTextStart;
        t->list.ucode_data = gspL3DEX_fifoDataStart;
        t->list.flags = 0;
        t->list.dram_stack = NULL;
        t->list.dram_stack_size = 0;
        t->list.output_buff = gRspFifoBuffer;
        t->list.output_buff_size = (u64 *)((u8 *)gRspFifoBuffer + gRspFifoSize);
        t->list.data_size = dataSize2;
        task->sc[1].next = NULL;
        t->flags = 0x23;
        t->msgQ = gGfxFrameMessageQueue;
        t->msg = &task->msg;
        t->framebuffer = task->framebuffer;
    }
    osWritebackDCacheAll();
    Sched_SubmitTask(gScheduler, &task->sc[0]);
    if (task->twoTasks) {
        Sched_SubmitTask(gScheduler, &task->sc[1]);
    }
    gFrameCount++;
    idx = gFrameCount % gTotalFramebuffers;
    gFrameCountGameplay++;
    gEvenOdd ^= 1;
    gfx->cur = &gfx->tasks[idx];
    gfx->tasks[idx].dl[0] = gEvenOdd == 0 ? gDisplayList1 : gDisplayList0;
    cur = gfx->cur;
    cur->dl[1] = gEvenOdd == 0 ? gLineList1 : gLineList0;
}

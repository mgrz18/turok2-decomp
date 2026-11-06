/* Boundary fixed in versions/cuts.us.txt: splat's VM_ManagerThread (0x23C bytes) holds three functions:
 * VM_ManagerThread (0x100), the reader thread at 0x28E8A4 (splat's D_0028E8A4,
 * 0xE8) and an unlink at 0x28E98C (0x54). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_16.CPP, func_8023C924_de,
 * func_8023CA14_de and func_8023CB0C_de, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "globals/sysrc.h"


extern char gPager_readerStack[];               /* the reader thread's stack; its OSThread is 0x230 below */
extern char gVMPages[];               /* the page array, 0x1000 per page */

void memset(void *dst, s32 value, s32 size);
void osCreateThread(void *thread, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
void osStartThread(void *thread);
void osCreateMesgQueue(OSMesgQueue *queue, void **msg, s32 count);
s32 osRecvMesg(OSMesgQueue *queue, void **msg, s32 block);
s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
void CLoader__LoadSync(CLoader *reader, u8 *source, s32 size, void *dest);
void osInvalDCache(void *addr, s32 size);
void osInvalICache(void *addr, s32 size);
void VM_HandleTLBMiss(void);
void VM_FinishPageJob(PageJob *job);
void VM_FreeRegion(PageJob *job);
void VM_AllocRegion(PageJob *job);
void VM_ReaderThread(void *unused);

/* Resource manager thread: starts the reader thread, then dispatches requests forever. */
void VM_ManagerThread(void) {
    void *request;

    memset(gPager_readerStack, 0x62, 0x200);
    osCreateThread(gPager_readerStack - 0x230, 0x62, VM_ReaderThread, NULL, gPager_readerStack + 0x200, 0x94);
    osStartThread(gPager_readerStack - 0x230);
    for (;;) {
        osRecvMesg(&gPager.completion, &request, 1);
        switch (((PageJob *)request)->type) {
            case 0:
                VM_HandleTLBMiss();
                break;
            case 1:
                VM_FinishPageJob(request);
                break;
            case 3:
                VM_AllocRegion(request);
                break;
            case 2:
                VM_FreeRegion(request);
                break;
        }
    }
}


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
#include "protos/ultra.h"



void memset(void *dst, s32 value, s32 size);
void CLoader__LoadSync(CLoader *reader, u8 *source, s32 size, void *dest);
void VM_HandleTLBMiss(void);
void VM_FinishPageJob(PageJob *job);
void VM_FreeRegion(PageJob *job);
void VM_AllocRegion(PageJob *job);
void VM_ReaderThread(void *unused);

/* Reader thread: reads each pending page from the cartridge into its slot of
 * the page array and hands the job back to the manager. */
void VM_ReaderThread(void *unused) {
    OSMesgQueue queue;
    void *queueMessage;
    void *message;

    osCreateMesgQueue(&queue, &queueMessage, 1);
    for (;;) {
        osRecvMesg(&gPager.pending, &message, 1);
        if (*((PageJob *)message)->slot == 0xFF) {
            CLoader__LoadSync(&gLoader, ((PageJob *)message)->source, (((PageJob *)message)->size + 1) & ~1,
                          gVMPages + (((PageJob *)message)->mapping->index << 12));
            osInvalDCache(gVMPages + (((PageJob *)message)->mapping->index << 12), 0x1000);
            osInvalICache(gVMPages + (((PageJob *)message)->mapping->index << 12), 0x1000);
        }
        ((PageJob *)message)->type = 1;
        osSendMesg(&gPager.completion, message, 1);
    }
}


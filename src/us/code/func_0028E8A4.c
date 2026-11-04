/* Boundary fixed in versions/cuts.us.txt: splat's func_0028E7A4 (0x23C bytes) holds three functions:
 * func_0028E7A4 (0x100), the reader thread at 0x28E8A4 (splat's D_0028E8A4,
 * 0xE8) and an unlink at 0x28E98C (0x54). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_16.CPP, func_8023C924_de,
 * func_8023CA14_de and func_8023CB0C_de, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"


extern char D_80120DC0[];               /* the reader thread's stack; its OSThread is 0x230 below */
extern char D_800D1CD8[];               /* the cartridge reader */
extern char D_80132000[];               /* the page array, 0x1000 per page */

void func_00266C5C(void *dst, s32 value, s32 size);
void func_0029B060(void *thread, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
void func_0029BB10(void *thread);
void func_0029B030(OSMesgQueue *queue, void **msg, s32 count);
s32 osRecvMesg(OSMesgQueue *queue, void **msg, s32 block);
s32 osSendMesg(OSMesgQueue *queue, void *msg, s32 block);
void func_00201DBC(void *reader, u8 *source, s32 size, void *dest);
void osInvalDCache(void *addr, s32 size);
void osInvalICache(void *addr, s32 size);
void func_0028E068(void);
void func_0028ED10(PageJob *job);
void func_0028EB54(PageJob *job);
void func_0028EC54(PageJob *job);
void func_0028E8A4(void *unused);

/* Reader thread: reads each pending page from the cartridge into its slot of
 * the page array and hands the job back to the manager. */
void func_0028E8A4(void *unused) {
    OSMesgQueue queue;
    void *queueMessage;
    void *message;

    func_0029B030(&queue, &queueMessage, 1);
    for (;;) {
        osRecvMesg(&D_801206A0.pending, &message, 1);
        if (*((PageJob *)message)->slot == 0xFF) {
            func_00201DBC(D_800D1CD8, ((PageJob *)message)->source, (((PageJob *)message)->size + 1) & ~1,
                          D_80132000 + (((PageJob *)message)->mapping->index << 12));
            osInvalDCache(D_80132000 + (((PageJob *)message)->mapping->index << 12), 0x1000);
            osInvalICache(D_80132000 + (((PageJob *)message)->mapping->index << 12), 0x1000);
        }
        ((PageJob *)message)->type = 1;
        osSendMesg(&D_801206A0.completion, message, 1);
    }
}


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

/* Resource manager thread: starts the reader thread, then dispatches requests forever. */
void func_0028E7A4(void) {
    void *request;

    func_00266C5C(D_80120DC0, 0x62, 0x200);
    func_0029B060(D_80120DC0 - 0x230, 0x62, func_0028E8A4, NULL, D_80120DC0 + 0x200, 0x94);
    func_0029BB10(D_80120DC0 - 0x230);
    for (;;) {
        osRecvMesg(&D_801206A0.completion, &request, 1);
        switch (((PageJob *)request)->type) {
            case 0:
                func_0028E068();
                break;
            case 1:
                func_0028ED10(request);
                break;
            case 3:
                func_0028EC54(request);
                break;
            case 2:
                func_0028EB54(request);
                break;
        }
    }
}


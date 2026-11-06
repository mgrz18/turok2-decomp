/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00256CB0.c, func_00256CB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
OSTime osGetTime(void);
u32 osAiGetStatus(void);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern u32 D_800B2660;
extern s32 D_800B266C;
extern u32 D_800B2670;
extern u8 D_8011AD90[];
extern OSMesgQueue D_800F27A0;
extern s32 D_800F27C8[];
extern u8 D_800F27D4[];
typedef struct B12Timer2 {
    struct B12Timer2 *next;
    u8 pad4[4];
    u16 count;
} B12Timer2;
extern B12Timer2 *D_800F279C;
void osScAddClient(void *a, void *b, void *c);
void __amHandleFrameMsg(s32 buf, s32 z);
void alClose(void *g);
void __amMain(void) {
    s32 sp10[2];
    u32 msg;
    u32 cnt;
    s32 arg;
    s32 done;
    OSTime last;
    OSTime now;
    B12Timer2 *p;
    cnt = arg = done = 0;
    last = 0;
    osScAddClient(D_8011AD90, sp10, &D_800F27A0);
    do {
        now = osGetTime();
        D_800B2670 = (u32)(((now - last) << 6) / 3000);
        osRecvMesg(&D_800F27A0, (OSMesg *)&msg, 1);
        last = osGetTime();
        switch (msg) {
        case 4:
            break;
        case 1:
            cnt++;
            if (cnt >= 0x15) {
                cnt = 0;
        case 7:
                D_800B266C++;
                while (!(osAiGetStatus() & 0x80000000)) {
                    __amHandleFrameMsg(D_800F27C8[D_800B2660 % 3], arg);
                }
                for (p = D_800F279C; p; p = p->next) {
                    if (p->count) {
                        p->count--;
                    }
                }
            }
            break;
        }
    } while (done == 0);
    alClose(D_800F27D4);
}

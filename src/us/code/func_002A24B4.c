/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002016FC.c, func_002016FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
typedef u32 OSEvent;
void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg);
void osStartThread(OSThread *thread);
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
extern volatile s32 D_800BC300;
extern OSMesg D_801270B4[];
extern OSMesgQueue D_801270C0;
extern OSMesg D_801280E0[];
extern OSMesg D_801280E4[];
extern u8 D_801280DC[];
extern OSMesgQueue D_801280F0;
extern OSMesgQueue D_80128370;
extern OSMesg D_80128388[];
extern OSThread D_80128390;
extern OSMesgQueue D_80128540;
void HostIo_ThreadMain(void *arg);
void HostIo_StartThread(s32 pri) {
    if (D_800BC300 == 0) {
        D_800BC300 = 1;
        osCreateMesgQueue(&D_801280F0, D_801280E4, 1);
        osCreateMesgQueue(&D_80128370, D_801280E0, 1);
        osCreateMesgQueue(&D_80128540, D_80128388, 1);
        osCreateMesgQueue(&D_801270C0, D_801270B4, 1);
        osSetEventMesg(0, &D_801270C0, (OSMesg)1);
        osCreateThread(&D_80128390, 0x4653, HostIo_ThreadMain, 0, D_801280DC + 4, pri);
        osStartThread(&D_80128390);
    }
}

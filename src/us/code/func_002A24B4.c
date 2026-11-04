/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002016FC.c, func_002016FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "ultra_types.h"
typedef u32 OSEvent;
void func_0029B950(OSEvent e, OSMesgQueue *mq, OSMesg msg);
void func_0029BB10(OSThread *thread);
void func_0029B060(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void func_0029B030(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
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
void D_002A2650(void *arg);
void func_002A24B4(s32 pri) {
    if (D_800BC300 == 0) {
        D_800BC300 = 1;
        func_0029B030(&D_801280F0, D_801280E4, 1);
        func_0029B030(&D_80128370, D_801280E0, 1);
        func_0029B030(&D_80128540, D_80128388, 1);
        func_0029B030(&D_801270C0, D_801270B4, 1);
        func_0029B950(0, &D_801270C0, (OSMesg)1);
        func_0029B060(&D_80128390, 0x4653, D_002A2650, 0, D_801280DC + 4, pri);
        func_0029BB10(&D_80128390);
    }
}

#include "common.h"

extern void func_00275904(s32 volume);
extern void func_00275A98(void);
extern s32 func_002759B4(void);
extern void func_002759C4(s32 track);
extern void func_00275A74(void);
extern void func_0027598C(s32 on);
extern void func_00285CC4(void);
extern s32 func_00416894(s32, s32, s32);
extern u8 D_004394B4[];   /* menu template */
extern s32 D_800C1F70;    /* saved music track */
extern s32 D_800C1F74;
extern s32 D_800C1F78;

/* Opens the menu D_004394B4, switching the music (restored by func_00419BCC). */
s32 func_0041A140(s32 arg0, s32 arg1, s32 arg2) {
    func_00275904(300);
    func_00275A98();
    D_800C1F70 = func_002759B4();
    func_002759C4(D_800C1F78);
    func_00275A74();
    func_0027598C(1);
    D_800C1F74 = 1;
    func_00285CC4();
    func_00416894(arg2, arg1, (s32) D_004394B4);
    return 1;
}

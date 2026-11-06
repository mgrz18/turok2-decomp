/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_8.c, func_80264104_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void RumblePool_FreeFinished(void *arg0);
extern int gScene_m_WarpState;
extern u32 osMotorStop(struct Triple *);
extern float D_800A9CF8;
extern f32 Rumble_GetLevelAt(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 gRumblePool;
extern s32 gEngineApp_m_Settings_paused;
extern void RumblePool_FreeAll(s32 *);
extern unsigned int osMotorStart(void *arg0);
void Controller_UpdateRumble(ControllerProfile *arg0) {
    Triple zero;
    void *object;
    s32 *global;
    f32 first;
    if (arg0->rumblePak != 0) {
        zero.x = 0;
        zero.y = 0;
        zero.z = 0;
        if (arg0->rumbleOn == 0) {
            goto reset;
        }
        global = &gEngineApp_m_Settings_paused;
        if ((*global != 0) || (gScene_m_WarpState != 0) || (global[-6] != 13)) {
reset:
            arg0->rumbleLevel = 0.0f;
            RumblePool_FreeAll(arg0->rumblePool);
        } else {
            object = arg0->rumblePool;
            RumblePool_FreeFinished(object);
            first = Rumble_GetLevelAt(&gRumblePool, arg0->unk8, arg0->unkC, arg0->unk10);
            arg0->rumbleLevel = first + Rumble_GetLevelAt(object, zero.x, zero.y, zero.z);
        }
        if (arg0->rumblePak != 0) {
            if (arg0->rumbleLevel != 0.0f) {
                arg0->rumbleTimer += arg0->rumbleLevel;
                if (arg0->rumbleTimer >= D_800A9CF8) {
                    arg0->rumbleTimer -= D_800A9CF8;
osMotorStart
((void *)arg0->rumblePfs);
                    return;
                }
            }
((unsigned int (*)(void *))osMotorStop)
((void *)arg0->rumblePfs);
        }
    }
}

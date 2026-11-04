#include "common.h"
#include "structs/game.h"

s32 func_002759C4(s32);
s32 func_00284080(void);
s32 func_00285410(s32);
s32 func_0042F514(void);

s32 func_00275904(s32);                     /* extern */
s32 func_00275A08();                            /* extern */
extern s32 D_800B6D78;
extern s32 D_8011B110;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table D_800B6D78/D_800B6D7C has 0xC-byte rows indexed by mode). */

void func_00289DFC(CEngineApp *arg0) {
    s32 (*temp_v0)(CEngineApp *);
    s32 temp_v1;

    if ((D_8011B110 == 2) && (arg0->m_Settings.unk1C8 == 0)) {
        arg0->m_Settings.modeTimer = 0;
        arg0->m_Settings.mode = arg0->m_Settings.nextMode;
        func_00284080();
        func_00275904(-1);
        temp_v1 = arg0->m_Settings.nextMode;
        if ((temp_v1 != 8) && (temp_v1 != 0xC)) {
            if ((arg0->m_Settings.prevMode != 8) || (arg0->m_Settings.mode != 0xD)) {
                func_002759C4(-1);
                func_00275A08();
                goto block_7;
            }
        } else {
block_7:
            if (arg0->m_Settings.mode != 0xD) {
                func_0042F514();
            }
        }
        func_00285410(0);
        temp_v0 = *(((__typeof__(&D_800B6D78))((s8 *)&D_800B6D78 + (arg0->m_Settings.mode * 0xC))));
        if (temp_v0 != NULL) {
            temp_v0(arg0);
        }
    }
}

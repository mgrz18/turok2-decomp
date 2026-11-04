#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern f32 func_002119FC(f32 lo, f32 hi);
extern s32 func_0026D518(s32 id);
extern void func_00275544(s16 sound, CVector3 pos, s32 arg2, CGameObjectInstance *source);


/* Run the current step of the third script (sounds, jumps, waits). */
s32 func_004053C4(CGameObjectInstance *obj) {
    s32 sound;
    s32 i;

    if (D_8012FA1C.timer > 0.0f) {
        return 0;
    }
    if (D_8012FA1C.steps == NULL) {
        return 0;
    }
    switch (D_8012FA1C.steps[D_8012FA1C.current].type) {
        case 1:
            if (D_8012FA1C.steps[D_8012FA1C.current].arg != -1) {
                func_00275544(D_8012FA1C.steps[D_8012FA1C.current].arg, obj->header.m_vPos, 0, obj);
            }
            D_8012FA1C.current = -1;
            return 1;
        case 2:
            sound = (s16) func_0026D518(D_8012FA1C.steps[D_8012FA1C.current].arg);
            if (sound != -1) {
                func_00275544(sound, obj->header.m_vPos, 0, obj);
            }
            D_8012FA1C.current = -1;
            return 1;
        case 3:
            D_8012FA1C.steps = (BossStep3 *) D_8012FA1C.steps[D_8012FA1C.current].arg;
            D_8012FA1C.cursor = -1;
            D_8012FA1C.current = -1;
            return 1;
        case 5:
            D_8012FA1C.current = -1;
            return 1;
        case 6:
            D_8012FA1C.timer = D_8012FA1C.steps[D_8012FA1C.cursor].delay
                             + func_002119FC(0.0f, D_8012FA1C.steps[D_8012FA1C.cursor].delayRange) * 15.0f;
            for (i = 0; i < D_8012FA1C.steps[D_8012FA1C.current].arg; i++) {
                D_8012FA1C.cursor++;
                if (D_8012FA1C.steps[D_8012FA1C.cursor].type == 4 && i < D_8012FA1C.steps[D_8012FA1C.current].arg - 1) {
                    D_8012FA1C.cursor = 0;
                }
                D_8012FA1C.current = D_8012FA1C.cursor;
            }
            if (D_8012FA1C.steps[D_8012FA1C.cursor].type == 4) {
                D_8012FA1C.cursor = -1;
                D_8012FA1C.current = -1;
                D_8012FA1C.steps = NULL;
                return 1;
            }
            return 0;
        case 4:
            break;
    }
    return 0;
}

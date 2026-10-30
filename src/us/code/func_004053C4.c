#include "common.h"
#include "structs/game_object.h"

extern f32 func_002119FC(f32 lo, f32 hi);
extern s32 func_0026D518(s32 id);
extern void func_00275544(s16 sound, CVector3 pos, s32 arg2, CGameObjectInstance *source);

/* One step of the boss's third script (0x14 bytes). */
typedef struct BossStep3 {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ s32 chance;
    /* 0x0C */ f32 delay;
    /* 0x10 */ f32 delayRange;
} BossStep3;

/* Boss module state from 0x8012FA1C. */
typedef struct {
    /* 0x00 */ f32 timer;         /* 0x8012FA1C */
    /* 0x04 */ BossStep3 *script; /* 0x8012FA20 */
    /* 0x08 */ s32 index;         /* 0x8012FA24 */
    /* 0x0C */ s32 current;       /* 0x8012FA28 */
} BossScript3;

extern BossScript3 D_8012FA1C;

/* Run the current step of the third script (sounds, jumps, waits). */
s32 func_004053C4(CGameObjectInstance *obj) {
    s32 sound;
    s32 i;

    if (D_8012FA1C.timer > 0.0f) {
        return 0;
    }
    if (D_8012FA1C.script == NULL) {
        return 0;
    }
    switch (D_8012FA1C.script[D_8012FA1C.current].type) {
        case 1:
            if (D_8012FA1C.script[D_8012FA1C.current].arg != -1) {
                func_00275544(D_8012FA1C.script[D_8012FA1C.current].arg, obj->header.m_vPos, 0, obj);
            }
            D_8012FA1C.current = -1;
            return 1;
        case 2:
            sound = (s16) func_0026D518(D_8012FA1C.script[D_8012FA1C.current].arg);
            if (sound != -1) {
                func_00275544(sound, obj->header.m_vPos, 0, obj);
            }
            D_8012FA1C.current = -1;
            return 1;
        case 3:
            D_8012FA1C.script = (BossStep3 *) D_8012FA1C.script[D_8012FA1C.current].arg;
            D_8012FA1C.index = -1;
            D_8012FA1C.current = -1;
            return 1;
        case 5:
            D_8012FA1C.current = -1;
            return 1;
        case 6:
            D_8012FA1C.timer = D_8012FA1C.script[D_8012FA1C.index].delay
                             + func_002119FC(0.0f, D_8012FA1C.script[D_8012FA1C.index].delayRange) * 15.0f;
            for (i = 0; i < D_8012FA1C.script[D_8012FA1C.current].arg; i++) {
                D_8012FA1C.index++;
                if (D_8012FA1C.script[D_8012FA1C.index].type == 4 && i < D_8012FA1C.script[D_8012FA1C.current].arg - 1) {
                    D_8012FA1C.index = 0;
                }
                D_8012FA1C.current = D_8012FA1C.index;
            }
            if (D_8012FA1C.script[D_8012FA1C.index].type == 4) {
                D_8012FA1C.index = -1;
                D_8012FA1C.current = -1;
                D_8012FA1C.script = NULL;
                return 1;
            }
            return 0;
        case 4:
            break;
    }
    return 0;
}

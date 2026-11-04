#include "common.h"

s32 func_0026D474(void *, void *);              /* extern */
extern f32 D_800B6D28;

/* A repeating timer: counts down by the frame time, fires its callback,
 * and is removed after its last repeat. */
typedef struct Timer002367A0 {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Timer002367A0 *next;
    /* 0x08 */ f32 time;
    /* 0x0C */ f32 period;
    /* 0x10 */ s32 repeats;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 (*callback)(void *);
} Timer002367A0;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7580];
    /* 0x7580 */ u8 timerList[0x758C - 0x7580];
    /* 0x758C */ Timer002367A0 *timerHead;
} World002367A0;

void func_002367A0(World002367A0 *arg0) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    Timer002367A0 *temp_s1;
    Timer002367A0 *var_s0;

    var_s0 = arg0->timerHead;
    if (var_s0 != NULL) {
        do {
            temp_f1 = var_s0->time - D_800B6D28;
            temp_s1 = var_s0->next;
            var_s0->time = temp_f1;
            if (temp_f1 <= 0.0f) {
                temp_f0 = temp_f1 + var_s0->period;
                var_s0->time = temp_f0;
                if (temp_f0 < 0.0f) {
                    var_s0->time = 0.0f;
                }
                var_s0->callback(var_s0);
                temp_v0 = var_s0->repeats - 1;
                var_s0->repeats = temp_v0;
                if (temp_v0 <= 0) {
                    func_0026D474(arg0->timerList, var_s0);
                }
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}

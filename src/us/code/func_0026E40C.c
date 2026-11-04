#include "common.h"

s32 func_002759C4(u16);

/* The mode parameter this AI callback is handed: only the field it reads. */
typedef struct AIModeParam {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 value;
} AIModeParam;                         /* extern */

void func_0026E40C(AIModeParam *arg0) {
    func_002759C4(arg0->value);
}

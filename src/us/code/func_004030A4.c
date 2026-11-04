#include "common.h"

s32 func_0026D518(s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 handle;
} Config;

extern Config *D_8012F5B4;

s16 func_004030A4(void) {
    return func_0026D518(D_8012F5B4->handle);
}

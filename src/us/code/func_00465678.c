#include "common.h"

typedef struct LevelLights {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 flags;
} LevelLights;

typedef struct {
    /* 0x00000 */ u8 pad0[0x18E50];
    /* 0x18E50 */ LevelLights *lights;
} World;

s32 func_0042A9A0();                            /* extern */
extern s32 D_800AB8A0;

void func_00465678(World *arg0) {
    func_0042A9A0();
    D_800AB8A0 = ((u8) arg0->lights->flags >> 2) & 1;
}

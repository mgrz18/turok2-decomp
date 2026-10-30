/* WRONG CUT: REAL func_0042AA80 size 0x18C (splat cut it into func_0042AA80 + func_0042AB2C). */
#include "common.h"

/* The level's light settings (colours as bytes, direction as signed bytes). */
typedef struct LevelLights {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u8 color[3];
    /* 0x0D */ u8 ambient[3];
    /* 0x10 */ u8 flags;
    /* 0x11 */ s8 dir[3];
} LevelLights;

typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18E50];
    /* 0x18E50 */ LevelLights *lights;
    /* 0x18E54 */ s16 dir[3];
} MenuState;

/* A Lights1 display-list block. */
typedef struct SceneLight {
    /* 0x00 */ u8 ambCol[4];
    /* 0x04 */ u8 ambColc[4];
    /* 0x08 */ u8 col[4];
    /* 0x0C */ u8 colc[4];
    /* 0x10 */ s8 dir[4];
} SceneLight;

extern SceneLight D_800AB0D0;

extern f32 func_00298470(f32);
extern void func_0020EF60(f32 *out, f32 *in, f32 scale);

/* Loads the level's light colours into the scene light and its direction, normalised to 127, into the menu. */
void func_0042AA80(MenuState *menu) {
    s32 i;
    f32 dir[3];
    f32 len;

    for (i = 0; i < 3; i++) {
        D_800AB0D0.ambColc[i] = D_800AB0D0.ambCol[i] = menu->lights->ambient[i];
        D_800AB0D0.colc[i] = D_800AB0D0.col[i] = menu->lights->color[i];
        D_800AB0D0.dir[i] = menu->lights->dir[i];
    }
    dir[0] = menu->lights->dir[0];
    dir[1] = menu->lights->dir[1];
    dir[2] = menu->lights->dir[2];
    len = func_00298470(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    if (len != 0.0f) {
        func_0020EF60(dir, dir, 127.0f / len);
        menu->dir[0] = dir[0];
        menu->dir[1] = dir[1];
        menu->dir[2] = dir[2];
    }
}

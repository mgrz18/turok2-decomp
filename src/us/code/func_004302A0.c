/* WRONG CUT: the real function is func_004302A0, size 0x90 (splat split it at func_004302CC). */
#include "common.h"

typedef struct QuadVtx {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} QuadVtx;

extern QuadVtx D_8011AD48[4];

/* Builds a unit quad in the XZ plane, white, textured 32x32 (S10.5 texture coordinates). */
void func_004302A0(void) {
    s32 i;
    QuadVtx *v = D_8011AD48;

    v[0].ob[0] = -1;
    v[0].ob[2] = 1;
    v[0].tc[0] = 0;
    v[0].tc[1] = 0;
    v[1].ob[0] = 1;
    v[1].ob[2] = 1;
    v[1].tc[0] = 0x400;
    v[1].tc[1] = 0;
    v[2].ob[0] = 1;
    v[2].ob[2] = -1;
    v[2].tc[0] = 0x400;
    v[2].tc[1] = 0x400;
    v[3].ob[0] = -1;
    v[3].ob[2] = -1;
    v[3].tc[0] = 0;
    v[3].tc[1] = 0x400;
    for (i = 0; i < 4; i++) {
        v[i].ob[1] = 0;
        v[i].flag = 0;
        v[i].cn[0] = 0xFF;
        v[i].cn[1] = 0xFF;
        v[i].cn[2] = 0xFF;
        v[i].cn[3] = 0xFF;
    }
}

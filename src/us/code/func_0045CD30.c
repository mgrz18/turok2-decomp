/* Sets the video mode for the TV type and memory size: screen size, VI features. The mode record is read as an s32 array (an in-struct read changes GCC's alias ordering). */
#include "common.h"

/* One screen setting for a TV type and memory size (0x1C bytes). */
typedef struct {
    /* 0x00 */ s32 width;
    /* 0x04 */ s32 height;
    /* 0x08 */ s32 arg2;
    /* 0x0C */ s32 arg3;
    /* 0x10 */ s32 arg4;
    /* 0x14 */ s32 arg5;
    /* 0x18 */ s32 pad18;
} ScreenMode;

typedef struct {
    /* 0x00 */ s32 antialias;
    /* 0x04 */ s32 v[4];
} ScreenExtra; /* 0x14 */

extern s32 D_80000300; /* osTvType */
extern s32 D_800C2030;
extern s32 D_800C2034;
extern s32 D_800C2038; /* current video mode */
extern s32 D_800C2044;
extern ScreenMode D_800C2054[];
extern ScreenMode D_800C20E0[];
extern ScreenExtra D_800C216C[];
extern s32 D_800AB890;
extern s32 D_800AB894;
extern s32 D_800AB898;
extern s32 D_800AB89C;
extern s32 D_801198E8;

s32 func_00266B80(void); /* osGetMemSize */
void func_0029AAD0(u32 features); /* osViSetSpecialFeatures */
void func_0042456C(s32 width, s32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void func_0045CD30(void) {
    s32 *mode = NULL;
    ScreenExtra extra;

    D_801198E8 = 2;
    if (D_800C2038 == -1) {
        if (func_00266B80() != 0x400000) {
            D_800C2038 = 1;
        } else {
            D_800C2038 = 0;
        }
    }
    switch (D_80000300) {
        case 1:
            mode = (s32 *)&D_800C2054[D_800C2038];
            break;
        case 0:
        case 2:
            mode = (s32 *)&D_800C20E0[D_800C2038];
            break;
    }
    if (mode != NULL) {
        extra = D_800C216C[D_800C2038];
        D_800C2044 = extra.antialias;
        D_800AB890 = extra.v[0];
        D_800AB894 = extra.v[1];
        D_800AB898 = extra.v[2];
        D_800AB89C = extra.v[3];
        D_800C2030 = mode[0];
        D_800C2034 = mode[1];
        func_0042456C(mode[0], mode[1], mode[2], mode[3], mode[4], mode[5]);
        func_0029AAD0(D_800C2044 ? 0x40 : 0x80);
        func_0029AAD0(0x20);
        func_0029AAD0(2);
    }
}

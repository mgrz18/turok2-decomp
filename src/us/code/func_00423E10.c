#include "common.h"
#include "structs/frame.h"

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FRONTEND/8040B45C_21.CPP, func_8040BBB0_de, with Turok 2's layouts. */
/* Video setup: picks the low or high resolution set (high when an expansion pak is present, decided once), then
 * the mode for the TV type, copies its flags to the display globals and starts the display. */

typedef struct {
    s32 a, b, c, d, e;
} VideoFlags;

extern s32 D_80000300; /* osTvType */
extern s32 D_801198E8;
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */
extern s32 D_800C2038; /* resolution set, -1 until decided */
extern s32 D_800C2044;
extern s32 D_800AB890;
extern s32 D_800AB894;
extern s32 D_800AB898;
extern s32 D_800AB89C;
extern VideoFlags D_800C216C[];
extern s32 func_00266B80(void); /* osGetMemSize */
extern void func_0042464C(s32 width, s32 height, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_0029AAD0(s32 flags);

static __inline__ void SetConfig(s32 a, s32 b, s32 c, s32 d, s32 e) {
    D_800C2044 = a;
    D_800AB890 = b;
    D_800AB894 = c;
    D_800AB898 = d;
    D_800AB89C = e;
}

static __inline__ void SetMode(s32 *mode) {
    D_800C2030 = mode[0];
    D_800C2034 = mode[1];
    func_0042464C(mode[0], mode[1], mode[2], mode[3], mode[4], mode[5]);
}

void func_00423E10(void) {
    VideoMode *mode = NULL;
    s32 flag;

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
            mode = &D_800C2054[0][D_800C2038];
            break;
        case 0:
        case 2:
            mode = &D_800C20E0[D_800C2038];
            break;
    }
    if (mode != NULL) {
        VideoFlags flags = D_800C216C[D_800C2038];

        SetConfig(flags.a, flags.b, flags.c, flags.d, flags.e);
        SetMode((s32 *)mode);
        flag = 0x80;
        if (D_800C2044 != 0) {
            flag = 0x40;
        }
        func_0029AAD0(flag);
        func_0029AAD0(0x20);
        func_0029AAD0(2);
    }
}

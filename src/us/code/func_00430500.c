/* WRONG CUT: the real function is func_00430500, size 0x2A8 (0x430500-0x4307A8): it runs
 * past splat's end of virtual_text_0 (ROM 0x17A600) into what the yaml calls virtual_rodata_0. */
#include "common.h"

typedef struct LineVtx {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} LineVtx;

typedef struct LineState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 counts[11];
} LineState;

/* Two (double-buffered) sets of debug vertex buffers, one colour each. */
extern LineVtx D_80129A40[2][0x200]; /* green */
extern LineVtx D_8012DA40[2][8];     /* magenta */
extern LineVtx D_8012DB40[2][0xC0];  /* red */
extern LineVtx D_8012F340[2][3];     /* yellow */

#define SET_VTX(v, r, g, b)  \
    (v).ob[0] = 0;          \
    (v).ob[1] = 0;          \
    (v).ob[2] = 0;          \
    (v).flag = 0;           \
    (v).tc[0] = 0;          \
    (v).tc[1] = 0;          \
    (v).cn[0] = (r);        \
    (v).cn[1] = (g);        \
    (v).cn[2] = (b);        \
    (v).cn[3] = 0xFF;

void func_00430500(LineState *s) {
    s32 i;
    s32 j;

    s->counts[0] = 0;
    s->counts[1] = 0;
    s->counts[2] = 0;
    s->counts[3] = 0;
    s->counts[4] = 0;
    s->counts[5] = 0;
    s->counts[6] = 0;
    s->counts[7] = 0;
    s->counts[8] = 0;
    s->counts[9] = 0;
    s->counts[10] = 0;
    s->unk0 = 0;
    s->unk4 = 0;
    s->unk8 = 0;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 0x200; i++) {
            SET_VTX(D_80129A40[j][i], 0, 0xC8, 0);
        }
        for (i = 0; i < 3; i++) {
            SET_VTX(D_8012F340[j][i], 0xC8, 0xC8, 0);
        }
        for (i = 0; i < 0xC0; i++) {
            SET_VTX(D_8012DB40[j][i], 0xC8, 0, 0);
        }
        for (i = 0; i < 8; i++) {
            SET_VTX(D_8012DA40[j][i], 0xC8, 0, 0xC8);
        }
    }
}

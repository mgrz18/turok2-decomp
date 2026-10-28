/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002576AC.c, func_002576AC, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x0000 */ u8 pad0[0x2B48];
    /* 0x2B48 */ void *root;        /* sound data file */
    /* 0x2B4C */ u32 *tree;
    /* 0x2B50 */ void *node2B50;
    /* 0x2B54 */ s32 unk2B54;
    /* 0x2B58 */ void *unk2B58;
    /* 0x2B5C */ void *unk2B5C;
    /* 0x2B60 */ void *unk2B60;
    /* 0x2B64 */ s32 unk2B64;
    /* 0x2B68 */ u8 pad2B68[8];
    /* 0x2B70 */ u8 *tab70;
    /* 0x2B74 */ u8 *tab74;
    /* 0x2B78 */ u8 *tab78;
    /* 0x2B7C */ s32 unk2B7C;
    /* 0x2B80 */ s32 unk2B80;
    /* 0x2B84 */ s32 unk2B84;
} SndMgr;

extern void *func_0028E60C(s32 a, s32 b, s32 c);
extern u32 *func_002017D4(void *table, s32 index);   /* entry of a packed table */

/* Sound manager: find the sound tables in the sound data file. */
void func_00270550(SndMgr *m) {
    u32 *s3;
    u32 *t0;
    u32 *t1;
    u32 *t2;
    u32 *n3;
    u32 *n4;
    u32 *n5;
    u32 *r;
    u32 *z;

    m->root = func_0028E60C(0x3EA7A0, 0, 0xBE1814);
    s3 = func_002017D4(m->root, 0);
    func_002017D4(m->root, 1);
    do {
        t0 = func_002017D4(s3, 0);
        t1 = func_002017D4(s3, 1);
        t2 = func_002017D4(s3, 2);
        m->tree = t2;
        m->unk2B54 = t2[0];
        m->unk2B58 = (u8 *)t0 + 8;
        m->unk2B5C = (u8 *)t1 + 8;
        n3 = func_002017D4(s3, 3);
        n4 = func_002017D4(s3, 4);
        n5 = func_002017D4(s3, 5);
        m->unk2B7C = n3[1];
        m->tab70 = (u8 *)n3 + 8;
        m->unk2B80 = n4[1];
        m->tab74 = (u8 *)n4 + 8;
        m->unk2B84 = n5[1];
        m->tab78 = (u8 *)n5 + 8;
        r = func_002017D4(m->root, 1);
        z = func_002017D4(r, 0);
        m->unk2B60 = (u8 *)z + 8;
        m->unk2B64 = z[1];
        m->node2B50 = func_002017D4(r, 1);
    } while (0);
}

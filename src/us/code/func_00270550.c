/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002576AC.c, func_002576AC, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"

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

    m->soundData = func_0028E60C(0x3EA7A0, 0, 0xBE1814);
    s3 = func_002017D4(m->soundData, 0);
    func_002017D4(m->soundData, 1);
    do {
        t0 = func_002017D4(s3, 0);
        t1 = func_002017D4(s3, 1);
        t2 = func_002017D4(s3, 2);
        m->variants = t2;
        m->rangeCount = t2[0];
        m->ranges = (u8 *)t0 + 8;
        m->weights = (u8 *)t1 + 8;
        n3 = func_002017D4(s3, 3);
        n4 = func_002017D4(s3, 4);
        n5 = func_002017D4(s3, 5);
        m->count2B70 = n3[1];
        m->table2B70 = (u8 *)n3 + 8;
        m->count2B74 = n4[1];
        m->table2B74 = (u8 *)n4 + 8;
        m->count2B78 = n5[1];
        m->table2B78 = (u8 *)n5 + 8;
        r = func_002017D4(m->soundData, 1);
        z = func_002017D4(r, 0);
        m->table2B60 = (u8 *)z + 8;
        m->count2B64 = z[1];
        m->node2B50 = func_002017D4(r, 1);
    } while (0);
}

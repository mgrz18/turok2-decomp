/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/CPlayer__PickLandingAnim.c, CPlayer__PickLandingAnim, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"
#include "protos/iset.h"

extern void *VM_Alloc(s32 a, s32 b, s32 c);

/* Sound manager: find the sound tables in the sound data file. */
void CAudioWorld__LoadSfxDataFile(CAudioWorld *m) {
    u32 *s3;
    u32 *t0;
    u32 *t1;
    u32 *t2;
    u32 *n3;
    u32 *n4;
    u32 *n5;
    u32 *r;
    u32 *z;

    m->soundData = VM_Alloc(0x3EA7A0, 0, 0xBE1814);
    s3 = CISet__GetBlock(m->soundData, 0);
    CISet__GetBlock(m->soundData, 1);
    do {
        t0 = CISet__GetBlock(s3, 0);
        t1 = CISet__GetBlock(s3, 1);
        t2 = CISet__GetBlock(s3, 2);
        m->variants = t2;
        m->rangeCount = t2[0];
        m->ranges = (u8 *)t0 + 8;
        m->weights = (u8 *)t1 + 8;
        n3 = CISet__GetBlock(s3, 3);
        n4 = CISet__GetBlock(s3, 4);
        n5 = CISet__GetBlock(s3, 5);
        m->count2B70 = n3[1];
        m->table2B70 = (u8 *)n3 + 8;
        m->count2B74 = n4[1];
        m->table2B74 = (u8 *)n4 + 8;
        m->count2B78 = n5[1];
        m->table2B78 = (u8 *)n5 + 8;
        r = CISet__GetBlock(m->soundData, 1);
        z = CISet__GetBlock(r, 0);
        m->table2B60 = (u8 *)z + 8;
        m->count2B64 = z[1];
        m->node2B50 = CISet__GetBlock(r, 1);
    } while (0);
}

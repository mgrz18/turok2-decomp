#ifdef TARGET_PORT
/* The port: the view read the scene's bit sections and level at the N64 offsets 0x6C/0x18FA8 and the
 * actor's killed index at 0x16E; typed on CScene and CLevelActor at their host offsets. Collecting a
 * level pickup (CPlayer__TouchPickup) crashed here. */
#include "common.h"
#include "structs/world.h"
#include "structs/level_actor.h"
#include "protos/iset.h"
void CScene__SetLevelActorKilled(CScene *o, CLevelActor *e, s32 set) {
    s32 bit = e->m_nKilledIndex;
    s32 idx = o->m_nLevel;
    sptr n = (sptr) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(o->m_pBitSections, 0), idx), 1);
    u8 *bits;
    s32 m;

    CISet__GetBlock((void *) n, 0);
    CISet__GetBlockSize((s32 *) n, 1);
    bits = (u8 *) CISet__GetBlock((void *) n, 1);
    m = 1 << (bit & 7);
    if (set != 0) {
        bits[bit / 8] |= m;
    } else {
        bits[bit / 8] &= ~m;
    }
}
#else
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B874.c, func_0028B874.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/iset.h"
typedef struct { u8 pad0[0x16E]; u16 f16E; } P4EntB;
typedef struct { u8 pad0[0x6C]; s32 f6C; } P4OwnB;
void CScene__SetLevelActorKilled(P4OwnB *o, P4EntB *e, s32 set) {
    s32 bit = e->f16E;
    s32 idx = *(s32 *)((u8 *)o + 0x18FA8);
    sptr n = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(o->f6C, 0), idx), 1);
    CISet__GetBlock(n, 0);
    CISet__GetBlockSize(n, 1);
    {
        u8 *bits = (u8 *)CISet__GetBlock(n, 1);
        u8 *p2 = bits;
        s32 m = 1 << (bit & 7);
        if (set != 0) {
            bits[bit / 8] |= m;
        } else {
            p2[bit / 8] &= ~m;
        }
    }
}
#endif

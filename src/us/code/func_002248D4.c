/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B964.c, func_0028B964.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"

#ifdef TARGET_PORT
/* the port: the actor's killed index at its host offset (CLevelActor.m_nKilledIndex) */
#include "structs/level_actor.h"
typedef struct { u8 pad0[__builtin_offsetof(CLevelActor, m_nKilledIndex)]; u16 unk16E; } Src;
#else
typedef struct { u8 pad0[0x16E]; u16 unk16E; } Src;
#endif
s32 CScene__IsLevelActorKilled(CScene *lv, Src *s) {
    s32 bit = s->unk16E;
    s32 idx = lv->m_nLevel;
    s32 *t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(lv->m_pBitSections, 0), idx), 1);
    u8 *bits;
    s32 m;
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

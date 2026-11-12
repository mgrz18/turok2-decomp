#include "structs/vector.h"
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00268768.c, func_00268768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
typedef struct B15Blk8 {
    s16 b, a;   /* b (the sound) is the low half of word 0 */
    s32 c;
} B15Blk8;
#else
typedef struct B15Blk8 {
    s16 a, b;
    s32 c;
} B15Blk8;
#endif
typedef struct B15Def {
               s32 type;
               u8 pad4[0xA];
               s8 unkE;
} B15Def;
typedef struct B15Src {
               u8 type;
               u8 pad1[3];
               CVector3 pos;       /* 0x4 */
               u8 pad10[4];
               B15Def *def;     /* 0x14 */
               u8 pad18[0x68];
               void *unk80;     /* 0x80 (0xD0 in Rage Wars) */
} B15Src;
void Sound__PlayPitched(s16 id, CVector3 pos, s32 a, void *src, f32 vol);
s32 GameObject_GetFloorKind(void *a);
void func_00239254(B15Src *act, B15Src *src, s32 c, CVector3 pos, B15Blk8 k) {
    f32 vol = 1.0f;
    void *s = (void *)-1;
    if (gEngineApp_m_Settings.paused == 0 && GameObject_GetFloorKind(act) == k.c) {
        switch (src->type) {
        case 0:
            s = src->unk80;
            break;
        case 1:
            if (act->def->type == 1) {
                if (gEngineApp_m_Settings.cheatFlags & 0x40) {
                    vol = 2.5f;
                }
                if (gEngineApp_m_Settings.cheatFlags & 0x20) {
                    vol = 0.35f;
                }
            }
            break;
        }
        Sound__PlayPitched(k.b, pos, 0, s, vol);
    }
}

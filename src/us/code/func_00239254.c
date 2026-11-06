#include "structs/vector.h"
#include "common.h"
#include "structs/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00268768.c, func_00268768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B15Blk8 {
    s16 a, b;
    s32 c;
} B15Blk8;
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
extern f32 D_800A65F0;
extern f32 D_800A65F4;
extern f32 D_800A65F8;
void func_00239254(B15Src *act, B15Src *src, s32 c, CVector3 pos, B15Blk8 k) {
    f32 vol = D_800A65F0;
    void *s = (void *)-1;
    if (D_8011AAD8.paused == 0 && GameObject_GetFloorKind(act) == k.c) {
        switch (src->type) {
        case 0:
            s = src->unk80;
            break;
        case 1:
            if (act->def->type == 1) {
                if (D_8011AAD8.cheatFlags & 0x40) {
                    vol = D_800A65F4;
                }
                if (D_8011AAD8.cheatFlags & 0x20) {
                    vol = D_800A65F8;
                }
            }
            break;
        }
        Sound__PlayPitched(k.b, pos, 0, s, vol);
    }
}

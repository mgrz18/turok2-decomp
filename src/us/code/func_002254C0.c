#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C6B0.c, func_0028C6B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Ent8 {
    s32 **tblp;
    s32 b;
} Ent8;
/* Turok 2 layout: count at 0x14A8, 8-byte entries from 0x14AC
 * (Rage Wars: count 0x1504, 12-byte entries from 0x1508). */
typedef struct Level {
    u8 pad0[0x14A8];
    s32 numTbls;            /* 0x14A8 */
    Ent8 tbls[16];          /* 0x14AC */
} Level;
typedef struct { CVector3 a; CVector3 b; } Pair;
void func_002228E4(Level *lv, CVector3 *a, u8 *buf, s32 *out);
void func_00222A9C(u8 *buf, s32 n, CVector3 *a, Pair *out);
extern Pair D_800AB0D0;
extern s32 D_800ACAE0;
s32 *func_002017D4(s32 *tbl, s32 idx);

/* A separate function in Rage Wars; inlined into this one in Turok 2. */
static inline s32 hasTbl(Level *lv) {
    s32 i;
    for (i = 0; i < lv->numTbls; i++) {
        if (func_002017D4(*lv->tbls[i].tblp, 2)[1] != 0) {
            return 1;
        }
    }
    return 0;
}
void func_002254C0(Level *lv, CVector3 *a1, Pair *out) {
    u8 buf[0x28];
    s32 n;
    if (hasTbl(lv) == 0 || D_800ACAE0 == 0) {
        *out = D_800AB0D0;
        return;
    }
    func_002228E4(lv, a1, buf, &n);
    func_00222A9C(buf, n, a1, out);
}

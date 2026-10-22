/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CF7C.c, func_0028CF7C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
/* Turok 2 level/game state: only the fields used here (offsets differ from Rage Wars). */
typedef struct Level {
    u8 pad0[0x64];
    s32 *unk64; /* 0x64 */
    u8 pad68[0x4];
    s32 *unk6C; /* 0x6C */
    s32 *unk70; /* 0x70 */
    u8 pad74[0x18FA8 - 0x74];
    s32 unk18FA8; /* 0x18FA8 */
} Level;
typedef struct { s32 key; u8 pad4[8]; s16 unkC; } Ent;
Ent *func_002017D4(s32 *tbl, s32 idx);
Ent *func_00225EBC(Level *lv, s32 a, s32 b) {
    s32 n = lv->unk64[0];
    s32 i = 0;
    Ent *e;
    if (n > 0) {
        do {
            e = func_002017D4(lv->unk64, i);
            if (a == -1 || e->key == a) {
                if (b == -1 || e->unkC == b) {
                    return e;
                }
            }
            i++;
        } while (i < n);
    }
    return 0;
}

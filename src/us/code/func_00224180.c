/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B1F8.c, func_0028B1F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

/* Turok 2 layout: the id table pointer is at 0x74 (0x94 in Rage Wars). */
typedef struct Level {
    u8 pad0[0x74];
    s32 *idTbl; /* 0x74: { ?, count, u16 ids[] } */
} Level;

s32 func_00224180(Level *lv, s32 id) {
    s32 *t = lv->idTbl;
    s32 n = t[1];
    u16 *p = (u16 *)(t + 2);
    s32 i = 0;
    if (n > 0) {
        do {
            if (*p == (u16)id) {
                return i;
            }
            i++;
            p++;
        } while (i < n);
    }
    return -1;
}

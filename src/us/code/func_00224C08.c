/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BC98.c, func_0028BC98.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
typedef unsigned char u8;
typedef signed int s32;
/* Turok 2 level record-table pointers sit earlier than in Rage Wars. */
typedef struct Level {
    u8 pad0[0x6C];
    s32 *unk6C;     /* 0x6C */
    u8 pad70[0x20];
    s32 *unk90;     /* 0x90 */
} Level;
s32 *func_002017D4(s32 *tbl, s32 idx);
s32 func_00224C08(Level *lv, s32 key) {
    s32 *t = lv->unk90;
    s32 n = t[1];
    s32 *ents = t + 2;
    s32 i;
    for (i = 0; i < n; i++) {
        if (ents[i] == key) {
            s32 *u = func_002017D4(lv->unk6C, 1);
            func_002017D4(u, 0);
            return *((u8 *)func_002017D4(u, 1) + i);
        }
    }
    return -1;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D7A0.c, func_0028D7A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef struct Rec14 {
    u8 pad0[0xE];
    u8 flags;
    u8 key;
    u8 pad10[0x4];
} Rec14;
/* Turok 2: these record tables sit 0x58 earlier than in Rage Wars. */
typedef struct Level {
    u8 pad0[0x1168];
    s32 cnt1168;        /* 0x1168 */
    s32 cnt116C;        /* 0x116C */
    s32 unk1170;        /* 0x1170 */
    s32 unk1174;        /* 0x1174 */
    Rec14 *recA;        /* 0x1178 */
    Rec14 *recB;        /* 0x117C */
} Level;
s32 func_00266D8C(s32 a, s32 b, s32 c, s32 *lo, s32 *hi);
void func_00226660(Level *lv, s32 which, s32 id, Rec14 **outRecs, s32 *outCount) {
    s32 lo;
    s32 hi;
    s32 a;
    s32 b;
    s32 off;
    if (which != 0) {
        a = lv->unk1174;
        b = lv->cnt116C;
    } else {
        a = lv->unk1170;
        b = lv->cnt1168;
    }
    if (func_00266D8C(a, b, id, &lo, &hi) != 0) {
        off = lo * 0x14;
        if (which != 0) {
            *outRecs = (Rec14 *)((u8 *)lv->recB + off);
        } else {
            *outRecs = (Rec14 *)((u8 *)lv->recA + off);
        }
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_6.CPP, func_8028D050_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
struct Container;
typedef struct Container Container;
struct RecordRecAtE4;
typedef struct RecordRecAtE4 RecordRecAtE4;
struct Sub18;
struct Sub18 {
    s32 unk0;
    char pad4[8];
    s16 unkC;
};
struct RecordRecAtE4;
struct Sub18;
/* Turok 2 LevelActor: def 0x14, type id 0xB8, size 0x288
 * (Rage Wars 0x18 / 0xE4 / 0x2E8). */
struct RecordRecAtE4 {
    char pad0[0x14];
    struct Sub18 *unk14;
    char pad18[0xA0];
    u16 unkB8;
    char padBA[0x288 - 0xBA];
};
struct Container;
struct RecordRecAtE4;
/* Turok 2: actor table at 0xE0 (Rage Wars 0x138). */
struct Container {
    char pad0[0xE0];
    struct RecordRecAtE4 *unkE0;
    s32 unkE4;
    s32 unkE8;
};
struct Sub18;
typedef struct Sub18 Sub18;
extern s32 func_00225F6C(Container *arg0, s32 key0, s32 key1, s32 key2, RecordRecAtE4 **out, s32 max);
s32 func_00225F6C(Container *arg0, s32 key0, s32 key1, s32 key2, RecordRecAtE4 **out, s32 max) {
    s32 i = arg0->unkE4;
    s32 end = arg0->unkE8;
    RecordRecAtE4 *base = arg0->unkE0;
    s32 count = 0;
    RecordRecAtE4 *rec;
    for (; i < end; i++) {
        rec = &base[i];
        if (count >= max) {
            return count;
        }
        if ((key0 == -1 || rec->unkB8 == key0) && (key1 == -1 || rec->unk14->unk0 == key1)
            && (key2 == -1 || rec->unk14->unkC == key2)) {
            *out++ = rec;
            count++;
        }
    }
    return count;
}

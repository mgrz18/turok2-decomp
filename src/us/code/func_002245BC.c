/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C.CPP, func_8028B670_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Def {
    s32 type;
    s32 flags;
};

/* Turok 2 level actor: 0x288 bytes (0x2E8 in Rage Wars). */
typedef struct LevelActor {
    u8 pad0[0x14];
    struct Def *definition; /* 0x14 */
    u8 pad18[0x288 - 0x18];
} LevelActor;

/* Minimal Turok 2 view of the world/scene object. */
typedef struct Shared_World {
    s32 mode;               /* 0x00 */
    u8 pad4[0x6C - 0x4];
    void *bitSections;      /* 0x6C */
    u8 pad70[0xE0 - 0x70];
    LevelActor *actors;     /* 0xE0 */
    u8 padE4[0x18FA8 - 0xE4];
    s32 level;              /* 0x18FA8 */
} Shared_World;

void *func_002017D4(void *table, s32 index);
extern void func_00201818(s32 arg0, s32 arg1);

void func_002245BC(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *entry;
    struct Def *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 mask;
    s32 idx;
    s32 idx2;
    if (((Shared_World *)arg0)->mode == 2) {
        return;
    }
    if (((Shared_World *)arg0)->level == arg1) {
        entry = (char *)((Shared_World *)arg0)->actors + (s32) arg2 * 0x288;
        cond = ((LevelActor *)entry)->definition;
        if ((u32) (cond->type - 9) < 2) {
            if (cond->flags & 0x200) {
                return;
            }
        }
    }
    field80 = ((Shared_World *)arg0)->bitSections;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(field80, 0), (s32) arg1), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    base1 = (u8 *) func_002017D4(temp_v0, 1);
    base2 = base1;
    mask = 1 << (arg2 & 7);
    if (arg3 != 0) {
        idx = arg2;
        if ((s32) arg2 < 0) {
            idx = arg2 + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = arg2;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}

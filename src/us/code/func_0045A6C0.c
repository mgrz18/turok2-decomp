/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_2.CPP, func_8040B700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct ObjRecAt5DC;
typedef struct ObjRecAt5DC ObjRecAt5DC;
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct ObjRecAt5DC;
struct ObjRecAt5DC {
    char pad0[0x5DC];
    char * unk5DC;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct Shared_PakMenu;
struct PakMenuSlot;
struct PakNotesMenuItem {
    char pad0[8];
    u32 flags;
    char padC[0x1C];
};
typedef struct Shared_PakMenu {
    s16 state;
    char pad02[0xA];
    struct PakNotesMenuItem *items;
    char pad10[4];
    char *dialogData;
    char pad18[4];
    ObjRecAt5DC *player;
    union {
        PakMenuSlot *owner;
        PakMenuSlot *slot;
    };
    void *unk24;
} Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 func_0041DCB0(Shared_PakMenu *action, s32 channel, s32 arg2);
extern s32 func_0041DCB0(Shared_PakMenu *, s32, s32);
extern s32 D_801309BC;
extern s32 func_0028591C(s32);
extern s32 func_0041DCB0(struct Shared_PakMenu *, s32, s32);
static __inline__ s32 func_80409BDC_de(struct Shared_PakMenu *record) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return record->owner->unk4;
}
s32 func_0045A6C0(struct Shared_PakMenu *record) {
    if (D_801309BC == 0) {
        if (func_80409BDC_de(record) != -1) {
            if (func_0028591C(func_80409BDC_de(record)) != 0) {
                if (func_0041DCB0(record, func_80409BDC_de(record), 0) != 0) {
                    D_801309BC = 1;
                }
            }
        }
    }
    return D_801309BC;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F2C_de.
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
struct PakMenuSlot;
struct PakMenuSlot;
struct Shared_PakMenu;
extern void func_004219E0(struct Shared_PakMenu *record);
struct Shared_PakMenu;
extern void func_00426480(s32);
void func_00426480(s32);
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct PakNotesMenuItem;
typedef struct PakNotesMenuItem PakNotesMenuItem;
struct PakMenuSlot;
struct PakNotesMenuItem;
extern s32 D_800C2028;
extern s32 D_80130990;
void func_004219E0(struct Shared_PakMenu *record) {
    s32 value;
    if (D_80130990 != 0) {
        value = D_800C2028;
    } else {
        value = record->owner->unk4;
    }
    func_00426480(value);
}
extern s32 D_80130990;

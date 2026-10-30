/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80405DC0_7.CPP, func_804066BC_de.
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
    char pad0[0x51C];
    char * unk51C;
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
void func_0041D948(Shared_PakMenu *menu);
extern s32 D_80130994;
extern s32 D_801309BC;
extern s32 D_80130984;
extern s32 D_801309B4;
extern s32 D_801309A0;
extern s32 D_800C2024;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
struct Shared_PakMenu;
extern char D_0043BB8C[];
extern char D_0043BB20[];
extern char D_0043874C[];
struct PakNotesMenuItem;
typedef struct PakNotesMenuItem PakNotesMenuItem;
struct PakMenuSlot;
struct PakNotesMenuItem;
extern s32 D_80130990;
extern s32 D_800C2020;
extern char D_80119E2C[];
extern char D_0043BF7C[];
extern char D_0043B5C8[];
extern void func_004263A0(s32 ch);
extern s32 func_0042647C(s32 ch);
extern s32 func_004264B4(s32 ch);
extern s32 func_00426AF8(s32 ch);
extern void func_00416644(char *, char *, ObjRecAt5DC *, PakMenuSlot *, char *);
void func_004570F8(Shared_PakMenu *menu) {
    s32 ch;
    D_80130984 = 0;
    D_801309A0 = 0;
    D_80130994 = 1;
    D_801309B4 = 1;
    D_800C2024 = 1;
    D_801309BC = 0;
    D_80130990 = 0;
    menu->items[20].flags &= ~0x01000000;
    ch = menu->slot->unk4;
    if (D_800C2020 != 0) {
        D_801309BC = 1;
        func_00416644(menu->player->unk51C + 0x53C, D_0043874C, menu->player, menu->slot, 0);
        return;
    }
    func_004263A0(ch);
    if (func_0042647C(ch) == 0) {
        func_0041D948(menu);
        return;
    }
    D_801309BC = 1;
    if (func_004264B4(ch) != 0) {
        func_00416644(D_80119E2C, D_0043BB8C, menu->player, menu->slot, D_0043874C);
    } else if (func_00426AF8(ch) != 0) {
        func_00416644(D_80119E2C, D_0043BF7C, menu->player, menu->slot, D_0043B5C8);
    } else {
        func_00416644(D_80119E2C, D_0043BB20, menu->player, menu->slot, D_0043874C);
    }
}

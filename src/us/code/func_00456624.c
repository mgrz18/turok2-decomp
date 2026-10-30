/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/804453C4_11.CPP, func_80445D6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct ObjRecAt5D8;
typedef struct ObjRecAt5D8 ObjRecAt5D8;
struct ObjRecAt5D8;
struct ObjRecAt5D8 {
    char pad0[0x5D8];
    void * unk5D8;
};
struct EntryRecAt1C;
struct EntryRecAt1C {
    char pad[0x1C];
    ObjRecAt5D8 *unk1C;
};
struct TextEntryRecAtC;
typedef struct TextEntryRecAtC TextEntryRecAtC;
struct TextEntryRecAtC;
struct TextEntryRecAtC {
    s32 length;
    s32 cursor;
    s32 count;
    char text[12];
};
struct EntryRecAt1C;
typedef struct EntryRecAt1C EntryRecAt1C;
extern TextEntryRecAtC D_800C1FB0[];
extern s32 func_0025340C(void *table, void *player);
extern u8 D_80119870[];
s32 func_00456624(s32 unused, EntryRecAt1C *arg1) {
    s32 i;
    s32 count;
    i = func_0025340C(D_80119870, arg1->unk1C);
    count = D_800C1FB0[i].count;
    D_800C1FB0[i].cursor = 0;
    if (count == 0) {
        return 0;
    }
    D_800C1FB0[i].count = count - 1;
    D_800C1FB0[i].length--;
    D_800C1FB0[i].text[D_800C1FB0[i].length] = 0;
    return 0;
}

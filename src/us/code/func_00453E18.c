/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_8.CPP, func_8043CF44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct ObjRecAt14V1;
typedef struct ObjRecAt14V1 ObjRecAt14V1;
struct ObjRecAt14V1;
struct ObjRecAt14V1 {
    char pad0[0x14];
    void * unk14;
};
struct EntryRecAt0V2;
struct EntryRecAt0V2 {
    void *ptr;
    s32 pad[3];
};
struct EntryRecAt0V2;
typedef struct EntryRecAt0V2 EntryRecAt0V2;
extern u32 sEnterCheatResult;
extern EntryRecAt0V2 sCheatCodes_unk4_2[];
extern char D_800C1144;
s32 EnterCheatMenu_ResultLabel_2(ObjRecAt14V1 *arg0) {
    if (sEnterCheatResult < 12) {
        arg0->unk14 = sCheatCodes_unk4_2[sEnterCheatResult].ptr;
    } else {
        arg0->unk14 = &D_800C1144;
    }
    return 0;
}

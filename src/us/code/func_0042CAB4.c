/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044D668_de.c, func_8044D804_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct GroupCode8044E2B8;
typedef struct GroupCode8044E2B8 GroupCode8044E2B8;
struct Piece;
typedef struct Piece Piece;
struct Piece;
struct Piece {
    char pad0[0x14];
    s32 tag;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
};
struct GroupCode8044E2B8;
struct GroupCode8044E2B8 {
    s32 unk0;
    s32 count;
    Piece pieces[1];
};
extern s32 func_00200B00(void ***, s32);
extern void *func_002017D4(void *, s32);
extern s32 func_00224F84(s32, s32);
void func_0042CAB4(s32 arg0, void ***arg1) {
    void *obj;
    GroupCode8044E2B8 *group;
    Piece *piece;
    s32 base;
    s32 count;
    s32 i;
    if (func_00200B00(arg1, 0) != 0) {
        obj = **arg1;
        group = func_002017D4(obj, 2);
        count = group->count;
        piece = group->pieces;
        i = 0;
        base = (s32) func_002017D4(obj, i);
        if (count > 0) {
            do {
                if (piece[i].tag == -1) {
                    piece[i].tag = 0;
                } else {
                    piece[i].tag = func_00224F84(arg0, piece[i].tag);
                }
                piece[i].unk18 = base + piece[i].unk18;
                piece[i].unk1C = base + piece[i].unk1C;
                piece[i].unk20 = base + piece[i].unk20;
                piece[i].unk24 = base + piece[i].unk24;
                piece[i].unk28 = base + piece[i].unk28;
                piece[i].unk2C = base + piece[i].unk2C;
                piece[i].unk30 = base + piece[i].unk30;
                piece[i].unk34 = base + piece[i].unk34;
                piece[i].unk38 = base + piece[i].unk38;
                i++;
            } while (i < count);
        }
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044D668_de.c, func_8044D804_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/iset.h"

extern s32 ProPackDecompress(void ***, s32);
extern s32 CScene__GetObject(s32, s32);
void CScene__DecompressParticles(s32 arg0, void ***arg1) {
    void *obj;
    GroupCode8044E2B8 *group;
    Piece *piece;
    s32 base;
    s32 count;
    s32 i;
    if (ProPackDecompress(arg1, 0) != 0) {
        obj = **arg1;
        group = CISet__GetBlock(obj, 2);
        count = group->count;
        piece = group->pieces;
        i = 0;
        base = (s32) CISet__GetBlock(obj, i);
        if (count > 0) {
            do {
                if (piece[i].tag == -1) {
                    piece[i].tag = 0;
                } else {
                    piece[i].tag = CScene__GetObject(arg0, piece[i].tag);
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

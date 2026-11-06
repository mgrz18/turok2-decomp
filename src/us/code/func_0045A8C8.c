/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409EF4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/pak.h"
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct PakMenuSlot;
struct RecordRecAt20V2;
struct PakMenuSlot;
struct RecordRecAt20V2 {
    char pad[0x1C];
    s32 result;
    struct PakMenuSlot *inner;
};
struct RecordRecAt20V2;
struct PakMenuSlot;
extern s32 gPakMenuPort;
extern s32 PakMenu_GetPlayerAndPort_2(struct RecordRecAt20V2 *record, s32 *out);
s32 PakMenu_GetPlayerAndPort_2(struct RecordRecAt20V2 *record, s32 *out) {
    s32 value;
    s32 result;
    if (gPakMenuAnyPak != 0) {
        value = gPakMenuPort;
        result = 0;
    } else {
        value = record->inner->unk4;
        result = record->result;
    }
    *out = value;
    return result;
}

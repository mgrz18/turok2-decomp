/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025BA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct SlotRecAtA8;
struct SlotRecAtA8 {
    char pad0[8];
    s32 used;
    char pad[0x9C];
    s32 value;
    char pad2[0x20];
};
struct HeaderRecAt102;
typedef struct HeaderRecAt102 HeaderRecAt102;
struct SlotRecAtA8;
typedef struct SlotRecAtA8 SlotRecAtA8;
struct HeaderRecAt102;
struct HeaderRecAt102 {
    char pad[0x102];
    s16 local;
};
struct RecordRecAt4V3;
struct RecordRecAt4V3 {
    HeaderRecAt102 *header;
    SlotRecAtA8 slots[17];
};
struct RecordRecAt4V3;
typedef struct RecordRecAt4V3 RecordRecAt4V3;
struct HeaderRecAt102;
struct HeaderRecAt102;
s32 func_00273D88(RecordRecAt4V3 *record, s32 value) {
    s32 i;
    SlotRecAtA8 *slot = record->slots;
    for (i = 0; i < 16; i++, slot++) {
        if (slot->used != -1 && record->header->local != i && slot->value == value) {
            return 1;
        }
    }
    return 0;
}

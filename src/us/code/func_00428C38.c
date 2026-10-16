/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_15.c, func_802B7AD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
struct EntryRecAt3;
struct EntryRecAt3 {
    u16 id;
    u8 team;
    u8 slot;
};
struct EntryRecAt3;
typedef struct EntryRecAt3 EntryRecAt3;
struct __OSContRequesFormat;
struct __OSContRequesFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
    u8 dummy1;
};
struct __OSContRequesFormat;
typedef struct __OSContRequesFormat __OSContRequesFormat;
void func_00428C38(u8 *valid, EntryRecAt3 *data) {
    extern char D_80130E20;
    extern u8 D_801213DC[];
    u8 *ptr;
    __OSContRequesFormat readformat;
    s32 i;
    u8 mask = 0;
    ptr = (u8 *)&D_80130E20;
    for (i = 0; i < D_801213DC[0]; i++, ptr += sizeof(readformat), data++) {
        readformat = *(__OSContRequesFormat *)ptr;
        data->slot = (readformat.rxsize & 0xC0) >> 4;
        if (data->slot != 0) {
            continue;
        }
        data->id = (readformat.typel << 8) | readformat.typeh;
        data->team = readformat.status;
        mask |= 1 << i;
    }
    *valid = mask;
}

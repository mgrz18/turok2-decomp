/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_8.c, func_80446EE4_de.
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
struct __OSContRequesFormatShort;
typedef struct __OSContRequesFormatShort __OSContRequesFormatShort;
struct __OSContRequesFormatShort;
struct __OSContRequesFormatShort {
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
};
extern void func_004280E4(int channel, EntryRecAt3 *data);
void func_004280E4(s32 channel, EntryRecAt3 *data) {
    extern char D_80130E20;
    u8 *ptr;
    __OSContRequesFormatShort requestformat;
    s32 i;
    ptr = (u8 *)&D_80130E20;
    for (i = 0; i < channel; i++) {
        ptr++;
    }
    requestformat = *(__OSContRequesFormatShort *)ptr;
    data->slot = (requestformat.rxsize & 0xC0) >> 4;
    if (data->slot == 0) {
        data->id = (requestformat.typel << 8) | requestformat.typeh;
        data->team = requestformat.status;
    }
}

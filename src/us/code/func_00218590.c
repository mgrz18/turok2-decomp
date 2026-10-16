/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de_2.c, func_802610F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Pack;
struct Pack;
struct Pack {
    s32 count;
    s32 offsets[1];
};
struct Pack;
typedef struct Pack Pack;
extern void func_0029E460(char *data);
char *func_00218590(Pack *pack, s32 prepare, s32 size)
{
    s32 i;
    char *data;
    for (i = 0; i <= pack->count; i++) {
        if (pack->offsets[i] == 0xDEADBEEF) {
            break;
        }
    }
    i--;
    pack->offsets[i + 1] = pack->offsets[i] + size;
    data = (char *)pack + pack->offsets[i];
    if (prepare) {
        func_0029E460(data);
    }
    return data;
}

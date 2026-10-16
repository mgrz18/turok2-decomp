/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_4.c, func_80275AA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Packed;
struct Packed {
    unsigned short unk0;
    unsigned short unk2;
    unsigned short first[3];
    unsigned short second[3];
    int unk10;
};
struct Packed;
typedef struct Packed Packed;
struct HeaderRecAt1C;
struct HeaderRecAt1C {
    short unk0;
    short unk2;
    void *first[3];
    void *second[3];
    int unk1C;
};
struct HeaderRecAt1C;
typedef struct HeaderRecAt1C HeaderRecAt1C;
extern void func_0026BB30(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra);
void func_0026BB30(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra) {
    int i;
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk1C = src->unk10;
    for (i = 0; i < 3; i++) {
        dst->first[i] = base + src->first[i] * 16;
        if (src->second[i] == 0xFFFF) {
            dst->second[i] = 0;
        } else {
            dst->second[i] = extra + src->second[i] * 32;
        }
    }
}

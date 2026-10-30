/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_4.CPP, func_80403E10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct RecordRecAt12;
typedef struct RecordRecAt12 RecordRecAt12;
struct RecordRecAt12;
struct RecordRecAt12 {
    char pad[0x11];
    unsigned char kind;
    unsigned char subtype;
};
int func_0044C1B0(RecordRecAt12 *rec) {
    int result = 0;
    switch (rec->kind) {
    case 1:
        if (rec->subtype == 5) {
            result = 1;
        }
        break;
    case 9:
        if (rec->subtype == 4) {
            result = 1;
        }
        break;
    case 10:
        if (rec->subtype == 3) {
            result = 1;
        }
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

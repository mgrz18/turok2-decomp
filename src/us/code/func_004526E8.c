/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EB70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct Flagged;
struct Flagged {
    u8 pad0[0x58];
    u32 unk58;
};
struct Flagged;
struct Obj8043ECE8;
struct Obj8043ECE8 {
    s16 unk0;
    u8 pad2[10];
    struct Flagged *unkC;
};
struct Obj8043ECE8;
typedef struct Obj8043ECE8 Obj8043ECE8;
extern s32 func_002859DC(void);
s32 func_004526E8(void *arg0, Obj8043ECE8 *arg1) {
    if (func_002859DC() < 2) {
        arg1->unkC->unk58 &= 0xFEFFFFFF;
        if (arg1->unk0 == 2) {
            arg1->unk0 = 1;
        }
    } else {
        arg1->unkC->unk58 |= 0x01000000;
    }
    return 0;
}

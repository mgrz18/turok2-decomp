/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E73C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef float f32;
struct FloatState3C;
typedef struct FloatState3C FloatState3C;
struct FloatState3C;
struct FloatState3C {
    unsigned char padding_0[56];
    f32 unk_38;
};
extern void func_0024599C(void *arg0, void *arg1, void *arg2);
f32 func_002687FC(u8 *arg0) {
    char buf[0x40];
    if (*arg0 == 1) {
        func_0024599C(arg0, arg0 + 0x140, buf);
        return ((struct FloatState3C *) buf)->unk_38;
    }
    return 0.0f;
}

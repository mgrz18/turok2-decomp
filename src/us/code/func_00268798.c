/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E6D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
extern void func_0024599C(void *arg0, void *arg1, void *arg2);
void func_00268798(u8 *arg0, void *arg1) {
    Triple buf;
    char pad[0x30];
    if (*arg0 == 1) {
        func_0024599C(arg0, arg0 + 0x140, &buf);
        *(Triple *)arg1 = buf;
        return;
    }
    *(s32 *)arg1 = 0;
    ((Triple *)arg1)->y = 0;
    ((Triple *)arg1)->z = 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_8.CPP, func_80405290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
extern s32 func_00285878(s32 arg0);
extern int D_80130C60[];
struct Device;
typedef struct Device Device;
struct Device;
struct Device {
    s8 pad0[0x68];
};
extern s8 D_800F6750;
extern void func_002847E0(void);
extern void func_002858A4(void);
void func_002847E0();
void func_002858A4();
void func_00425808(s32);
s32 func_00429050(void *);
extern Device D_80130C80[];
s32 func_0045F7F0(s32 arg0) {
    s32 var_s0;
    if (D_80130C60[arg0] != 3) {
        return -2;
    }
    ((void (*)(s32))func_00285878)(1);
    func_002847E0();
    D_800F6750 = 2;
    var_s0 = func_00429050(&D_80130C80[arg0]);
    if (var_s0 != 0) {
        var_s0 = -1;
    }
    func_00425808(arg0);
    func_002858A4();
    return var_s0;
}

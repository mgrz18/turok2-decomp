/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804051D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
extern s32 func_00285878(s32 arg0);
extern int D_80130C60[];
extern int D_80130C70[];
extern s8 D_800F6750;
extern void func_002847E0(void);
extern void func_002858A4(void);
void func_002847E0();
void func_002858A4();
void func_004258E8(s32);
s32 osPfsRepairId(void *);
s32 func_00426814(s32 arg0) {
    extern char D_80130C80[];
    s32 temp_s0;
    s32 var_s0;
    temp_s0 = arg0 * 4;
    if (D_80130C60[arg0] != 3) {
        return -2;
    }
    ((void (*)(s32))func_00285878)(1);
    func_002847E0();
    var_s0 = D_80130C70[arg0];
    D_800F6750 = 2;
    if (var_s0 == -4) {
        var_s0 = osPfsRepairId((arg0 * 0x68) + D_80130C80);
        if (var_s0 != 0) {
            var_s0 = -1;
        }
        func_004258E8(arg0);
    }
    func_002858A4();
    return var_s0;
}

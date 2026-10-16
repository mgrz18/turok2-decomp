/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_7.c, func_80263C24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern int func_00285A54(int arg0);
extern int D_800F6788[];
extern void func_00284CE0();
extern int func_00285980(int arg0);
extern s32 D_800F5EA8[];
extern s32 D_800F67B0[];
void func_00284CE0(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (func_00285A54(i) != 0) {
            if (func_00285980(i) != 0) {
                D_800F5EA8[i] = 1;
                if (D_800F67B0[i] != 0) {
                    D_800F6788[i] = 1;
                    D_800F67B0[i] = 0;
                }
            } else if (D_800F5EA8[i] != 0) {
                D_800F67B0[i] = 1;
            }
        } else {
            D_800F6788[i] = 0;
            D_800F67B0[i] = 0;
            D_800F5EA8[i] = 0;
        }
    }
}

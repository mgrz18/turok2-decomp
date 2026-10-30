/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409FA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern void func_00426898(s32,s32,s32 *),func_00426BA8(s32,void *,s32);
extern s32 func_00426C74(void *,void *);
s32 func_0045A974(void *unused, void *arg1, s32 arg2, s32 *arg3) {
    char sp10[16];
    s32 sp20;
    s32 var_s0;
    for(var_s0=0;var_s0<16;var_s0++) {
        func_00426898(arg2,var_s0,&sp20);
        func_00426BA8(sp20,&sp10,16);
        if(func_00426C74(&sp10,arg1)==0) {
            if(arg3) *arg3=var_s0;
            return 1;
        }
    }
    return 0;
}

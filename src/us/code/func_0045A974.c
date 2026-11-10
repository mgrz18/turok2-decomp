/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409FA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
#else
typedef s32 sptr;
#endif
extern void Pak_GetNoteGameName_2(s32,s32,sptr *),func_0045FBA8(sptr,void *,s32);
extern s32 Str_Compare_2(void *,void *);
s32 PakMenu_FindNoteByName_2(void *unused, void *arg1, s32 arg2, s32 *arg3) {
    char sp10[16];
    sptr sp20;
    s32 var_s0;
    for(var_s0=0;var_s0<16;var_s0++) {
        Pak_GetNoteGameName_2(arg2,var_s0,&sp20);
        func_0045FBA8(sp20,&sp10,16);
        if(Str_Compare_2(&sp10,arg1)==0) {
            if(arg3) *arg3=var_s0;
            return 1;
        }
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409FA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Pak_GetNoteGameName(s32,s32,sptr *),Pak_DecodeText(sptr,void *,s32);
extern s32 Str_Compare(void *,void *);
s32 PakMenu_FindNoteByName(void *unused, void *arg1, s32 arg2, s32 *arg3) {
    char sp10[16];
    sptr sp20;
    s32 var_s0;
    for(var_s0=0;var_s0<16;var_s0++) {
        Pak_GetNoteGameName(arg2,var_s0,&sp20);
        Pak_DecodeText(sp20,&sp10,16);
        if(Str_Compare(&sp10,arg1)==0) {
            if(arg3) *arg3=var_s0;
            return 1;
        }
    }
    return 0;
}

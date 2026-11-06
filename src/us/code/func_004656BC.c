/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_30.CPP, func_8044D5C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct State_func_8044D5C4_de;
struct State_func_8044D5C4_de {
    char p[0x1168];
    s32 unk1168;
    s32 unk116C;
    char q[8];
    s32 unk1178;
    s32 unk117C;
};
struct State_func_8044D5C4_de;
typedef struct State_func_8044D5C4_de State_func_8044D5C4_de;
void Cinema_Enable();
void Cinema_Disable();
void CScene__RestoreFiredEvent(void *, s32);
void func_004656BC(State_func_8044D5C4_de *arg0) {
 s32 i,n;
 Cinema_Disable();
 n=arg0->unk1168;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk1178+i*20);
 n=arg0->unk116C;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk117C+i*20);
 Cinema_Enable();
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern s32 func_00285878(s32 arg0);
extern int D_80130C60[];
extern u8 D_800F677B[];
extern void func_002847E0(void);
extern u8 D_800F6750;
extern u8 D_800F677B[];
extern u8 D_80130AA0[];
extern u8 D_800F6798[];
extern s32 osMotorInit(u8 *a0, u8 *a1, s32 a2);
extern s32 func_00425808(s32 a0);
extern void func_002858A4(void);
void func_0045F3A0(s32 kind)
{
    u8 *cur;
    s32 ready;
    ((void (*)(s32))func_00285878)(1);
    func_002847E0();
    D_800F6750 = 2;
    cur = D_80130AA0 + kind * 112;
    if (D_800F677B[kind * 4] != 0) {
        cur[0] = 0;
        ready = 0;
    } else {
        cur[0] = osMotorInit(D_800F6798, cur + 8, kind) == 0;
        ready = cur[0];
    }
    if (ready != 0) {
        D_80130C60[kind] = 2;
    } else if (func_00425808(kind) != 0) {
        D_80130C60[kind] = 3;
    } else {
        D_80130C60[kind] = 1;
    }
    func_002858A4();
}

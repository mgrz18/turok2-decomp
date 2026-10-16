/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_8.c, func_80446E58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
struct OSPifRam;
struct OSPifRam {
    u32 ramarray[15];
    u32 pifstatus;
};
struct OSPifRam;
typedef struct OSPifRam OSPifRam;
struct __OSContRequesFormatShort;
typedef struct __OSContRequesFormatShort __OSContRequesFormatShort;
struct __OSContRequesFormatShort;
struct __OSContRequesFormatShort {
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
};
extern void func_00460F78(int channel);
extern u8 D_80121420;
void func_00460F78(int channel)
{
    extern OSPifRam D_80130E20;
    u8 *base;
    u8 *ptr;
    __OSContRequesFormatShort requestformat;
    int i;
    D_80121420 = 0;
    base = (u8 *)&D_80130E20;
    D_80130E20.pifstatus = 1;
    ptr = base;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = 0;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    for (i = 0; i < channel; i++) {
        *ptr++ = 0;
    }
    *(__OSContRequesFormatShort *)ptr = requestformat;
    ptr += sizeof(__OSContRequesFormatShort);
    *ptr = 0xFE;
}

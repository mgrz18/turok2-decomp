/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_2.c, func_8044799C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
struct __OSContRequesFormat;
struct __OSContRequesFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
    u8 dummy1;
};
struct OSPifRam;
struct OSPifRam {
    u32 ramarray[15];
    u32 pifstatus;
};
struct __OSContRequesFormat;
typedef struct __OSContRequesFormat __OSContRequesFormat;
struct OSPifRam;
typedef struct OSPifRam OSPifRam;
extern u8 D_80121420;
void func_00461ABC(u8 cmd) {
    extern OSPifRam D_80130E20;
    extern u8 D_801213DC;
    u8 *ptr;
    __OSContRequesFormat requestformat;
    s32 i;
    u8 *count;
    D_80121420 = cmd;
    ptr = (u8 *)&D_80130E20;
    D_80130E20.pifstatus = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    count = &D_801213DC;
    for (i = 0; i < *count; i++) {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(requestformat);
    }
    *ptr = 0xFE;
}

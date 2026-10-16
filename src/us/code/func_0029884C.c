/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_14.c, func_802B768C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct OSPifRam;
struct OSPifRam {
    u32 ramarray[15];
    u32 pifstatus;
};
struct OSPifRam;
typedef struct OSPifRam OSPifRam;
struct ContReadFormat;
typedef struct ContReadFormat ContReadFormat;
struct ContReadFormat;
struct ContReadFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 button;
    s8 stick_x;
    s8 stick_y;
};
extern void func_0029884C();
extern s8 D_801213DC;
void func_0029884C(void) {
    extern OSPifRam D_801213E0;
    ContReadFormat readformat;
    u8 *ptr;
    u8 *count;
    s32 i;
    ptr = (u8 *)D_801213E0.ramarray;
    for (i = 14; i >= 0; i--) {
        ((s32 *)ptr)[i] = 0;
    }
    D_801213E0.pifstatus = 1;
    readformat.dummy = 0xFF;
    readformat.txsize = 1;
    readformat.rxsize = 4;
    readformat.cmd = 1;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;
    count = &D_801213DC;
    for (i = 0; i < *count; i++) {
        *(ContReadFormat *)ptr = readformat;
        ptr += sizeof(ContReadFormat);
    }
    *ptr = 0xFE;
}

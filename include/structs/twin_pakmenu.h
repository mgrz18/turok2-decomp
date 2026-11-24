/* Local types that vm1/PakMenu and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_PAKMENU_H
#define STRUCTS_TWIN_PAKMENU_H

#include "common.h"
#include "structs/menu.h"

typedef struct {
    char c[4];
} Chars4;
typedef union {
    u32 w;
    struct {
        s16 start;
        s16 end;
    } h;
} Span;
typedef struct {
    /* 0x00 */ u32 ctrl;
    /* 0x04 */ u32 width;
    /* 0x08 */ u32 burst;
    /* 0x0C */ u32 vSync;
    /* 0x10 */ u32 hSync;
    /* 0x14 */ u32 leap;
    /* 0x18 */ Span hStart;
    /* 0x1C */ u32 xScale;
    /* 0x20 */ u32 vCurrent;
} ViCommonRegs;
typedef struct {
    /* 0x00 */ u32 origin;
    /* 0x04 */ Span yScale;
    /* 0x08 */ Span vStart;
    /* 0x0C */ u32 vBurst;
    /* 0x10 */ u32 vIntr;
} ViFieldRegs;
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ ViCommonRegs comRegs;
    /* 0x28 */ ViFieldRegs fldRegs[2];
} ViMode; /* OSViMode, 0x50 */

#endif

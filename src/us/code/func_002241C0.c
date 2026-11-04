#include "common.h"

/* A table of halfwords after a count (TableRecAt8 in rw/abu_types.h). */
typedef struct {
    /* 0x00 */ s32 unused;
    /* 0x04 */ s32 count;
    /* 0x08 */ u16 entries[1];
} HalfwordTable;

/* The world (D_800F7078): only its halfword table at 0x74 is used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ HalfwordTable *table74;
} World;

u16 func_002241C0(World *arg0, s32 arg1) {
    return arg0->table74->entries[arg1];
}

#include "common.h"
#include "structs/world.h"

/* A table of halfwords after a count (TableRecAt8 in rw/abu_types.h). */
typedef struct {
    /* 0x00 */ s32 unused;
    /* 0x04 */ s32 count;
    /* 0x08 */ u16 entries[1];
} HalfwordTable;

u16 CScene__GetObjectTypeID(CScene *arg0, s32 arg1) {
    return ((HalfwordTable *)arg0->m_pObjectTypes)->entries[arg1];
}

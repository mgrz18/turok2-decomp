#include "common.h"
#include "protos/iset.h"

#ifdef TARGET_PORT
/* The port: the same through CScene (structs/world.h) and the host CGameRegion stride. */
#include "structs/world.h"
#include "structs/region.h"
sptr CScene__GetRegion(CScene *arg0, s32 arg1) {
    return (sptr)((CGameRegion *)((u8 *)CISet__GetBlock(arg0->m_pRegionData, 2) + 8) + arg1);
}
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 table;
} Arg0;


/* Entry arg1 (0x20 bytes each) of section 2 of the table at 0x58. */
s32 CScene__GetRegion(Arg0 *arg0, s32 arg1) {
    { s32 base = CISet__GetBlock(arg0->table, 2) + 8; return base + (arg1 << 5); }
}
#endif

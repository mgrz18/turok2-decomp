#include "common.h"
#include "structs/table.h"
#include "protos/iset.h"
#include "protos/cache.h"

#ifdef TARGET_PORT
/* The port: the scene by its typed fields (the view below has N64 offsets: 0x1C =
 * m_GraphicSetsKey, 0x48 = m_pGraphicSets, 0x7C = m_pGraphicSetTypes); with the view the
 * font's graphic set (COnScreen__LoadFontTexture) was never found and no text was drawn. */
#include "structs/world.h"
typedef CScene Owner;
#define unk1C m_GraphicSetsKey
#define unk48 m_pGraphicSets
#define table_of(o) ((StrideTable *)(o)->m_pGraphicSetTypes)
#else
typedef struct Owner {
    u8 pad0[0x1C];
    s32 unk1C;      /* 0x1C */
    u8 pad20[0x48 - 0x20];
    s32 unk48;      /* 0x48 */
    u8 pad4C[0x7C - 0x4C];
    StrideTable *table;   /* 0x7C */
} Owner;
#define table_of(o) ((o)->table)
#endif

extern s32 BinarySearch(void *entries, s32 count, sptr id);
extern u8 ProPack__Decompress[];

sptr CScene__LoadGraphicSet(Owner *owner, s32 id, s32 flags, s32 arg3) {
    s32 index;
    s32 out;

    index = BinarySearch(table_of(owner)->entries, table_of(owner)->count, id);
    if (index == -1) {
        return 0;
    }
    switch (id) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
        flags |= 0x18;
        break;
    default:
        flags |= 4;
        break;
    }
    return CCache__Request(0, CISet__GetROMAddressAndSize(owner->unk48, owner->unk1C, index, &out), out, flags, 0,
                         ProPack__Decompress, "graphicset", arg3);
}

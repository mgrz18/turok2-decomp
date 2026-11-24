#include "common.h"

#include "structs/hud.h"

/* Frame `frame` (wrapped to the count) of *ptable: its data in *pdata and its
 * byte in *pbyte; *pdata is NULL with no table. */
void Hud_GetImageFrame(FrameTable **ptable, s32 frame, void **pdata, u8 *pbyte) {
    FrameTable *table;
    FrameEntry *entry;
    s32 offset;
    u8 byte;
    s32 i;

    *pdata = NULL;
    if (ptable != NULL) {
        table = *ptable;
        entry = table->m_Frames;
        i = frame % table->m_nFrames;
        offset = entry[i].m_Offset;
        byte = entry[i].m_Byte;
        *pbyte = byte;
        *pdata = (u8 *)table + offset;
    }
}

#include "common.h"

/* A table of frames: a count, then for each an offset from the table's start
 * and a byte. */
typedef struct {
    /* 0x0 */ s32 m_Offset;
    /* 0x4 */ u8 m_Byte;
} FrameEntry; /* size 0x8 */

typedef struct {
    /* 0x0 */ s16 m_nFrames;
    /* 0x2 */ u8 _pad[6];
    /* 0x8 */ FrameEntry m_Frames[1];
} FrameTable;

/* Frame `frame` (wrapped to the count) of *ptable: its data in *pdata and its
 * byte in *pbyte; *pdata is NULL with no table. */
void func_0027AE44(FrameTable **ptable, s32 frame, void **pdata, u8 *pbyte) {
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

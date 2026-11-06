/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80256190_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 Random_Next(void);
void *CList__GetRandom(void *arg0) {
    s32 index;
    s32 offset;
    void *node;
    node = ((CList *)arg0)->m_pHead;
    if (node == 0) {
        return 0;
    }
    index = Random_Next() % (u32)((CList *)arg0)->m_nCount;
    index--;
    if (index != -1) {
        offset = ((CList *)arg0)->m_NextOffset;
        do {
            { u8 *cursor = (u8 *)node; cursor += offset; node = *(void **)cursor; }
            index--;
        } while (index != -1);
    }
    return node;
}

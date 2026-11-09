/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80255F70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/list.h"

void CList__RemoveHead(CList *arg0) {
    char *cursor = arg0->m_pHead;
    int stride = arg0->m_NextOffset;
    void *value = *(void **)(cursor + stride);
    int count = arg0->m_nCount - 1;
    arg0->m_nCount = count;
    arg0->m_pHead = value;
}

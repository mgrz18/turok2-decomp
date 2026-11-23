#include "common.h"
#include "structs/game_object.h"

/* De-faked in round 21 (j4): the draft returned 0 through three identical arms and read m_dwFlags by
 * offset (a host offset under TARGET_PORT); typed, and testing the kind-1 case first, it matches. */

/* Flag 1 of an instance of kind 1 (0 for other kinds). */
s32 func_00268BE0(CGameObjectInstance *obj) {
    if (obj->header.m_Type == 1) {
        return obj->m_dwFlags & 1;
    }
    return 0;
}

#include "common.h"
#ifdef TARGET_PORT
#include "structs/region.h"
#endif

/* Resource entry: a key, its flags and three child entries. */
#ifdef TARGET_PORT
typedef struct Entry0026C5C4 {
/* the view at the host offsets of CGameRegion (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Entry0026C5C4(m) __builtin_offsetof(CGameRegion, m)
    union {
        struct { u8 _hp_key[T2_HOFF_Entry0026C5C4(m_nInfo)]; u16 key; }; /* N64 0x0 */
        struct { u8 _hp_flags[T2_HOFF_Entry0026C5C4(m_wFlags)]; u16 flags; }; /* N64 0x2 */
        struct { u8 _hp_child0[T2_HOFF_Entry0026C5C4(m_pNeighbours)]; struct Entry0026C5C4 *child0; }; /* N64 0x10 */
        struct { u8 _hp_child1[T2_HOFF_Entry0026C5C4(m_pNeighbours[1])]; struct Entry0026C5C4 *child1; }; /* N64 0x14 */
        struct { u8 _hp_child2[T2_HOFF_Entry0026C5C4(m_pNeighbours[2])]; struct Entry0026C5C4 *child2; }; /* N64 0x18 */
    };
} Entry0026C5C4;
#else
typedef struct Entry0026C5C4 {
    /* 0x00 */ u16 key;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Entry0026C5C4 *child0;
    /* 0x14 */ struct Entry0026C5C4 *child1;
    /* 0x18 */ struct Entry0026C5C4 *child2;
} Entry0026C5C4;
#endif

void CGameRegion__FloodClearFlag4Same(Entry0026C5C4 *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    Entry0026C5C4 *temp_a0;
    Entry0026C5C4 *temp_a0_2;
    Entry0026C5C4 *temp_a0_3;

    temp_s0 = arg0->key;
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = arg0->flags;
        if (temp_v1 & 4) {
            temp_a0 = arg0->child0;
            arg0->flags = (u16) (temp_v1 & 0xFFFB);
            if (temp_a0 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0, temp_s0);
            }
            temp_a0_2 = arg0->child1;
            if (temp_a0_2 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_2, temp_s0);
            }
            temp_a0_3 = arg0->child2;
            if (temp_a0_3 != NULL) {
                CGameRegion__FloodClearFlag4Same(temp_a0_3, temp_s0);
            }
        }
    }
}

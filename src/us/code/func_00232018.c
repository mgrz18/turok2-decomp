#include "common.h"

#ifdef TARGET_PORT
#include "common.h"
#include "structs/particle.h"
#endif
/* A record with a type at 2 (0x56 here) and flags at 0x40. */
#ifdef TARGET_PORT
typedef struct TypedRecord {
/* the view at the host offsets of CParticle (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_TypedRecord(m) __builtin_offsetof(CParticle, m)
    union {
        struct { u8 _hp_type[T2_HOFF_TypedRecord(header.m_nObjType)]; u16 type; }; /* N64 0x2 */
        struct { u8 _hp_flags[T2_HOFF_TypedRecord(m_dwFlags)]; s32 flags; }; /* N64 0x40 */
    };
} TypedRecord;
#else
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 type;
    /* 0x04 */ u8 pad4[0x40 - 4];
    /* 0x40 */ s32 flags;
} TypedRecord;
#endif

void func_00232018(TypedRecord *arg0, s32 arg1) {
    if ((arg0->type == 0x56) && (arg1 == 2)) {
        arg0->flags |= 0x04000000;
    }
}

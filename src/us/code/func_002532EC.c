#include "structs/vector.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/player_list.h"
#endif

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A470.c, func_0022A470.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2 layout: pos at 0x4, next at 0x1320 (Rage Wars: 0x8 / 0x16E0). */
typedef struct Plr Plr;
#ifdef TARGET_PORT
struct Plr {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Plr(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Plr(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_next[T2_HOFF_Plr(m_pNext)]; Plr *next; }; /* N64 0x1320 */
    };
};
#else
struct Plr {
    u8 pad0[0x4];
    CVector3 pos;          /* 0x0004 */
    u8 pad10[0x1310];
    Plr *next;          /* 0x1320 */
};
#endif
#ifdef TARGET_PORT
typedef struct PlrGroup {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PlrGroup(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_head[T2_HOFF_PlrGroup(m_ActiveList.m_pHead)]; Plr *head; }; /* N64 0x20 */
    };
} PlrGroup;
#else
typedef struct PlrGroup {
    u8 pad0[0x20];
    Plr *head;          /* 0x20 */
} PlrGroup;
#endif
Plr *CPlayerPool__FindNearest(PlrGroup *g, CVector3 p) {
    Plr *a;
    Plr *best = ((void *)0);
    f32 min = 3.4028235e38f;
    f32 dx, dy, dz, d;
    for (a = g->head; a != ((void *)0); a = a->next) {
        dx = a->pos.x - p.x;
        dy = a->pos.y - p.y;
        dz = a->pos.z - p.z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < min) {
            min = d;
            best = a;
        }
    }
    return best;
}

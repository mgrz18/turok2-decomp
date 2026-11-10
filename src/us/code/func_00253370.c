#include "structs/vector.h"
#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/player_list.h"
#endif

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A4F4.c, func_0022A4F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Plr Plr;
typedef struct PlrGroup PlrGroup;
/* Turok 2 player: only the fields used here (Rage Wars: pos 0x8, unk5E4, next 0x16E0). */
#ifdef TARGET_PORT
struct Plr {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Plr(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Plr(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk524[T2_HOFF_Plr(m_Data.m_Health)]; s32 unk524; }; /* N64 0x524 */
        struct { u8 _hp_next[T2_HOFF_Plr(m_pNext)]; Plr *next; }; /* N64 0x1320 */
    };
};
#else
struct Plr {
    u8 pad0[0x4];
    CVector3 pos;      /* 0x4 */
    u8 pad10[0x514];
    s32 unk524;     /* 0x524 */
    u8 pad528[0xDF8];
    Plr *next;      /* 0x1320 */
};
#endif
#ifdef TARGET_PORT
struct PlrGroup {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PlrGroup(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_unk0[T2_HOFF_PlrGroup(m_pmePlayers)]; s32* unk0; }; /* N64 0x0 */
        struct { u8 _hp_unk4[T2_HOFF_PlrGroup(m_Players)]; Plr* unk4; }; /* N64 0x4 */
        struct { u8 _hp_unk8[T2_HOFF_PlrGroup(m_nPlayers)]; s32 unk8; }; /* N64 0x8 */
        struct { u8 _hp_head[T2_HOFF_PlrGroup(m_ActiveList.m_pHead)]; Plr* head; }; /* N64 0x20 */
    };
};
#else
struct PlrGroup {
                 s32* unk0;
                 Plr* unk4;
                 s32 unk8;
                 u8 padC[0x14];
                 Plr* head;
                 u8 pad24[0x1C];
                 s32 unk40;
};
#endif
Plr *CPlayerPool__FindNearestOther(PlrGroup *g, CVector3 p, Plr *skip) {
    Plr *a;
    Plr *best = ((void *)0);
    f32 min = 3.4028235e38f;
    f32 dx, dy, dz, d;
    for (a = g->head; a != ((void *)0); a = a->next) {
        if (a->unk524 != 0 && a != skip) {
            dx = a->pos.x - p.x;
            dy = a->pos.y - p.y;
            dz = a->pos.z - p.z;
            d = dx * dx + dy * dy + dz * dz;
            if (d < min) {
                min = d;
                best = a;
            }
        }
    }
    return best;
}

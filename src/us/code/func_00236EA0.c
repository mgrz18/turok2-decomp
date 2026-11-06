#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00265E30.c, func_00265E30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/world.h"
typedef struct M04Part {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) M04Part;
typedef void (*M04Handler)(void *, void *, s32, CVector3, M04Part);
typedef struct M04Msg {
    M04Handler fn;
    u32 flags;
} M04Msg;
typedef struct M04WorldB {
                u8 pad0[0xBF8];
                void *tab[1];
} M04WorldB;
extern M04Msg D_800AD080[];
extern CScene gScene;
extern CPlayer *D_80119890;     /* the players, a list through m_pNext */
/* Dispatch one message through the handler table (inlined). */
static __inline__ void send_(void *tgt, void *dst, s32 kind, CVector3 p, M04Part q) {
    if (D_800AD080[kind].fn != 0) {
        D_800AD080[kind].fn(tgt, dst, kind, p, q);
    }
}
void ImpactEvent_Send(void *src, void *dst, s32 kind, s32 id, CVector3 pos, M04Part part) {
    s32 i;
    s32 n;
    u8 *p;
    void **pp;
    void *cur;
    u8 *base;
    CPlayer *o;
    if (gScene.m_Mode == 4 || (D_800AD080[kind].flags & 0x10)) {
        if (!(D_800AD080[kind].flags & 2)) {
            send_(src, dst, kind, pos, part);
        } else if (D_800AD080[kind].flags & 8) {
            n = gScene.m_nActors;
            base = (u8 *)gScene.m_pActors;
            i = 0;
            if (n > 0) {
                do {
                    cur = base + i * 0x288;
                    if (cur != src) {
                        send_(cur, dst, kind, pos, part);
                    }
                    i++;
                } while (i < n);
            }
            o = D_80119890;
            if (o != 0) {
                do {
                    if ((void *)o != src && (void *)&o->m_WeaponActor != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->m_pNext;
                } while (o != 0);
            }
        } else {
            n = *(s32 *)((u8 *)&gScene + 0xDF8);
            for (i = 0; i < n; i++) {
                cur = ((M04WorldB *)&gScene)->tab[i];
                if (cur != src) {
                    send_(cur, dst, kind, pos, part);
                }
            }
            o = D_80119890;
            if (o != 0) {
                do {
                    if ((void *)o != src && (void *)&o->m_WeaponActor != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->m_pNext;
                } while (o != 0);
            }
        }
    }
}

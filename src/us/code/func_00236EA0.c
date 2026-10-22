/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00265E30.c, func_00265E30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
typedef struct M04Part {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) M04Part;
typedef void (*M04Handler)(void *, void *, s32, M04Vec3, M04Part);
typedef struct M04Msg {
    M04Handler fn;
    u32 flags;
} M04Msg;
typedef struct M04World {
                s32 mode;
                u8 pad4[0xDC];
                u8 *objs;       /* 0xE0: LevelActor array (stride 0x288) */
                u8 padE4[4];
                s32 nobjs;
} M04World;
typedef struct M04WorldB {
                u8 pad0[0xBF8];
                void *tab[1];
} M04WorldB;
typedef struct M04Node {
    u8 pad0[0x1320];
    struct M04Node *next;
} M04Node;
extern M04Msg D_800AD080[];
extern M04World D_800F7078;
extern M04Node *D_80119890;
/* Dispatch one message through the handler table (inlined). */
static __inline__ void send_(void *tgt, void *dst, s32 kind, M04Vec3 p, M04Part q) {
    if (D_800AD080[kind].fn != 0) {
        D_800AD080[kind].fn(tgt, dst, kind, p, q);
    }
}
void func_00236EA0(void *src, void *dst, s32 kind, s32 id, M04Vec3 pos, M04Part part) {
    s32 i;
    s32 n;
    u8 *p;
    void **pp;
    void *cur;
    u8 *base;
    M04Node *o;
    if (D_800F7078.mode == 4 || (D_800AD080[kind].flags & 0x10)) {
        if (!(D_800AD080[kind].flags & 2)) {
            send_(src, dst, kind, pos, part);
        } else if (D_800AD080[kind].flags & 8) {
            n = D_800F7078.nobjs;
            base = D_800F7078.objs;
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
                    if ((void *)o != src && (void *)((u8 *)o + 0x288) != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->next;
                } while (o != 0);
            }
        } else {
            n = *(s32 *)((u8 *)&D_800F7078 + 0xDF8);
            for (i = 0; i < n; i++) {
                cur = ((M04WorldB *)&D_800F7078)->tab[i];
                if (cur != src) {
                    send_(cur, dst, kind, pos, part);
                }
            }
            o = D_80119890;
            if (o != 0) {
                do {
                    if ((void *)o != src && (void *)((u8 *)o + 0x288) != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->next;
                } while (o != 0);
            }
        }
    }
}

#include "common.h"

typedef struct Sweep {
    /* 0x00 */ u8 pad0[0x9C];
    /* 0x9C */ s32 done;
} Sweep;

typedef struct ModelSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ u8 pad4[0x44];
    /* 0x48 */ f32 hitDist;
    /* 0x4C */ void *model;
    /* 0x50 */ s32 kind;
    /* 0x54 */ f32 mtx[0x10];
    /* 0x94 */ f32 inverse[0x10];
    /* 0xD4 */ char *meshes;
    /* 0xD8 */ s32 allPolys;
} ModelSweep;

/* A vertex of the display list, put in the sweep's space and classified. */
typedef struct ProjVertex {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s16 *pos;
} ProjVertex;

typedef struct CVector3 {
    f32 x, y, z;
} CVector3;

typedef union DLCmd {
    struct { u32 w0; u32 w1; };
    u8 b[8];
} DLCmd;

char *func_002017D4(s32 *list, s32 index);
void func_0020F85C(f32 *mtx, CVector3 *in, ProjVertex *out);
void func_0023D4D4(ModelSweep *ms, ProjVertex **tri);

#define G_VTX 0x01
#define G_TRI1 0x05
#define G_TRI2 0x06
#define G_ENDDL 0xDF

/* Walk one polygon group's display list (F3DEX2 G_VTX/G_TRI1/G_TRI2) and
 * sweep against every triangle that is not wholly outside the sweep. */
void func_0023D7CC(ModelSweep *ms, s32 *group) {
    ProjVertex verts[32];
    ProjVertex *tri[3];
    CVector3 pos;
    s32 doubleSided;
    u8 op;
    Sweep *sweep;
    u32 *header;
    DLCmd *cmd;
    ProjVertex *v;
    ProjVertex *swap;
    s16 *src;
    s32 n;
    s32 flags;
    f32 z;

    header = (u32 *)func_002017D4(group, 0);
    if (ms->allPolys == 0 && (*header & 0x280)) {
        return;
    }
    doubleSided = *header & 4;
    sweep = ms->sweep;
    cmd = (DLCmd *)func_002017D4(group, 1);
    do {
        if (sweep->done != 0) {
            return;
        }
        op = cmd->b[0];
        switch (op) {
            case G_TRI1:
            case G_TRI2:
                tri[0] = &verts[(cmd->w0 & 0xFE0000) >> 17];
                tri[1] = &verts[(cmd->w0 & 0xFE00) >> 9];
                tri[2] = &verts[(cmd->w0 & 0xFF) >> 1];
                if (!(tri[0]->flags & tri[1]->flags & tri[2]->flags)) {
                    func_0023D4D4(ms, tri);
                    if (doubleSided) {
                        swap = tri[0];
                        tri[0] = tri[2];
                        tri[2] = swap;
                        func_0023D4D4(ms, tri);
                    }
                }
                if (op == G_TRI2) {
                    tri[0] = &verts[(cmd->w1 & 0xFE0000) >> 17];
                    tri[1] = &verts[(cmd->w1 & 0xFE00) >> 9];
                    tri[2] = &verts[(cmd->w1 & 0xFF) >> 1];
                    if (!(tri[0]->flags & tri[1]->flags & tri[2]->flags)) {
                        func_0023D4D4(ms, tri);
                        if (doubleSided) {
                            swap = tri[0];
                            tri[0] = tri[2];
                            tri[2] = swap;
                            func_0023D4D4(ms, tri);
                        }
                    }
                }
                break;
            case G_VTX:
                src = (s16 *)((cmd->w1 & 0xFFFFFF) + (u32)ms->meshes);
                n = (cmd->w0 >> 12) & 0xFF;
                v = &verts[((cmd->w0 & 0xFE) >> 1) - n];
                do {
                    v->pos = src;
                    pos.x = src[0];
                    pos.y = src[1];
                    pos.z = src[2];
                    func_0020F85C(ms->inverse, &pos, v);
                    flags = 2;
                    if (v->x < 0.0f) {
                        flags = 1;
                    }
                    if (v->y < 0.0f) {
                        flags |= 8;
                    } else {
                        flags |= 4;
                    }
                    z = v->z;
                    v->flags = flags;
                    if (z >= 0.0f) {
                        v->flags = flags | 0x10;
                    } else if (z < ms->hitDist) {
                        v->flags = flags | 0x20;
                    }
                    src += 8;
                    v++;
                } while (--n != 0);
                break;
        }
        cmd++;
    } while (op != G_ENDDL);
}

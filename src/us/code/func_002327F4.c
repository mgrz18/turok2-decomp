/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/CODE_8028414C.CPP, func_80284178_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;

typedef struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
} CVector3;

typedef struct Matrix {
    f32 m[4][4];
} Matrix;

/* Turok 2 layout (smaller than Rage Wars' Shared_Particle). */
typedef struct Shared_Particle {
    char pad0[0x4];
    CVector3 pos;      /* 0x04 */
    char pad10[0x30];
    s32 flags;         /* 0x40 */
    char pad44[0x4];
    Matrix mtx[2];     /* 0x48, double-buffered */
} Shared_Particle;

struct Shared_ParticleView {
    char pad0[0x20C];
    Matrix viewMtx;    /* 0x20C */
};

extern void func_0020F85C(void *arg0, void *arg1, void *arg2);
extern s32 D_801198E0[];
extern s32 D_800B6D1C;
extern void func_0022D2FC(void *, s32, s32, f32);

void func_002327F4(void *arg0) {
    CVector3 delta;
    void *arg1;
    f32 amount;
    s32 *state;
    state = D_801198E0;
    if (state[0] == 1) {
        arg1 = (void *)state[-4];
        func_0020F85C(&((struct Shared_ParticleView *)arg1)->viewMtx, &((Shared_Particle *)arg0)->pos, &delta);
        amount = delta.z;
        if (amount < 0.0f) {
            amount = -amount;
        }
        func_0022D2FC(arg0,
                      (s32)((char *)arg0 + ((D_800B6D1C << 6) + 0x48)),
                      (s32)arg1, amount);
    } else {
        func_0022D2FC(arg0,
                      (s32)((char *)arg0 + ((D_800B6D1C << 6) + 0x48)),
                      0, 0.0f);
    }
    ((Shared_Particle *)arg0)->flags |= 0x100000;
}

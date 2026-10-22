/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409A18.c, func_00409A18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef struct O01Save {
              u16 w[4];
              u8 b0;
              u8 b1;
              u8 b2;
              u8 b3;
} O01Save;
typedef struct O01SaveSrc {
               u16 w[4];
               u8 pad8[0x28];
               u8 b0;          /* 0x30 */
               u8 pad31[2];
               u8 tail[8];     /* 0x33 */
} O01SaveSrc;
typedef struct O01Actor {
                u8 pad0[0x518];
                O01SaveSrc *save; /* 0x518: player options */
} O01Actor;
extern O01Save D_80130968;
extern u8 D_80130974[8];
void func_0045A374(O01Actor *a) {
    O01SaveSrc *p = a->save;
    s32 i;
    D_80130968.b0 = p->b0;
    D_80130968.w[0] = p->w[0];
    D_80130968.w[1] = p->w[1];
    D_80130968.w[2] = p->w[2];
    D_80130968.w[3] = p->w[3];
    D_80130968.b1 = 0;
    D_80130968.b2 = 0;
    D_80130968.b3 = 0;
    for (i = 0; i < 8; i++) {
        D_80130974[i] = p->tail[i];
    }
}

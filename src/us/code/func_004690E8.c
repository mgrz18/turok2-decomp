/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044BC58.c, func_0044BC58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
typedef struct S5CtlEnt {
            u8 a;
            u8 b;
            u8 c;
            u8 d;
} S5CtlEnt;
typedef struct S5Ctl {
                s32 f0;
                s8 port;
                u8 pad5[3];
                s32 f8;
                s32 fC;
                s32 f10;
                s32 f14;
                s32 f18;
                s32 f1C;
                s32 f20;
                s32 f24;
                s32 f28;
                S5CtlEnt ent[32];
                u8 padAC[0xC4 - 0xAC];
                u8 fC4;
                u8 fC5;
                u8 fC6;
                u8 fC7;
                s32 fC8;
                s32 fCC;
                s32 fD0;
                s32 fD4;
                u8 padD8[0x140 - 0xD8];
                u8 blkA[0x2C];
                u8 blkB[0xB4];
                s32 f220;
} S5Ctl;
typedef struct S5PortFlags {
            u8 b[4];
} S5PortFlags;
extern S5PortFlags D_800F6778[];
extern void func_0026EB3C(void *a, void *b, s32 n);
s32 func_004690E8(S5Ctl *c, s32 port) {
    s32 i;
    c->port = port;
    c->f0 = 0;
    c->fC8 = 0;
    c->fCC = 0;
    c->f220 = 0;
    c->f8 = 0;
    c->fC = 0;
    c->f10 = 0;
    c->fC4 = 0;
    c->fC5 = 0;
    c->fC6 = 0;
    c->fC7 = 0;
    c->f14 = 0;
    c->f18 = 0;
    c->f1C = 0;
    c->f20 = 0;
    c->f24 = 0;
    c->f28 = 0;
    for (i = 0; i < 32; i++) {
        c->ent[i].a = 0;
        c->ent[i].b = 0;
        c->ent[i].c = 0x30;
        c->ent[i].d = 0;
    }
    c->f0 = ((D_800F6778[port].b[3] >> 3) ^ 1) & 1;
    c->fD0 = 0;
    c->fD4 = 0;
    func_0026EB3C(c->blkA, c->blkB, 3);
    return c->f0;
}

typedef signed char s8;
typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    /* 0x00 */ char pad0[0x2F];
    /* 0x2F */ s8 kind;
    /* 0x30 */ char pad30[0x3];
    /* 0x33 */ u8 name[8];
} NamedThing;

extern u8 D_800B5E80[];
extern u8 D_800B5E8C[];
extern u8 D_800B5E98[];
extern u8 D_800B5EA4[];

void func_00454BB0(NamedThing *t) {
    s32 i;

    switch (t->kind) {
        case 0:
        default:
            for (i = 0; i < 8; i++) {
                t->name[i] = D_800B5E80[i];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++) {
                t->name[i] = D_800B5E8C[i];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                t->name[i] = D_800B5E98[i];
            }
            break;
        case 3:
            for (i = 0; i < 8; i++) {
                t->name[i] = D_800B5EA4[i];
            }
            break;
    }
}

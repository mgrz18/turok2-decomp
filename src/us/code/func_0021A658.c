/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34.CPP, func_8024715C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
union func_80203908_S3_U124;
typedef union func_80203908_S3_U124 func_80203908_S3_U124;
union func_80203908_S3_U124;
union func_80203908_S3_U124 {
    f32 v0;
    s32 v1;
};
struct CollisionInfo;
struct CollisionInfo {
    s32 w[8];
};
struct ObjectLinks23C;
struct ObjectLinks23C {
    char pad0[0x4];
    CVector3 unk_8;     /* 0x04 (0x08 in Rage Wars) */
    u16 *unk_14;        /* 0x10 (0x14) */
    s32 *unk_18;        /* 0x14 (0x18) */
    char pad18[0xD4 - 0x18];
    s32 unk_100;        /* 0xD4 (0x100) */
    char padD8[0x170 - 0xD8];
    void *unk_1A0;      /* 0x170 (0x1A0) */
    char pad174[0x1C4 - 0x174];
    u16 *unk_1F4;       /* 0x1C4 (0x1F4) */
    char pad1C8[0x204 - 0x1C8];
    u16 unk_238;        /* 0x204 (0x238) */
};
struct CallbackState288 {
    unsigned char padding_0[0x23C];
    void (*callback)(void *, void *, struct CollisionInfo *); /* 0x23C (0x284) */
};
typedef struct ObjectStateC ObjectStateC;
struct ObjectLinks10;
typedef struct ObjectLinks10 ObjectLinks10;
struct CallbackState288;
typedef struct CallbackState288 CallbackState288;
struct CollisionInfo;
struct ObjectLinks10;
struct ObjectLinks10 {
    char pad0[0xC];
    struct CollisionInfo * unk_C;
};
struct ObjectLinks23C;
typedef struct ObjectLinks23C ObjectLinks23C;
struct ObjectStateC;
struct ObjectStateC {
    char pad0[0x8];
    func_80203908_S3_U124 unk_8;
};
struct func_80237E70_G1;
struct func_80237E70_G1 {
    u8 * unk0;
};
struct func_80237E70_G1;
typedef struct func_80237E70_G1 func_80237E70_G1;
extern void func_0021A658(char *, CVector3 *);
typedef struct CollisionInfo CollisionInfo;
extern CollisionInfo D_800F1EB0;
extern func_80237E70_G1 D_800F1CE0;
extern s32 func_00239F00(void *arg0, CVector3 arg1, CollisionInfo *arg2);
extern void func_0026E19C(s32 arg0, s32 arg1, void *arg2);
extern void func_0026E110(void *arg0, s32 arg1, void *arg2);
void func_0021A658(char *arg0, CVector3 *arg1) {
    CollisionInfo *collision;
    u16 *old_contact;
    s32 collided;
    f32 saved;
    void (*callback)(void *, void *, CollisionInfo *);
    if ((((ObjectLinks23C *)(arg0))->unk_100 & 0x10000) && ((ObjectLinks23C *)(arg0))->unk_1A0 != 0) {
        collision = ((ObjectLinks10 *)(((ObjectLinks23C *)(arg0))->unk_1A0))->unk_C;
        if (collision == 0) {
            collision = &D_800F1EB0;
        }
        saved = ((ObjectStateC *)(collision))->unk_8.v0;
        if ((u32)(((ObjectLinks23C *)(arg0))->unk_238 - 0x2B0C) < 8) {
            ((ObjectStateC *)(collision))->unk_8.v1 = 0;
        }
        old_contact = ((ObjectLinks23C *)(arg0))->unk_14;
        collided = func_00239F00(arg0, *arg1, collision);
        if (old_contact != ((ObjectLinks23C *)(arg0))->unk_14) {
            ((ObjectLinks23C *)(arg0))->unk_1F4 = old_contact;
        }
        ((ObjectStateC *)(collision))->unk_8.v0 = saved;
        if (old_contact != 0 && ((ObjectLinks23C *)(arg0))->unk_14 != 0 &&
                *old_contact != *((ObjectLinks23C *)(arg0))->unk_14 &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1) {
            func_0026E19C((s32)old_contact, 0x80, arg0);
            func_0026E19C((s32)((ObjectLinks23C *)(arg0))->unk_14, 0x40, arg0);
        }
        if (collided != 0 && !(((ObjectLinks23C *)(arg0))->unk_100 & 0x300000) &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1 && D_800F1CE0.unk0 != 0 &&
                *D_800F1CE0.unk0 == 1) {
            func_0026E110(D_800F1CE0.unk0, 0x20, arg0);
        }
        callback = ((struct CallbackState288 *) ((char *) arg0))->callback;
        if (callback != 0) {
            callback(arg0, arg0 + 0x140, collision);
        }
    } else {
        ((ObjectLinks23C *)(arg0))->unk_8 = *arg1;
    }
}

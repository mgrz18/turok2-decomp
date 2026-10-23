typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ f32 scale;
} Def292DB0;

typedef struct {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ Def292DB0 *def;
    /* 0x1C */ char pad1C[0x8];
    /* 0x24 */ s32 value;
} Obj292DB0;

void func_00292DB0(Obj292DB0 *obj, f32 f) {
    if (obj->def != 0) {
        obj->value = f * obj->def->scale;
    } else {
        obj->value = 0x1E8;
    }
}

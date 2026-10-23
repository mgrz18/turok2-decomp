typedef int s32;

typedef struct {
    char pad0[0x1C8];
    s32 unk_1C8; /* 0x1C8 */
} Obj1C8;

s32 func_00416C54(Obj1C8 *obj) {
    s32 v = obj->unk_1C8;
    if (v < 0x18) {
        return v;
    }
    return 0x2F - v;
}

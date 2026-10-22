/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6BD4.c, func_002A6BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct Node6BD4 {
    u8 pad0[8];
    u8 sub[0x40];
    s32 unk48;
} Node6BD4;
typedef struct Pool6BD4 {
    u8 pad0[0x94D8];
    u8 lists[0xC8];     /* Turok 2: 0x94D8 (Rage Wars: 0x94E0) */
    Node6BD4 *unk95A0;
} Pool6BD4;
extern f32 D_800A6524;
extern s32 D_800ACFC0;
void func_0020E810(void *a, Vec3f *b);
void CList__Remove(void *list, Node6BD4 *n);
void CList__AddTail(void *list, Node6BD4 *n);
void func_00211114(void *a, void *b);
void func_002106C0(void *a, f32 f);
void func_0020F040(Vec3f *v);
void func_0020EF60(Vec3f *a, Vec3f *b, f32 s);
void func_0020EEF8(Vec3f *a, Vec3f *b, Vec3f *c);
void func_00210318(void *m, f32 x, f32 y, f32 z);
void func_0021034C(void *m, f32 a, f32 b, f32 c);
void func_0020D274(void *a, void *b);
Node6BD4 *func_002364A4(Pool6BD4 *o, Vec3f pos, Vec3f dir, s32 idx, s32 val) {
    u8 m10[0x40];
    u8 m50[0x10];
    Node6BD4 *n;
    u8 *list;
    u8 *l;
    func_0020E810(m50, &dir);
    if (D_800ACFC0 != 0) {
        return 0;
    }
    {
        D_800ACFC0 = 1;
        n = o->unk95A0;
        list = o->lists + idx * 0x14;
        if (n != 0) {
            CList__Remove((u8 *)o + 0x95A0, n);
            goto have;
        }
        n = *(Node6BD4 **)list;
        if (n != 0) {
            CList__Remove(list, n);
have:
            CList__AddTail(list, n);
            if (n != 0) {
                n->unk48 = val;
                func_00211114(m50, m10);
                func_002106C0(m10, 1.5707965f);
                func_0020F040(&dir);
                func_0020EF60(&dir, &dir, 2.048f);
                func_0020EEF8(&pos, &pos, &dir);
                func_00210318(m10, pos.x, pos.y, pos.z);
                func_0021034C(m10, D_800A6524, 1.0f, D_800A6524);
                func_0020D274(m10, n->sub);
            }
        }
    }
    return n;
}

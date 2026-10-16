/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_8.c, func_80264104_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern void func_0026ECF0(void *arg0);
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
extern int D_80110028;
struct Triple;
extern u32 func_00298E10(struct Triple *);
struct Obj80264124;
struct Obj80264124 {
    s32 flags;
    s8 enabled;
    char pad05[3];
    s32 x;
    s32 y;
    s32 z;
    char pad14[0xB4];
    s32 active;
    s32 state;
    f32 amount;
    f32 timer;
    char status[0x68];
    char object[1];
};
extern float D_800A9CF8;
struct Obj80264124;
typedef struct Obj80264124 Obj80264124;
extern f32 func_0026E760(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 D_800F5A50;
extern s32 D_8011ACA4;
extern void func_0026EDA8(s32 *);
extern unsigned int func_00298FD4(void *arg0);
void func_002851C0(Obj80264124 *arg0) {
    Triple zero;
    void *object;
    s32 *global;
    f32 first;
    if (arg0->active != 0) {
        zero.x = 0;
        zero.y = 0;
        zero.z = 0;
        if (arg0->state == 0) {
            goto reset;
        }
        global = &D_8011ACA4;
        if ((*global != 0) || (D_80110028 != 0) || (global[-6] != 13)) {
reset:
            arg0->amount = 0.0f;
            func_0026EDA8(arg0->object);
        } else {
            object = arg0->object;
            func_0026ECF0(object);
            first = func_0026E760(&D_800F5A50, arg0->x, arg0->y, arg0->z);
            arg0->amount = first + func_0026E760(object, zero.x, zero.y, zero.z);
        }
        if (arg0->active != 0) {
            if (arg0->amount != 0.0f) {
                arg0->timer += arg0->amount;
                if (arg0->timer >= D_800A9CF8) {
                    arg0->timer -= D_800A9CF8;
func_00298FD4
((void *)arg0->status);
                    return;
                }
            }
((unsigned int (*)(void *))func_00298E10)
((void *)arg0->status);
        }
    }
}

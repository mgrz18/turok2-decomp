/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043EA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct GameSession;
extern void func_00288C5C(struct GameSession *object, int value);
struct Inner8043E56C;
struct Inner8043E56C {
    u8 pad0[4];
    s8 unk4;
};
struct Inner8043E56C;
struct Outer8043E56C;
struct Outer8043E56C {
    u8 pad0[0x20];
    struct Inner8043E56C *unk20;
};
struct Outer8043E56C;
typedef struct Outer8043E56C Outer8043E56C;
extern s32 func_00419664(void *arg0, Outer8043E56C *arg1);
struct Inner8043E56C;
typedef struct Inner8043E56C Inner8043E56C;
extern void func_00285A80(s32 arg0);
extern s32 D_800F6CB0;
extern s32 D_801308F8;
s32 func_00419664(void *arg0, Outer8043E56C *arg1) {
    func_00288C5C(&D_800F6CB0, 8);
    D_801308F8 = 0;
    func_00285A80(arg1->unk20->unk4);
    return 1;
}

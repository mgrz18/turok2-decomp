/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FA0C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern void func_0029AC90(s32 arg0);
struct OSScTask_s;
struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    struct { s32 type; } list;
};
struct OSScTask_s;
typedef struct OSScTask_s OSScTask_s;
struct OSScTask_s;
extern s32 func_0028B0A8(void *,void *);
s32 func_0028B0A8(void *arg0, void *arg1) {
    if ((((s32)((struct OSScTask_s *)arg1)->state)) & 3) {
        return 0;
    }
    if ((((struct OSScTask_s *)arg1)->list.type) != 1) {
        return 1;
    }
    if (((((s32)((struct OSScTask_s *)arg1)->flags)) & 0x60) != 0x60) {
        return 1;
    }
    func_0029AC90(((s32)((struct OSScTask_s *)arg1)->framebuffer));
    return 1;
}

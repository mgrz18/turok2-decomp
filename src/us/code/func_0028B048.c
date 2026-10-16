/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F9AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
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
extern void *func_0028B048(struct OSScTask_s *arg0);
extern void *func_0029A6E0();
extern s32 D_8011B098;
extern s32 D_8011B0A0;
void *func_0028B048(struct OSScTask_s *arg0) {
    s32 f1a;
    s32 f1b;
    void *v2;
    if (arg0 == 0) {
        return 0;
    }
    f1a = (s32)arg0->framebuffer;
    if (f1a == D_8011B0A0) {
        return 0;
    }
    v2 = func_0029A6E0();
    f1b = (s32)arg0->framebuffer;
    if (f1b == (s32)v2) {
        return 0;
    }
    if (f1b != D_8011B098) {
        return arg0;
    }
    return 0;
}

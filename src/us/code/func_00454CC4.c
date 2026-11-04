#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044421C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
typedef struct IntrusiveList IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
struct IntrusiveList;
extern int func_00285628(ControllerProfile *object);
extern int func_00285670(ControllerProfile *object);
struct CVector3_t;
struct CVector3_t;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    CVector3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
struct PoolRecAt28V1 {
    IntrusiveList free;
    IntrusiveList active;
    s32 baseLevel;
};
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
s32 func_002855E8(char *);
extern char D_800F5C78;
extern u32 D_8011AAE0[];
s32 func_00454CC4(void) {
 if(((s32 (*)(char *))func_00285628)(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=32; break;
 case 16: D_8011AAE0[0]=8; break;
 case 32: D_8011AAE0[0]=16; break;
 }
 }
 if(((s32 (*)(char *))func_00285670)(&D_800F5C78) || func_002855E8(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=16; break;
 case 16: D_8011AAE0[0]=32; break;
 case 32: D_8011AAE0[0]=8; break;
 }
 }
 return 0;
}

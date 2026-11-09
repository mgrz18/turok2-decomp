#include "structs/vector.h"
#include "globals/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GAMECORE/80278900_2.CPP, func_80278ABC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
struct Shape_typemap_13;
struct Shape_typemap_13 {
    int field_0;
    int field_4;
};
struct ObjectLinks150C;
typedef struct ObjectLinks150C ObjectLinks150C;
struct IntegerState1504;
struct IntegerState1504 {
    char pad0[0x14A8];
    s32 unk_14A8;
};
struct IntegerState1504;
typedef struct IntegerState1504 IntegerState1504;
struct ObjectLinks150C;
struct ObjectLinks150C {
    unsigned char padding_0[0x14AC];
    s32 *unk_14AC;
};
extern s32 func_00214108(void);
s32 func_00214108(void) {
    s32 total = 0;
    s32 i;
    char *base = (char *)&gScene;
    for (i = 0; i < ((IntegerState1504 *)(base))->unk_14A8; i++) {
        total += ((struct Shape_typemap_13 *)CISet__GetBlock(*((struct ObjectLinks150C *) (base + (i * 8)))->unk_14AC, 2))->field_4;
    }
    return total;
}

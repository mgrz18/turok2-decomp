/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028BD88.CPP, func_8028BDAC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/iset.h"
#ifdef TARGET_PORT
#include "structs/world.h"
#endif
#include "protos/scene.h"
struct Field_void_6C;
struct Field_void_6C {
    char pad[0x6C];
    void * value;
};
struct Field_void_6C;
typedef struct Field_void_6C Field_void_6C;
s32 CScene__IsLinkGroupDone(void *arg0, s32 arg1) {
    void *temp_v0;
    u8 *result;
#ifdef TARGET_PORT /* m_pBitSections by member (host CScene layout) */
    temp_v0 = CISet__GetBlock(((CScene *) arg0)->m_pBitSections, 1);
#else
    temp_v0 = CISet__GetBlock((((struct Field_void_6C *) ((s8 *) arg0))->value), 1);
#endif
    CISet__GetBlock(temp_v0, 0);
    result = (u8 *)((char *)CISet__GetBlock(temp_v0, 1)) + arg1;
    return *result == 0;
}

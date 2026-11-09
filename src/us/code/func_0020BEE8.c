#include "common.h"
#include "protos/iset.h"
#include "protos/cache.h"


void Model_TouchTextures(s32 *arg0) {
    s32 *temp_v0;
    s32 *temp_v0_2;
    s32 temp_a1;
    s32 temp_s2;
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;

    temp_v0 = CISet__GetBlock(*arg0, 2);
    temp_s4 = *temp_v0;
    var_s1 = 0;
    if (temp_s4 > 0) {
        do {
            temp_v0_2 = CISet__GetBlock(temp_v0, var_s1);
            temp_s2 = *temp_v0_2;
            var_s0 = 0;
            if (temp_s2 > 0) {
                do {
                    temp_a1 = ((s32 *) CISet__GetBlock(CISet__GetBlock(temp_v0_2, var_s0), 0))[2];
                    if (temp_a1 != 0) {
                        CCache__Touch(0, temp_a1);
                    }
                    var_s0 += 1;
                } while (var_s0 < temp_s2);
            }
            var_s1 += 1;
        } while (var_s1 < temp_s4);
    }
}

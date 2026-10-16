/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80405454_de_2.c, func_80405CDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct RecordRecAt1C;
extern void func_0045E558(struct RecordRecAt1C *arg0);
struct RecordRecAt1C;
struct RecordRecAt1C {
    s32 a;
    s32 b;
    s32 c;
    s32 values[3];
    s32 d;
    s32 e;
};
struct RecordRecAt1C;
extern void func_002052D8(void *arg0, void *arg1);
void func_0045E558(struct RecordRecAt1C *arg0) {
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a1_4;
    s32 temp_a1_5;
    s32 var_s1;
    s32 var_v1;
    temp_a1 = arg0->b;
    if (temp_a1 != 0) {
        func_002052D8(0, temp_a1);
    }
    temp_a1_2 = arg0->c;
    if (temp_a1_2 != 0) {
        func_002052D8(0, temp_a1_2);
    }
    temp_a1_3 = arg0->d;
    if (temp_a1_3 != 0) {
        func_002052D8(0, temp_a1_3);
    }
    temp_a1_4 = arg0->e;
    if (temp_a1_4 != 0) {
        func_002052D8(0, temp_a1_4);
    }
    var_s1 = 0;
    do {
        temp_a1_5 = arg0->values[var_s1];
        if (temp_a1_5 != 0) {
            func_002052D8(0, temp_a1_5);
        }
        var_s1 += 1;
    } while (var_s1 < 3);
    var_v1 = 2;
    arg0->a = 0;
    arg0->b = 0;
    arg0->c = 0;
    arg0->d = 0;
    arg0->e = 0;
    do {
        arg0->values[var_v1] = 0;
        var_v1 -= 1;
    } while (var_v1 >= 0);
}

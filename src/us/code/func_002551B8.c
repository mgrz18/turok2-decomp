/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022BAA0_de_4.c, func_8022C67C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern float func_002551B8(void * arg0);
typedef struct ObjRecAtB8 {
    int unk0;
    int unk4;
    int unk8;
    char padC[0x90 - 0xC];
    float unk90;
    float unk94;
    float unk98;
    int unk9C;
    float unkA0;
    int unkA4;
    int unkA8;
    float unkAC;
    float unkB0;
    int unkB4;
    int unkB8;
} ObjRecAtB8;
extern float D_800A7140;
extern float D_800A7148;
extern float D_800A7150;
float func_002551B8(void *arg0) {
    ObjRecAtB8 *obj = arg0;
    float f1 = D_800A7140;
    float f2 = *(float *)((char *)&D_800A7140 + 4);
    float f0 = D_800A7148;
    float f3 = *(float *)((char *)&D_800A7148 + 4);
    float f4 = D_800A7150;
    obj->unkB4 = 0;
    obj->unk8 = 0;
    obj->unkB8 = 0;
    obj->unk4 = -1;
    obj->unk0 = -1;
    obj->unkA8 = 0;
    obj->unk9C = 0;
    obj->unkA4 = 0;
    obj->unkAC = f1;
    obj->unkB0 = f2;
    obj->unk90 = f0;
    obj->unk94 = f3;
    obj->unk98 = f0;
    obj->unkA0 = f4;
}

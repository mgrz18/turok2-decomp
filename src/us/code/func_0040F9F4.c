/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80400000_4.CPP, func_8040067C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Shape_typemap_165;
struct Pack;
struct Shape_typemap_165 {
    union { int field_0; struct Pack *pack; void *data; };
    int field_4;
    int field_8;
    int field_C;
};
struct Pack;
struct Pack {
    s32 count;
    s32 offsets[1];
};
struct Pack;
typedef struct Pack Pack;
struct Spline;
typedef struct Spline Spline;
struct SplineNode;
struct SplineNode {
    CVector3 p;
    CVector3 d2p;
    f32 s;
    f32 w;
    f32 d2s;
};
struct SplineNode;
typedef struct SplineNode SplineNode;
struct Spline;
struct Spline {
    s32 nodeSize;
    s32 count;
    SplineNode nodes[1];
};
typedef struct SplineSource { s32 stride,count; union { char data[0]; CVector3 points[0]; f32 weights[0]; }; } SplineSource;
typedef struct SplineTable { s32 count; } SplineTable;
extern int func_002018D8(int *);
extern void func_00218548(void *,s32);
extern char *func_00218590(Pack *,s32,s32);
typedef union SplineSourceCursor { SplineSource *header; CVector3 *points; f32 *weights; } SplineSourceCursor;
struct Shape_typemap_165;
struct Shape_typemap_165;
struct Shape_typemap_165;
struct Shape_typemap_165;
extern s32 func_00200B00(void ***, s32);
void *func_002017D4(void *table, s32 index);
extern struct Shape_typemap_165 *func_00205D7C(s32,void *,s32);
extern void func_0040F5B8(s32,CVector3 *,f32 *,Spline *);
extern void func_002057B0(s32,struct Shape_typemap_165 **,struct Shape_typemap_165 *);
static __inline__ char *packAppend(Pack *pack,void *prepare,s32 size) { return func_00218590(pack,(s32)prepare,size); }
static __inline__ s32 splineRecordSize(s32 left,s32 right) { s32 a=left*9,b=right*9; return (b+a)*4+48; }
void func_0040F9F4(s32 unused, struct Shape_typemap_165 **handle) {
    struct Shape_typemap_165 *output;
    void *source, *a, *b, *c, *part, *extra;
    Pack *pack, *dest;
    SplineTable *parts;
    Pack *table;
    SplineSourceCursor p,q,r,s;
    s32 count, i, size, sa, sb, sc, np, nr, byteP, byteR;
    void *dp, *dr;
    if (!func_00200B00((void **)handle, 1)) return;
    source = (*handle)->pack;
    parts = func_002017D4(source, 0);
    a = func_002017D4(source, 1);
    b = func_002017D4(source, 2);
    c = func_002017D4(source, 3);
    count = parts->count;
    size = (count * 4 + 15) & ~7;
    for (i = 0; i < parts->count; i++) {
        SplineSource *firstP,*firstR;
        void *firstPart;
        s32 byteP,byteR;
        firstPart = func_002017D4(parts, i);
        firstP = func_002017D4(firstPart, 0);
        firstR = func_002017D4(firstPart, 2);
        size += splineRecordSize(firstP->count,firstR->count);
    }
    sc = func_002018D8(c);
    sa = func_002018D8(a);
    sb = func_002018D8(b);
    output = func_00205D7C(0, handle, size + sa + sb + sc + 24);
    if (output == 0) return;
    pack = output->pack;
    func_00218548(pack, 4);
    table = (void *)packAppend(pack, 0, size);
    packAppend(pack, a, sa);
    packAppend(pack, b, sb);
    packAppend(pack, c, sc);
    func_00218548(table, count);
    for (i = 0; i < count; i++) {
        s32 byteP,byteR;
        SplineSource *sizeP,*sizeR;
        part = func_002017D4(parts, i);
        sizeP = func_002017D4(part, 0);
        sizeR = func_002017D4(part, 2);
        byteP = sizeP->count * 9;
        byteR = sizeR->count * 9;
        dest = (void *)packAppend(table, 0, (byteR + byteP)*4 + 48);
        func_00218548(dest, 3);
        p.header = func_002017D4(part, 0);
        q.header = func_002017D4(part, 1);
        r.header = func_002017D4(part, 2);
        s.header = func_002017D4(part, 3);
        extra = func_002017D4(part, 4);
        np = p.header->count;
        nr = r.header->count;
        { s32 pointBytes,otherBytes;
        pointBytes = np * 36 + 8;
        otherBytes = nr * 36 + 8;
        dp = packAppend(dest, 0, pointBytes);
        dr = packAppend(dest, 0, otherBytes);
        }
        p.points = p.header->points;
        q.weights = q.header->weights;
        r.points = r.header->points;
        s.weights = s.header->weights;
        packAppend(dest, extra, 8);
        func_0040F5B8(np, p.points, q.weights, dp);
        func_0040F5B8(nr, r.points, s.weights, dr);
    }
    func_002057B0(0, handle, output);
}

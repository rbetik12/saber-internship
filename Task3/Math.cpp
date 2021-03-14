#include "Math.h"
#include <intrin.h>
#include <cstring>
#include <cmath>

Vec3 CalcFaceNormal(IVec3& vertexIndexes, const Vec3* vertices) {
    // Load vertices in xmm regs
    __m128 vertex0 = _mm_loadu_ps((float*)&vertices[vertexIndexes.x]);
    __m128 vertex1 = _mm_loadu_ps((float*)&vertices[vertexIndexes.y]);
    __m128 vertex2 = _mm_loadu_ps((float*)&vertices[vertexIndexes.z]);
    // Calculate triangle vectors
    __m128 vec0 = _mm_sub_ps(vertex1, vertex0);
    __m128 vec1 = _mm_sub_ps(vertex2, vertex0);

    // Shuffle vectors for cross-product. We don't care about 4th element.
    // vec0 = (a1; a2; a3)
    // vec1 = (b1; b2; b3)
    // normal:
    // a2*b3 - a3*b2
    // a3*b1 - a1*b3
    // a1*b2 - a2*b1
    __m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 tmp2 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 tmp3 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 0, 2, 1));

    __m128 normalReg = _mm_sub_ps(
        _mm_mul_ps(tmp0, tmp1),
        _mm_mul_ps(tmp2, tmp3)
    );

    float normal[4];
    _mm_storeu_ps(normal, normalReg);
    Vec3 normalVec;
    std::memcpy(&normalVec, normal, sizeof(float) * 3);
    return normalVec;
}

float Vec3::Magnitude() {
    return sqrt(x * x + y * y + z * z);
}

float Vec3::Angle(Vec3& vec) {
    return (
        (x * vec.x + y * vec.y + z * vec.z) /
        (this->Magnitude() * vec.Magnitude())
    );
}

void Vec3::Normalize() {
    x /= Magnitude();
    y /= Magnitude();
    z /= Magnitude();
}

Vec3 Vec3::operator-(const Vec3& vec) {
    __m128 thisVec = _mm_loadu_ps((float*)this);
    __m128 otherVec = _mm_loadu_ps((float*)&vec);

    __m128 res = _mm_sub_ps(thisVec, otherVec);

    float result[4];
    _mm_storeu_ps(result, res);
    return Vec3(result[0], result[1], result[2]);
}

Vec3 Vec3::operator+(const Vec3& vec) {
    __m128 thisVec = _mm_loadu_ps((float*)this);
    __m128 otherVec = _mm_loadu_ps((float*)&vec);

    __m128 res = _mm_add_ps(thisVec, otherVec);

    float result[4];
    _mm_storeu_ps(result, res);
    return Vec3(result[0], result[1], result[2]);
}

Vec3 Vec3::operator*(const float a) {
    __m128 thisVec = _mm_loadu_ps((float*)this);
    __m128 constVec = _mm_load_ps1(&a);

    __m128 res = _mm_mul_ps(thisVec, constVec);
    float result[4];
    _mm_storeu_ps(result, res);
    return Vec3(result[0], result[1], result[2]);
}

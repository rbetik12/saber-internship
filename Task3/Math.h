#include <ostream>

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z): x(x), y(y), z(z) {}
    Vec3(): x(0), y(0), z(0) {}

    float Magnitude();
    float Angle(Vec3& vec);
    void Normalize();

    Vec3 operator-(const Vec3& vec);

    Vec3 operator+(const Vec3& vec);

    Vec3 operator*(const float a);

    Vec3& operator+=(const Vec3& vec) {
        *this = *this + vec;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& output, const Vec3& vec) {
        output << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z;
        return output;
    }
};

struct IVec3 {
    int x;
    int y;
    int z;
};

Vec3 CalcFaceNormal(IVec3& vertexIndexes, const Vec3* vertices);
struct Vec3 {
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z): x(x), y(y), z(z) {}
    Vec3(): x(0), y(0), z(0) {}

    float Magnitude();
    float Angle(Vec3& vec);
    void Normalize();

    Vec3 operator-(const Vec3& vec) {
        return Vec3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vec3 operator+(const Vec3& vec) {
        return Vec3(x + vec.x, y + vec.y, z + vec.z);
    }

    Vec3 operator*(const float a) {
        return Vec3(x * a, y * a, z * a);
    }

    Vec3& operator+=(const Vec3& vec) {
        *this = *this + vec;
        return *this;
    }
};

struct IVec3 {
    int x;
    int y;
    int z;
};

Vec3 CalcFaceNormal(IVec3& vertexIndexes, const Vec3* vertices);
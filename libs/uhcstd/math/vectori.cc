#include "uhcmath.hh"

namespace VectorI {
    unsigned char equals(VectorI::It& a, VectorI::It& b) {
        return !((a.x - b.x) + (a.y - b.y) + (a.z - b.z) + (a.w - b.w));
    };

    void set(VectorI::It& vec, int x, int y, int z, int w) {
        vec.x = x;
        vec.y = y;
        vec.z = z;
        vec.w = w;
    };

    void normalize(VectorI::It& vec) {
        float mag = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
        if (mag == 0.0) return;

        vec.x /= mag;
        vec.y /= mag;
        vec.z /= mag;
        vec.w /= mag;
    };

    float length(VectorI::It& vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    };

    void zero(VectorI::It& vec) {
        set(vec, 0, 0, 0, 0);
    };

    int dot(VectorI::It& a, VectorI::It& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    };

    void cross(VectorI::It& result, VectorI::It& a, VectorI::It& b) {
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
        result.w = 0;
    };

    void sub(VectorI::It& result, VectorI::It& a, VectorI::It& b) {
        result.x = a.x - b.x;
        result.y = a.y - b.y;
        result.z = a.z - b.z;
        result.w = a.w - b.w;
    };
};

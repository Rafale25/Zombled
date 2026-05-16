#include "uhcmath.hh"

namespace VectorF {
    unsigned char equals(VectorF::It& a, VectorF::It& b) {
        return !((a.x - b.x) + (a.y - b.y) + (a.z - b.z) + (a.w - b.w));
    };

    void set(VectorF::It& vec, float x, float y, float z, float w) {
        vec.x = x;
        vec.y = y;
        vec.z = z;
        vec.w = w;
    };

    void normalize(VectorF::It& vec) {
        float mag = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
        if (mag == 0.0) return;

        vec.x /= mag;
        vec.y /= mag;
        vec.z /= mag;
        vec.w /= mag;
    };

    float length(VectorF::It& vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    };

    void zero(VectorF::It& vec) {
        set(vec, 0, 0, 0, 0);
    };

    float dot(VectorF::It& a, VectorF::It& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    };

    void cross(VectorF::It& result, VectorF::It& a, VectorF::It& b) {
        result.x = a.y * b.z - a.z * b.y;
        result.y = a.z * b.x - a.x * b.z;
        result.z = a.x * b.y - a.y * b.x;
        result.w = 0.0f;
    };

    void sub(VectorF::It& result, VectorF::It& a, VectorF::It& b) {
        result.x = a.x - b.x;
        result.y = a.y - b.y;
        result.z = a.z - b.z;
        result.w = a.w - b.w;
    };
};

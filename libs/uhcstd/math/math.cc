#include "uhcmath.hh"

namespace Math {
    float rad(float angle) {
        return angle * UHC_PI / 180.0f;
    };

    char sign(int value) {
        if (value == 0) return 0;
        if (value < 0) return -1;
        
        return 1;
    };

    char fsign(float value) {
        if (value == 0.0) return 0;
        if (value < 0.0) return -1;
        
        return 1;
    };

    float fsignf(float value) {
        if (value == 0.0) return 0;
        if (value < 0.0) return -1;
        
        return 1;
    };

    int clamp(int v, int min, int max) {
        if (v > max) v = max;
        if (v < min) v = min;
        return v;
    };

    int fclamp(float v, float min, float max) {
        if (v > max) v = max;
        if (v < min) v = min;
        return v;
    };

    float fclampf(float v, float min, float max) {
        if (v > max) v = max;
        if (v < min) v = min;
        return v;
    };
};

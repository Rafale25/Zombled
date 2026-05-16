#pragma once

#include <math.h>

/// Pi constant (3.141592653589793)
#define UHC_PI 3.141592653589793

/// Scalar math helper functions
namespace Math {
    /// Convert degrees to radians
    /// @param angle Angle in degrees
    /// @returns Equivalent angle in radians
    float rad(float angle);
    /// Return the sign of an integer value
    /// @param value The value to inspect
    /// @returns -1 if negative, 0 if zero, 1 if positive
    char sign(int value);
    /// Return the sign of a float as an integer
    /// @param value The value to inspect
    /// @returns -1 if negative, 0 if zero, 1 if positive
    char fsign(float value);
    /// Return the sign of a float as a float
    /// @param value The value to inspect
    /// @returns -1.0 if negative, 0.0 if zero, 1.0 if positive
    float fsignf(float value);
    /// Clamp an integer between min and max
    /// @param v Value to clamp
    /// @param min Lower bound (inclusive)
    /// @param max Upper bound (inclusive)
    /// @returns Clamped integer value
    int clamp(int v, int min, int max);
    /// Clamp a float between boundaries, return as integer
    /// @param v Value to clamp
    /// @param min Lower bound
    /// @param max Upper bound
    /// @returns Clamped value as integer
    int fclamp(float v, float min, float max);
    /// Clamp a float between boundaries, return as float
    /// @param v Value to clamp
    /// @param min Lower bound
    /// @param max Upper bound
    /// @returns Clamped float value
    float fclampf(float v, float min, float max);
};

/// Four-component float vector (x, y, z, w)
namespace VectorF {
    /// Float vector object
    struct It {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };

    /// Test whether two float vectors are component-wise equal
    /// @param a First vector
    /// @param b Second vector
    /// @returns Non-zero if equal
    unsigned char equals(VectorF::It& a, VectorF::It& b);
    /// Set all four components of a float vector
    /// @param vec Target vector
    /// @param x X component
    /// @param y Y component
    /// @param z Z component
    /// @param w W component
    void set(VectorF::It& vec, float x, float y, float z, float w);
    /// Normalize a float vector to unit length in-place
    /// @param vec Vector to normalize
    void normalize(VectorF::It& vec);
    /// Compute the length (magnitude) of a float vector
    /// @param vec The vector
    /// @returns Euclidean length
    float length(VectorF::It& vec);
    /// Set all components of a float vector to zero
    /// @param vec Vector to zero
    void zero(VectorF::It& vec);
    /// Compute the dot product of two float vectors
    /// @param a First vector
    /// @param b Second vector
    /// @returns Dot product
    float dot(VectorF::It& a, VectorF::It& b);
    /// Compute the cross product of two float vectors
    /// @param result Vector to store the result
    /// @param a First vector
    /// @param b Second vector
    void cross(VectorF::It& result, VectorF::It& a, VectorF::It& b);
    /// Subtract one float vector from another
    /// @param result Vector to store the result
    /// @param a Minuend vector
    /// @param b Subtrahend vector
    void sub(VectorF::It& result, VectorF::It& a, VectorF::It& b);
};

/// Four-component integer vector (x, y, z, w)
namespace VectorI {
    /// Integer vector object
    struct It {
        int x = 0;
        int y = 0;
        int z = 0;
        int w = 0;
    };

    /// Test whether two integer vectors are component-wise equal
    /// @param a First vector
    /// @param b Second vector
    /// @returns Non-zero if equal
    unsigned char equals(VectorI::It& a, VectorI::It& b);
    /// Set all four components of an integer vector
    /// @param vec Target vector
    /// @param x X component
    /// @param y Y component
    /// @param z Z component
    /// @param w W component
    void set(VectorI::It& vec, int x, int y, int z, int w);
    /// Normalize an integer vector to unit length in-place
    /// @param vec Vector to normalize
    void normalize(VectorI::It& vec);
    /// Compute the length (magnitude) of an integer vector
    /// @param vec The vector
    /// @returns Euclidean length as float
    float length(VectorI::It& vec);
    /// Set all components of an integer vector to zero
    /// @param vec Vector to zero
    void zero(VectorI::It& vec);
    /// Compute the dot product of two integer vectors
    /// @param a First vector
    /// @param b Second vector
    /// @returns Dot product
    int dot(VectorI::It& a, VectorI::It& b);
    /// Compute the cross product of two integer vectors
    /// @param result Vector to store the result
    /// @param a First vector
    /// @param b Second vector
    void cross(VectorI::It& result, VectorI::It& a, VectorI::It& b);
    /// Subtract one integer vector from another
    /// @param result Vector to store the result
    /// @param a Minuend vector
    /// @param b Subtrahend vector
    void sub(VectorI::It& result, VectorI::It& a, VectorI::It& b);
};

/// Row 0, column 0 matrix index
#define M00 0x00
/// Row 0, column 1 matrix index
#define M01 0x01
/// Row 0, column 2 matrix index
#define M02 0x02
/// Row 0, column 3 matrix index
#define M03 0x03

/// Row 1, column 0 matrix index
#define M10 0x04
/// Row 1, column 1 matrix index
#define M11 0x05
/// Row 1, column 2 matrix index
#define M12 0x06
/// Row 1, column 3 matrix index
#define M13 0x07

/// Row 2, column 0 matrix index
#define M20 0x08
/// Row 2, column 1 matrix index
#define M21 0x09
/// Row 2, column 2 matrix index
#define M22 0x0A
/// Row 2, column 3 matrix index
#define M23 0x0B

/// Row 3, column 0 matrix index
#define M30 0x0C
/// Row 3, column 1 matrix index
#define M31 0x0D
/// Row 3, column 2 matrix index
#define M32 0x0E
/// Row 3, column 3 matrix index
#define M33 0x0F

/// 4x4 row-major float matrix
namespace Matrix {
    /// Matrix object (16 floats, initialized to identity)
    struct It {
        float values[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    };

    /// Reset a matrix to the identity matrix
    /// @param mat The matrix to reset
    void setIdentity(Matrix::It& mat);
    /// Multiply two matrices and store the result
    /// @param result Matrix to store the result
    /// @param left First (left) matrix
    /// @param right Second (right) matrix
    void multiply(Matrix::It& result, Matrix::It& left, Matrix::It& right);
    /// Copy a matrix into another
    /// @param src Source matrix
    /// @param dest Destination matrix
    void copy(Matrix::It& src, Matrix::It& dest);
    /// Build a 2D orthographic projection matrix
    /// @param mat Output matrix
    /// @param left Left boundary
    /// @param right Right boundary
    /// @param bottom Bottom boundary
    /// @param top Top boundary
    /// @param near Near plane
    /// @param far Far plane
    void ortho2D(Matrix::It& mat, float left, float right, float bottom, float top, float near, float far);
    /// Build a 2D orthographic projection matrix adjusted for Vulkan's coordinate system
    /// @param mat Output matrix
    /// @param left Left boundary
    /// @param right Right boundary
    /// @param bottom Bottom boundary
    /// @param top Top boundary
    /// @param near Near plane
    /// @param far Far plane
    void ortho2DVk(Matrix::It& mat, float left, float right, float bottom, float top, float near, float far);
    /// Build a 3D perspective projection matrix
    /// @param mat Output matrix
    /// @param width Viewport width
    /// @param height Viewport height
    /// @param fov Field of view angle in radians
    /// @param near Near plane distance
    /// @param far Far plane distance
    void projection(Matrix::It& mat, float width, float height, float fov, float near, float far);
    /// Apply a scale transform to a matrix
    /// @param mat Matrix to modify
    /// @param x X scale factor
    /// @param y Y scale factor
    /// @param z Z scale factor
    /// @param w W scale factor
    void scale(Matrix::It& mat, float x, float y, float z, float w);
    /// Apply a translation transform to a matrix
    /// @param mat Matrix to modify
    /// @param x X translation
    /// @param y Y translation
    /// @param z Z translation
    /// @param w W translation
    void translate(Matrix::It& mat, float x, float y, float z, float w);
    /// Apply a rotation transform to a matrix
    /// @param mat Matrix to modify
    /// @param angle Rotation angle in radians
    /// @param x X axis weight
    /// @param y Y axis weight
    /// @param z Z axis weight
    void rotate(Matrix::It& mat, float angle, float x, float y, float z);
    /// Build a view matrix from eye position, target, and up vector
    /// @param mat Output view matrix
    /// @param eye Camera position
    /// @param center Camera target point
    /// @param up World up direction
    void lookAt(Matrix::It& mat, VectorF::It& eye, VectorF::It& center, VectorF::It& up);
};

#include "uhcstd.hh"

namespace Buffer {
    void setOrder(Buffer::It& buffer, unsigned char order) {
        buffer.order = order;
    };

    void reset(Buffer::It& buffer) {
        buffer.cursor = 0;
    };

    char getI8(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 1;
        return (char)*p;
    };

    unsigned char getU8(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 1;
        return *p;
    };

    short getI16(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 2;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (short)(p[0] | (p[1] << 8));
        else
            return (short)((p[0] << 8) | p[1]);
    };

    unsigned short getU16(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 2;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (unsigned short)(p[0] | (p[1] << 8));
        else
            return (unsigned short)((p[0] << 8) | p[1]);
    };

    int getI32(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 4;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (int)(
                ( p[0]          ) |
                ( p[1] <<  8    ) |
                ( p[2] << 16    ) |
                ((int)p[3] << 24)
            );
        else
            return (int)(
                ((int)p[0] << 24) |
                ( p[1] << 16    ) |
                ( p[2] <<  8    ) |
                ( p[3]          )
            );
    };

    unsigned int getU32(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 4;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (unsigned int)(
                ( p[0]          ) |
                ( p[1] <<  8    ) |
                ( p[2] << 16    ) |
                ((unsigned int)p[3] << 24)
            );
        else
            return (unsigned int)(
                ((unsigned int)p[0] << 24) |
                ( p[1] << 16    ) |
                ( p[2] <<  8    ) |
                ( p[3]          )
            );
    };

    long long getI64(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 8;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (long long)(
                ((long long)p[0]      ) |
                ((long long)p[1] <<  8) |
                ((long long)p[2] << 16) |
                ((long long)p[3] << 24) |
                ((long long)p[4] << 32) |
                ((long long)p[5] << 40) |
                ((long long)p[6] << 48) |
                ((long long)p[7] << 56)
            );
        else
            return (long long)(
                ((long long)p[0] << 56) |
                ((long long)p[1] << 48) |
                ((long long)p[2] << 40) |
                ((long long)p[3] << 32) |
                ((long long)p[4] << 24) |
                ((long long)p[5] << 16) |
                ((long long)p[6] <<  8) |
                ((long long)p[7]      )
            );
    };

    unsigned long long getU64(Buffer::It& buffer) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 8;
        if (buffer.order == UHC_LITTLE_ENDIAN)
            return (unsigned long long)(
                ((unsigned long long)p[0]      ) |
                ((unsigned long long)p[1] <<  8) |
                ((unsigned long long)p[2] << 16) |
                ((unsigned long long)p[3] << 24) |
                ((unsigned long long)p[4] << 32) |
                ((unsigned long long)p[5] << 40) |
                ((unsigned long long)p[6] << 48) |
                ((unsigned long long)p[7] << 56)
            );
        else
            return (unsigned long long)(
                ((unsigned long long)p[0] << 56) |
                ((unsigned long long)p[1] << 48) |
                ((unsigned long long)p[2] << 40) |
                ((unsigned long long)p[3] << 32) |
                ((unsigned long long)p[4] << 24) |
                ((unsigned long long)p[5] << 16) |
                ((unsigned long long)p[6] <<  8) |
                ((unsigned long long)p[7]      )
            );
    };

    float getF32(Buffer::It& buffer) {
        union { unsigned int i; float f ;} u;
        u.i = getU32(buffer);
        return u.f;
    };

    double getF64(Buffer::It& buffer) {
        union { unsigned long long i; double f ;} u;
        u.i = getU64(buffer);
        return u.f;
    };

    void putI8(Buffer::It& buffer, char value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 1;
        p[0] = (unsigned char)value;
    };

    void putU8(Buffer::It& buffer, unsigned char value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 1;
        p[0] = value;
    };

    void putI16(Buffer::It& buffer, short value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 2;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value       & 0xFF;
            p[1] = (value >> 8) & 0xFF;
        } else {
            p[0] = (value >> 8) & 0xFF;
            p[1] =  value       & 0xFF;
        };
    };

    void putU16(Buffer::It& buffer, unsigned short value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 2;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value       & 0xFF;
            p[1] = (value >> 8) & 0xFF;
        } else {
            p[0] = (value >> 8) & 0xFF;
            p[1] =  value       & 0xFF;
        };
    };

    void putI32(Buffer::It& buffer, int value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 4;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value        & 0xFF;
            p[1] = (value >>  8) & 0xFF;
            p[2] = (value >> 16) & 0xFF;
            p[3] = (value >> 24) & 0xFF;
        } else {
            p[0] = (value >> 24) & 0xFF;
            p[1] = (value >> 16) & 0xFF;
            p[2] = (value >>  8) & 0xFF;
            p[3] =  value        & 0xFF;
        };
    };

    void putU32(Buffer::It& buffer, unsigned int value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 4;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value        & 0xFF;
            p[1] = (value >>  8) & 0xFF;
            p[2] = (value >> 16) & 0xFF;
            p[3] = (value >> 24) & 0xFF;
        } else {
            p[0] = (value >> 24) & 0xFF;
            p[1] = (value >> 16) & 0xFF;
            p[2] = (value >>  8) & 0xFF;
            p[3] =  value        & 0xFF;
        };
    };

    void putI64(Buffer::It& buffer, long long value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 8;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value        & 0xFF;
            p[1] = (value >>  8) & 0xFF;
            p[2] = (value >> 16) & 0xFF;
            p[3] = (value >> 24) & 0xFF;
            p[4] = (value >> 32) & 0xFF;
            p[5] = (value >> 40) & 0xFF;
            p[6] = (value >> 48) & 0xFF;
            p[7] = (value >> 56) & 0xFF;
        } else {
            p[0] = (value >> 56) & 0xFF;
            p[1] = (value >> 48) & 0xFF;
            p[2] = (value >> 40) & 0xFF;
            p[3] = (value >> 32) & 0xFF;
            p[4] = (value >> 24) & 0xFF;
            p[5] = (value >> 16) & 0xFF;
            p[6] = (value >>  8) & 0xFF;
            p[7] =  value        & 0xFF;
        };
    };

    void putU64(Buffer::It& buffer, unsigned long long value) {
        unsigned char* p = (unsigned char*)buffer.handle + buffer.cursor;
        buffer.cursor += 8;
        if (buffer.order == UHC_LITTLE_ENDIAN) {
            p[0] =  value        & 0xFF;
            p[1] = (value >>  8) & 0xFF;
            p[2] = (value >> 16) & 0xFF;
            p[3] = (value >> 24) & 0xFF;
            p[4] = (value >> 32) & 0xFF;
            p[5] = (value >> 40) & 0xFF;
            p[6] = (value >> 48) & 0xFF;
            p[7] = (value >> 56) & 0xFF;
        } else {
            p[0] = (value >> 56) & 0xFF;
            p[1] = (value >> 48) & 0xFF;
            p[2] = (value >> 40) & 0xFF;
            p[3] = (value >> 32) & 0xFF;
            p[4] = (value >> 24) & 0xFF;
            p[5] = (value >> 16) & 0xFF;
            p[6] = (value >>  8) & 0xFF;
            p[7] =  value        & 0xFF;
        };
    };

    void putF32(Buffer::It& buffer, float value) {
        union { float f; unsigned int u ;} u;
        u.f = value;
        putU32(buffer, u.u);
    };

    void putF64(Buffer::It& buffer, double value) {
        union { double f; unsigned long long u ;} u;
        u.f = value;
        putU64(buffer, u.u);
    };
};

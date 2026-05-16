#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <dlfcn.h>
#endif

#include <list>
#include <algorithm>

/// Mutual exclusion primitive for thread synchronization
namespace Mutex {
    /// Platform-specific mutex handle
#if defined(_WIN32) || defined(_WIN64)
    struct It {
        CRITICAL_SECTION handle;
    };
#else
    struct It {
        pthread_mutex_t handle;
    };
#endif

    /// Create and initialize a new mutex
    /// @returns Initialized mutex object
    Mutex::It create();
    /// Destroy a mutex and release its resources
    /// @param mutex The mutex to destroy
    void destroy(Mutex::It& mutex);
    /// Acquire a mutex, blocking until it is available
    /// @param mutex The mutex to lock
    void lock(Mutex::It& mutex);
    /// Release a previously acquired mutex
    /// @param mutex The mutex to unlock
    void unlock(Mutex::It& mutex);
};

/// Little-endian byte order constant
#define UHC_LITTLE_ENDIAN 0
/// Big-endian byte order constant
#define UHC_BIG_ENDIAN 1

/// Binary buffer with cursor-based reading and writing
namespace Buffer {
    /// Buffer object
    struct It {
        void* handle;
        unsigned char order;
        unsigned int cursor;
    };

    /// Set the byte order for subsequent get/put operations
    /// @param buffer The buffer
    /// @param order Byte order — use UHC_LITTLE_ENDIAN or UHC_BIG_ENDIAN
    void setOrder(Buffer::It& buffer, unsigned char order);
    /// Reset the buffer cursor to the beginning
    /// @param buffer The buffer
    void reset(Buffer::It& buffer);
    /// Read a signed 8-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    char getI8(Buffer::It& buffer);
    /// Read an unsigned 8-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    unsigned char getU8(Buffer::It& buffer);
    /// Read a signed 16-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    short getI16(Buffer::It& buffer);
    /// Read an unsigned 16-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    unsigned short getU16(Buffer::It& buffer);
    /// Read a signed 32-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    int getI32(Buffer::It& buffer);
    /// Read an unsigned 32-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    unsigned int getU32(Buffer::It& buffer);
    /// Read a signed 64-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    long long getI64(Buffer::It& buffer);
    /// Read an unsigned 64-bit integer from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    unsigned long long getU64(Buffer::It& buffer);
    /// Read a 32-bit float from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    float getF32(Buffer::It& buffer);
    /// Read a 64-bit float from the buffer
    /// @param buffer The buffer
    /// @returns The value read
    double getF64(Buffer::It& buffer);
    /// Write a signed 8-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putI8(Buffer::It& buffer, char value);
    /// Write an unsigned 8-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putU8(Buffer::It& buffer, unsigned char value);
    /// Write a signed 16-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putI16(Buffer::It& buffer, short value);
    /// Write an unsigned 16-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putU16(Buffer::It& buffer, unsigned short value);
    /// Write a signed 32-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putI32(Buffer::It& buffer, int value);
    /// Write an unsigned 32-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putU32(Buffer::It& buffer, unsigned int value);
    /// Write a signed 64-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putI64(Buffer::It& buffer, long long value);
    /// Write an unsigned 64-bit integer to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putU64(Buffer::It& buffer, unsigned long long value);
    /// Write a 32-bit float to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putF32(Buffer::It& buffer, float value);
    /// Write a 64-bit float to the buffer
    /// @param buffer The buffer
    /// @param value The value to write
    void putF64(Buffer::It& buffer, double value);
};

/// Lightweight thread creation and joining
namespace Thread {
    /// Thread entry point function type
    typedef void* (*Fn)(void*);

    /// Platform-specific thread handle
#if defined(_WIN32) || defined(_WIN64)
    struct It {
        HANDLE handle;
    };
#else
    struct It {
        pthread_t handle;
    };
#endif

    /// Start a new thread running the given function
    /// @param method The thread entry function
    /// @param arg Argument passed to the thread function
    /// @returns Thread handle — use with join
    Thread::It start(Thread::Fn method, void* arg);
    /// Wait for a thread to finish
    /// @param thread The thread to wait on
    void join(Thread::It& thread);
};

/// Cross-platform shared library loader
namespace DynamicLibrary {
    /// Platform-specific dynamic library handle
#if defined(_WIN32) || defined(_WIN64)
    struct It {
        HINSTANCE handle;
        char* name;
        unsigned char isValid;
    };
#else
    struct It {
        void* handle;
        char* name;
        unsigned char isValid;
    };
#endif

    /// Load a shared library by name or path
    /// @param name Library name or path
    /// @returns Library object — check isValid before use
    DynamicLibrary::It load(const char* name);
    /// Retrieve a function pointer from a loaded library
    /// @param dll The dynamic library
    /// @param functionName The exported symbol name
    /// @returns Raw function pointer — cast before calling
    void* get(DynamicLibrary::It& dll, const char* functionName);
    /// Unload a dynamic library and free its resources
    /// @param dll The dynamic library
    void unload(DynamicLibrary::It& dll);
};


/// Generic doubly-linked list
namespace List {
    /// List container object
    template<typename T>
    struct It {
        std::list<T> handle;
    };

    /// Append a value to the end of the list
    /// @param list Target list
    /// @param value Value to append
    template<typename T>
    void add(List::It<T>& list, T value) {
        list.handle.push_back(value);
    };

    /// Insert a value at a specific index
    /// @param list Target list
    /// @param value Value to insert
    /// @param index Zero-based position
    template<typename T>
    void addAt(List::It<T>& list, T value, int index) {
        auto it = list.handle.begin();
        std::advance(it, index);
        list.handle.insert(it, value);
    };

    /// Remove the first matching value from the list
    /// @param list Target list
    /// @param value Value to find and remove
    /// @returns Non-zero if element was found and removed
    template<typename T>
    unsigned char remove(List::It<T>& list, T value) {
        auto it = std::find(list.handle.begin(), list.handle.end(), value);
        if (it == list.handle.end()) return 0;
        list.handle.erase(it);
        return 1;
    };

    /// Remove the element at a specific index
    /// @param list Target list
    /// @param index Zero-based position to remove
    template<typename T>
    void removeAt(List::It<T>& list, int index) {
        auto it = list.handle.begin();
        std::advance(it, index);
        list.handle.erase(it);
    };

    /// Get a pointer to the element at a specific index
    /// @param list Target list
    /// @param index Zero-based position
    /// @returns Pointer to the element
    template<typename T>
    T* get(List::It<T>& list, int index) {
        auto it = list.handle.begin();
        std::advance(it, index);
        return &(*it);
    };

    /// Set the value of an element at a specific index
    /// @param list Target list
    /// @param index Zero-based position
    /// @param value New value
    template<typename T>
    void set(List::It<T>& list, int index, T value) {
        auto it = list.handle.begin();
        std::advance(it, index);
        *it = value;
    };

    /// Get a pointer to the first element
    /// @param list Target list
    /// @returns Pointer to first element, or NULL if empty
    template<typename T>
    T* first(List::It<T>& list) {
        if (list.handle.empty()) return NULL;
        return &list.handle.front();
    };

    /// Get a pointer to the last element
    /// @param list Target list
    /// @returns Pointer to last element, or NULL if empty
    template<typename T>
    T* last(List::It<T>& list) {
        if (list.handle.empty()) return NULL;
        return &list.handle.back();
    };

    /// Get the number of elements in the list
    /// @param list Target list
    /// @returns Element count
    template<typename T>
    unsigned long long size(List::It<T>& list) {
        return (unsigned long long)list.handle.size();
    };

    /// Remove all elements from the list
    /// @param list Target list
    template<typename T>
    void clear(List::It<T>& list) {
        list.handle.clear();
    };
};

#include <stdio.h>
#include <string.h>

#include "uhcio.hh"
#include "uhcstd.hh"

namespace DynamicLibrary {

    static const DynamicLibrary::It invalidLibrary = {NULL, 0, 0};

#if defined(_WIN32) || defined(_WIN64)

    DynamicLibrary::It load(const char* name) {
        DynamicLibrary::It dll;

        dll.handle = LoadLibrary(name);
        if (dll.handle == NULL) {
            Log::warn("Couldn't load library");
            return invalidLibrary;
        };

        dll.name = strdup(name);
        dll.isValid = 1;

        return dll;
    };

    void* get(DynamicLibrary::It& dll, const char* functionName) {
        void* func = (void*)(GetProcAddress(dll.handle, functionName));

        if (func == NULL) {
            Log::warn("Loading function %s", functionName);
        };

        return func;
    };

    void unload(DynamicLibrary::It& dll) {
        int r = FreeLibrary(dll.handle);

        dll.isValid = 0;

        if (r == 0) {
            Log::warn("Failed freeing library");
        };

        free(dll.name);
    };

#else

    DynamicLibrary::It load(const char* name) {
        DynamicLibrary::It dll;

        dll.handle = dlopen(name, RTLD_LAZY);
        if (dll.handle == NULL) {
            Log::warn("Couldn't load library");
            return invalidLibrary;
        };

        dll.name = strdup(name);
        dll.isValid = 1;

        return dll;

    };

    void* get(DynamicLibrary::It& lib, const char* functionName) {
        void* func = dlsym(lib.handle, functionName);

        if (func == NULL) {
            Log::warn("Loading function %s", functionName);
        };

        return func;
    };

    void unload(DynamicLibrary::It& dll) {
        if (dll.handle != NULL) {
            dlclose(dll.handle);
        };

        dll.isValid = 0;

        free(dll.name);
    };

#endif
};

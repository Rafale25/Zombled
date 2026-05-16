#include "uhcstd.hh"

namespace Mutex {
#if defined(_WIN32) || defined(_WIN64)
    Mutex::It create() {
        Mutex::It mutex;
        InitializeCriticalSection(&mutex.handle);

        return mutex;
    };

    void destroy(Mutex::It& mutex) {
        DeleteCriticalSection(&mutex.handle);
    };

    void lock(Mutex::It& mutex) {
        EnterCriticalSection(&mutex.handle);
    };

    void unlock(Mutex::It& mutex) {
        LeaveCriticalSection(&mutex.handle);
    };
#else
    Mutex::It create() {
        Mutex::It mutex;
        pthread_mutex_init(&mutex.handle, NULL);
        
        return mutex;
    };

    void destroy(Mutex::It& mutex) {
        pthread_mutex_destroy(&mutex.handle);
    };

    void lock(Mutex::It& mutex) {
        pthread_mutex_lock(&mutex.handle);
    };

    void unlock(Mutex::It& mutex) {
        pthread_mutex_unlock(&mutex.handle);
    };
#endif
};

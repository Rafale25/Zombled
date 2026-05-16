#include "uhcio.hh"

namespace File {
    File::It open(const char* filePath) {
        File::It file;
        
        FILE* handle = fopen(filePath, "rb");
        if (!handle) {
            Log::warn("Couldn't find file");
            return file;
        };

        file.handle = handle;

        fseek(file.handle, 0, SEEK_END);
        long long fileSize = ftell(file.handle);
        rewind(file.handle);

        file.size = fileSize;

        return file;
    };

    void close(File::It& file) {
        fclose(file.handle);
    };

    char* read(File::It& file) {
        if (!file.size) {
            Log::warn("File size error");
            fclose(file.handle);
            return NULL;
        };

        char* buffer = (char*)malloc(file.size + 1);
        if (!buffer) {
            Log::warn("File allocation failed");
            fclose(file.handle);
            return NULL;
        };

        long long data = fread(buffer, 1, file.size, file.handle);
        if (data != file.size) {
            Log::warn("Reading file failed");
            fclose(file.handle);
            return NULL;
        };
        
        buffer[file.size] = 0x00;
        fclose(file.handle);

        return buffer;
    };
};

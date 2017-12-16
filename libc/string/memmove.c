#include <string.h>

void* memmove(void* dest, const void* src, size_t count) {
    unsigned char* dest_buf = (unsigned char*) dest;
    const unsigned char* src_buf = (const unsigned char*) src;

    if (dest_buf < src_buf) {
        for (size_t i = 0; i < count; ++i) {
            dest_buf[i] = src_buf[i];
        }
    } else {
        for (size_t i = count; i != 0; --i) {
            dest_buf[i - 1] = src_buf[i - 1];
        }
    }   

    return dest;
}

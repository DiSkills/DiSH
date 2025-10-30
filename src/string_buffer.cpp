#include <string.h>

#include "string_buffer.hpp"

void StringBuffer::Resize()
{
    capacity += INIT_STRING_BUFFER_SIZE;
    char *new_buf = new char[capacity];
    for (unsigned int i = 0; i < size; i++) {
        new_buf[i] = buf[i];
    }
    if (buf) {
        delete[] buf;
    }
    buf = new_buf;
}

char *StringBuffer::GetString() const
{
    char *s = new char[size + 1];
    memcpy(s, buf, size);
    s[size] = '\0';
    return s;
}

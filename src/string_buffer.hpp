#ifndef STRING_BUFFER_HPP
#define STRING_BUFFER_HPP

#ifndef INIT_STRING_BUFFER_SIZE
#define INIT_STRING_BUFFER_SIZE 256
#endif

class StringBuffer {
    char *buf;
    unsigned int capacity, size;
public:
    StringBuffer() : buf(0), capacity(0), size(0) {}
    ~StringBuffer() { if (buf) delete[] buf; }
    void Append(char c)
        { if (size >= capacity) Resize(); buf[size] = c; size++; }
    char *GetString() const;
    void Clear() { size = 0; }
private:
    void Resize();
private:
    StringBuffer(const StringBuffer &);
    void operator=(const StringBuffer &);
};

#endif

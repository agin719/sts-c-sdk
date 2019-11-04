#ifndef _STS_BUF_H_
#define _STS_BUF_H_

#include <stdlib.h>
#include <string.h>

#define Buffer(name, len)                \
    char *name = (char*) malloc(len);    \
    int name##_pos = 0;                  \
    int name##_len = len;                \
    memset(name, 0, len);               

#define BufferAppend0(name, str)                     \
    do {                                             \
        int _slen = strlen(str);                     \
        BufferAppend(name, str, _slen);              \
    } while (0);

#define BufferAppend(name, str, len)                 \
    do {                                             \
        if (len > name##_len - name##_pos) {         \
            char *tmp;                               \
            name##_len = name##_len * 2 + len;       \
            tmp = (char*) malloc(name##_len);        \
            memset(tmp, 0, name##_len);              \
            memcpy(tmp, name, name##_pos);           \
            free(name);                              \
            name = tmp;                              \
        }                                            \
        memcpy(name+name##_pos, str, len);           \
        name##_pos += len;                           \
    } while (0);

#define BufferRelease(name)             \
    free(name)

#endif

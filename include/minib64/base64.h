#pragma once

#include <stddef.h>

typedef enum {
    MINIB64_SUCCESS = 0,
    MINIB64_INVALID_LENGTH,
    MINIB64_INVALID_CHARACTER,
    MINIB64_INVALID_PADDING,
} minib64_status_t;

char *
minib64_encode(const unsigned char *data, size_t len);

minib64_status_t
minib64_decode(const char* in, size_t in_len, unsigned char **out, size_t* out_len);
